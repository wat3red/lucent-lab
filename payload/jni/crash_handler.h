#include <fstream>
#include <dlfcn.h>
#include <unwind.h>
#include <cxxabi.h>

#include "logger.h"
#include "utils.h"

namespace CrashHandler {
    uintptr_t g_BaseAddress = 0;
    struct sigaction g_OldSA[NSIG];

    struct BacktraceState {
        void** current;
        void** end;
    };

    void LogSymbolInfo(void* addr) {
        Dl_info info;
        if (dladdr(addr, &info) && info.dli_sname) {
            int status;
            const char* demangled = abi::__cxa_demangle(info.dli_sname, nullptr, nullptr, &status);
            // LOGE("DL name: %s", info.dli_sname);
            if (demangled)
                LOGE("Symbol: %s", demangled);
            else
                LOGE("Symbol is null");
        }
        else {
            // LOGE("Failed to resolve symbol info for %p", addr);
        }
    }

    static _Unwind_Reason_Code UnwindCallback(_Unwind_Context* context, void* arg) {
        BacktraceState* state = static_cast<BacktraceState*>(arg);
        uintptr_t pc = _Unwind_GetIP(context);
        if (pc && state->current < state->end) {
            *state->current++ = reinterpret_cast<void*>(pc);
        }
        return (pc && state->current < state->end) ? _URC_NO_REASON : _URC_END_OF_STACK;
    }

    size_t CaptureBacktrace(void** buffer, size_t max) {
        BacktraceState state = { buffer, buffer + max };
        _Unwind_Backtrace(UnwindCallback, &state);
        return state.current - buffer;
    }

    void LogRegisters(ucontext_t* ctx) {
        const mcontext_t& m = ctx->uc_mcontext;

        LOGE("=== Register state ===");
        LOGE("R0 : %p", (void*)m.arm_r0);
        LOGE("R1 : %p", (void*)m.arm_r1);
        LOGE("R2 : %p", (void*)m.arm_r2);
        LOGE("R3 : %p", (void*)m.arm_r3);
        LOGE("R4 : %p", (void*)m.arm_r4);
        LOGE("R5 : %p", (void*)m.arm_r5);
        LOGE("R6 : %p", (void*)m.arm_r6);
        LOGE("R7 : %p", (void*)m.arm_r7);
        LOGE("R8 : %p", (void*)m.arm_r8);
        LOGE("R9 : %p", (void*)m.arm_r9);
        LOGE("R10: %p", (void*)m.arm_r10);
        LOGE("FP : %p", (void*)m.arm_fp);
        LOGE("IP : %p", (void*)m.arm_ip);
        LOGE("SP : %p", (void*)m.arm_sp);
        LOGE("LR : %p", (void*)m.arm_lr);
        LOGE("PC : %p", (void*)m.arm_pc);
        LOGE("CPSR: %p", (void*)m.arm_cpsr);
        LOGE("=======================");
    }

    std::string FindLibraryForAddress(uintptr_t addr) {
        std::ifstream maps(oxorany("/proc/self/maps"));
        std::string line;

        while (std::getline(maps, line)) {
            uintptr_t start, end;
            char path[512] = { 0 };
            if (sscanf(line.c_str(), oxorany("%p-%p %*s %*s %*s %*s %511[^\n]"), (void**)&start, (void**)&end, path) == 3) {
                if (addr >= start && addr <= end) {
                    return std::string(path);
                }
            }
        }
        return oxorany("Unknown");
    }

    std::vector<std::string> Split(const std::string& s, const std::string& delimiter) {
        std::vector<std::string> tokens;
        std::string temp_s = s; // Create a mutable copy of the input string
        size_t pos = 0;
        std::string token;
        while ((pos = temp_s.find(delimiter)) != std::string::npos) {
            token = temp_s.substr(0, pos);
            tokens.push_back(token);
            temp_s.erase(0, pos + delimiter.length()); // Erase from the copy
        }
        tokens.push_back(temp_s); // Add the remaining part

        return tokens;
    }

    void SignalHandler(int signal, siginfo_t* info, void* context) {
        LOGE("=== Caught signal %d (%s) at address: %p ===", signal, strsignal(signal), info->si_addr);
        LOGE("PID: %d, TID: %d", getpid(), gettid());

        LogRegisters((ucontext_t*)context);

        void* callstack[100];
        size_t frames = CaptureBacktrace(callstack, 100);
        LOGE("Captured %zu frames:", frames);

        for (size_t i = 0; i < frames; ++i) {
            uintptr_t addr = reinterpret_cast<uintptr_t>(callstack[i]);
            std::string libPath = FindLibraryForAddress(addr);
            uintptr_t offset = addr - GetModuleBase(libPath.c_str());
            std::string libName = FindLibraryForAddress(addr);
            auto libNameParts = Split(libName, oxorany("/"));

            LOGE("-----------------");
            LOGE("#%02zu: offset 0x%x from %s", i, offset, libNameParts.at(libNameParts.size() - 1).c_str());
            LogSymbolInfo(callstack[i]);
            LOGE("-----------------");
        }

        sigaction(signal, &g_OldSA[signal], nullptr);
        fflush(stdout);
        fflush(stderr);
        raise(signal);
    }

    void LogBacktrace(const char* callerName) {
        void* callstack[100];
        size_t frames = CaptureBacktrace(callstack, 100);

        LOGE("=== Backtrace from %s ===", callerName);

        for (size_t i = 0; i < frames; ++i) {
            uintptr_t addr = reinterpret_cast<uintptr_t>(callstack[i]);
            std::string libPath = FindLibraryForAddress(addr);
            uintptr_t offset = addr - GetModuleBase(libPath.c_str());
            auto libPathParts = Split(libPath, oxorany("/"));

            LOGE("#%02zu: offset 0x%x from %s", i, offset, libPathParts.at(libPathParts.size() - 1).c_str());
            LogSymbolInfo(callstack[i]);
        }

        LOGE("============================");
    }

    void InitializeCrashHandler() {
        g_BaseAddress = GetModuleBase(oxorany("libdaemon.so"));
        LOGD("Base address: %p", (void*)g_BaseAddress);

        struct sigaction sa {};
        sa.sa_sigaction = SignalHandler;
        sa.sa_flags = SA_SIGINFO | SA_RESETHAND;

        stack_t ss;
        ss.ss_sp = malloc(SIGSTKSZ);
        ss.ss_size = SIGSTKSZ;
        ss.ss_flags = 0;
        sigaltstack(&ss, nullptr);
        sa.sa_flags |= SA_ONSTACK;

        sigaction(SIGSEGV, &sa, &g_OldSA[SIGSEGV]);
        sigaction(SIGABRT, &sa, &g_OldSA[SIGABRT]);
        sigaction(SIGFPE, &sa, &g_OldSA[SIGFPE]);
        sigaction(SIGILL, &sa, &g_OldSA[SIGILL]);
        sigaction(SIGBUS, &sa, &g_OldSA[SIGBUS]);
        sigaction(SIGPIPE, &sa, &g_OldSA[SIGPIPE]);

        LOGD("Crash handler installed");
    }
}