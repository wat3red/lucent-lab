#include <oxorany/oxorany.h>
#include <xorstr.h>

#include <fcntl.h>
#include <jni.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <dlfcn.h>

#include <chrono>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#include "timer.h"
#include "sdk/functions.h"
#include "crash_handler.h"
#include "cheat/function.h"
#include "network_manager/network_manager.h"
#include "utils.h"

#include "il2cpp/il2cpp_utils.h"
#include "il2cpp/il2cpp_structs.h"

#include "global/global_hooks.h"
#include "global/global_variables.h"

#include "sdk/singleton_manager.h"
#include "sdk/Vector3.h"
#include "sdk/axlebolt_classes.h"
#include "sdk/unity_classes.h"

#include "cheat/functions_manger.h"
#include "cheat/demo_ping.h"
#include "cheat/demo_config_echo.h"
#include "cheat/demo_frame_stats.h"

#include <signal.h>
#include <string.h>
#include <android/log.h>
#include <unwind.h>
#include <cxxabi.h>
#include <asm/sigcontext.h>

#include <link.h>

uintptr_t g_LibUnityBase = 0;
size_t g_LibUnitySize = 0;
FunctionsManager g_FunctionsManager;

void GameVersionCheck() {
    PhotonNetworkStaticFields* photonNetworkStaticFields = PhotonNetworkStaticFields::GetInstance();
    if (!photonNetworkStaticFields) return;

    NetworkingPeer* networkingPeer = photonNetworkStaticFields->m_NetworkingPeer;
    if (!networkingPeer) return;

    Il2CppString* gameVersion = PhotonRoomExtension_GetGameVersion(networkingPeer->m_CurrentRoom);
    if (!gameVersion) return;

    // LOGD("Client Version: %s", gameVersion->c_str());

    if (strcmp(gameVersion->c_str(), oxorany("0.35.3F1")) != 0) {
        LOGE("version is not 0.35.3F1");
    }
}

// void MainUpdate() {
//     Timer timer1(oxorany("SingletonManager::Instance().Update();"));
//     SingletonManager::Instance().Update();
//     timer1.Stop();

//     Timer timer2(oxorany("Function::Update();"));
//     for (auto* func : g_FunctionsManager.GetFunctions())
//         func->Update();
//     timer2.Stop();

//     Timer timer3(oxorany("FunctionsManager::SendAllData();"));
//     g_FunctionsManager.SendAllData();
//     timer3.Stop();
// }


struct FindResult {
    uintptr_t base;
    uintptr_t text_addr;
    size_t text_size;
    std::string name;
    bool found;
};

static int phdr_callback(struct dl_phdr_info* info, size_t size, void* data) {
    FindResult* res = reinterpret_cast<FindResult*>(data);
    if (!info->dlpi_name) return 0;
    std::string sname = info->dlpi_name;
    // some entries show empty dlpi_name for the main executable; match substring
    if (sname.find(res->name) == std::string::npos) return 0;

    uintptr_t base = (uintptr_t)info->dlpi_addr;
    // iterate program headers of this shared object
    for (int i = 0; i < info->dlpi_phnum; ++i) {
        const ElfW(Phdr)& ph = info->dlpi_phdr[i];
        if (ph.p_type == PT_LOAD && (ph.p_flags & PF_X)) {
            // executable loadable segment -> text
            res->base = base;
            res->text_addr = base + ph.p_vaddr;
            res->text_size = ph.p_memsz;
            res->found = true;
            return 1; // stop iteration
        }
    }
    return 0;
}

bool GetModuleTextSection(const std::string& moduleName, uintptr_t& textBase, size_t& textSize) {
    FindResult res = {};
    res.name = moduleName;
    res.found = false;
    dl_iterate_phdr(phdr_callback, &res);
    if (!res.found) return false;
    textBase = res.text_addr;
    textSize = res.text_size;
    return true;
}

void MainThread() {
    CrashHandler::InitializeCrashHandler();

    // int* a = (int*)0x1337;
    // *a = 0; // to test crash handler

    g_LibUnityBase = GetModuleBase(oxorany("libunity.so"));
    g_LibUnitySize = GetModuleSize(oxorany("libunity.so"));

    if (!g_LibUnityBase) {
        LOGE("Failed to find libunity.so");
        return;
    }

    LOGD("libunity.so base: %p, its size: %zu", (void*)g_LibUnityBase, g_LibUnitySize);

    uintptr_t textBase = 0;
    size_t textSize = 0;

    if (GetModuleTextSection(oxorany("libunity.so"), textBase, textSize)) {
        LOGD("libunity.so text base: %p, its size: %zu", (void*)textBase, textSize);
        InitFunctionPointers(textBase, textSize);
    }
    else {
        // Fallback: if dl_iterate_phdr couldn't locate the executable segment, use the module base/size
        // This ensures InitFunctionPointers still has a valid range to scan.
        LOGE("Failed to find libunity.so text section via dl_iterate_phdr, falling back to module base/size");
        textBase = g_LibUnityBase;
        textSize = g_LibUnitySize;
        InitFunctionPointers(textBase, textSize);
    }

    LOGD("Done with initializing functions");

    // Lab/portfolio build: protocol demos only. Competitive modules are not registered.
    g_FunctionsManager.AddFunction(new LabPing());
    g_FunctionsManager.AddFunction(new LabConfigEcho());
    g_FunctionsManager.AddFunction(new LabFrameStats());

    g_FunctionsManager.InitFunctions();

    NetworkManager networkManager;
    // if (!networkManager.Connect(oxorany("127.0.0.1"), 64108, true)) {
    //     LOGE("Failed to start server");
    //     return;
    // }
    // LOGI("Connected to the client");

    // networkManager.SetRecieveDataCallback([](const std::vector<uint8_t>& data) {
    //     g_FunctionsManager.HandleIncomingData(data.data(), data.size());
    //     // usleep(1000);
    //     usleep(5000);
    //     });

    g_FunctionsManager.SetNetworkManager(&networkManager);

    // Global::Hooks::AddLateUpdateCallback(GameVersionCheck);
    Global::Hooks::InitHooks();
    Global::Hooks::SetFunctionsManager(&g_FunctionsManager);

    Il2CppDomain* domain = il2cpp_domain_get();
    if (!domain) {
        LOGE("Failed to get IL2CPP domain.");
        return;
    }

    // attaching the MainThread thread to the IL2CPP
    Il2CppThread* attached_thread = il2cpp_thread_attach(domain);
    if (!attached_thread) {
        LOGE("Failed to attach thread to IL2CPP.");
        return;
    }

    while (true) {
        SingletonManager::Instance().Update();

        for (auto* func : g_FunctionsManager.GetFunctions())
            func->Update();

        g_FunctionsManager.SendAllData();

        usleep(10000);

        // usleep(10000000);
    }
}

extern "C" __unused __attribute__((constructor)) void constructor_main() {
    std::thread([]() {
        LOGI("MainThread started");

        try {
            MainThread();
        }
        catch (const std::exception& e) {
            LOGE("Unhandled exception: %s", e.what());
        }
        catch (...) {
            LOGE("Unknown exception caught in thread!");
        }

        LOGI("exiting MainThread");
        }).detach();

    return;
}