#include "anti_debug.h"
#include "../oxorany/oxorany_include.h"
#include "../logger.h"
#include "../auth/api/auth.h"
#include "../auth/ui///LOGIn_window.h"
#include "scylla_hide_detector/scylla_detector.hpp"

#include "crc32.h"

#include <thread>
#include <unordered_map>
#include <winternl.h>
#include <TlHelp32.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#pragma intrinsic(__rdtsc)

typedef struct _OBJECT_TYPE_INFORMATION
{
	UNICODE_STRING TypeName;
	ULONG TotalNumberOfHandles;
	ULONG TotalNumberOfObjects;
} OBJECT_TYPE_INFORMATION, * POBJECT_TYPE_INFORMATION;

// Custom definition for NtQueryInformationProcess to avoid dependency on specific winternl.h versions
typedef NTSTATUS(WINAPI* PNtQueryInformationProcess)(
	IN HANDLE ProcessHandle,
	IN PROCESSINFOCLASS ProcessInformationClass,
	OUT PVOID ProcessInformation,
	IN ULONG ProcessInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);

// Custom definition for NtSetInformationThread
typedef NTSTATUS(WINAPI* PNtSetInformationThread)(
	IN HANDLE ThreadHandle,
	IN THREADINFOCLASS ThreadInformationClass,
	IN PVOID ThreadInformation,
	IN ULONG ThreadInformationLength
	);

typedef NTSTATUS(WINAPI* NtSetInformationThread_t)(HANDLE, ULONG, PVOID, ULONG);
#define ThreadHideFromDebugger 0x11

// Structure for hardware breakpoint check
CONTEXT g_context;
bool g_bHardwareBreakpoint = false;
LONG WINAPI HardwareBreakpointHandler(PEXCEPTION_POINTERS pExceptionInfo) {
	if (pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP) {
		// This is the key   a hardware breakpoint triggers a single-step exception.
		// If the context record shows it was triggered by one of our Dr registers, a debugger is present.
		if (pExceptionInfo->ContextRecord->Dr0 == (DWORD_PTR)&g_context ||
			pExceptionInfo->ContextRecord->Dr1 == (DWORD_PTR)&g_context ||
			pExceptionInfo->ContextRecord->Dr2 == (DWORD_PTR)&g_context ||
			pExceptionInfo->ContextRecord->Dr3 == (DWORD_PTR)&g_context) {
			g_bHardwareBreakpoint = true;
		}
		pExceptionInfo->ContextRecord->EFlags |= 0x10000; // Resume execution
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

// Master check
bool AntiDebug::IsDebugged(std::string& reason) {
	if (scylla::is_attached()) {
		reason = oxorany("ScyllaHide");
		return true;
	}

	if (CheckDebugFlags()) {
		reason = oxorany("Debug flags");
		return true;
	}
	if (CheckObjectHandles()) {
		reason = oxorany("Object handles");
		return true;
	}
	if (CheckExceptions()) {
		reason = oxorany("Exceptions");
		return true;
	}
	// if (CheckTiming()) {
	//     reason = oxorany("Timing");
	//     return true;
	// }
	if (CheckProcessMemory()) {
		reason = oxorany("Process memory");
		return true;
	}
	// if (CheckAssembly()) {
	//     reason = oxorany("Assembly");
	//     return true;
	// }
	if (CheckHardwareBreakpoint()) {
		reason = oxorany("Hardware breakpoint");
		return true;
	}
	if (CheckInteractive()) {
		reason = oxorany("Interactive");
		return true;
	}
	if (CheckMisc()) {
		reason = oxorany("Miscellaneous");
		return true;
	}

	reason.clear(); // не обнаружено отладчика
	return false;
}

struct FunctionData {
	PVOID addr;
	DWORD size;
	DWORD originalChecksum;
};
std::vector<FunctionData> observedFunctions;

static DWORD WINAPI ObserveCodeChanges(LPVOID lpThreadParameter)
{
	while (true)
	{
		for (const auto& func : observedFunctions) {
			if (CRC32((PBYTE)func.addr, func.size) != func.originalChecksum)
				ExitProcess(0);
		}
		Sleep(10000);
	}
	return 0;
}

size_t DetectFunctionSize(PVOID pFunc)
{
	PBYTE pMem = (PBYTE)pFunc;
	size_t nFuncSize = 0;
	do
	{
		++nFuncSize;
	} while (*(pMem++) != 0xC3);
	return nFuncSize;
}

void InstallObserver(PVOID function) {
	DWORD size = DetectFunctionSize(function);
	FunctionData functionData{ function, size, CRC32(function, size) };
	observedFunctions.push_back(functionData);
}

void AntiDebug::InstallCodeIntegrityObservers() {
	InstallObserver(LoginWindow::Draw);
	InstallObserver(LoginWindow::Init);

	InstallObserver(AntiDebug::Init);
	InstallObserver(AntiDebug::IsDebugged);
	InstallObserver(AntiDebug::CheckAssembly);
	InstallObserver(AntiDebug::CheckDebugFlags);
	InstallObserver(AntiDebug::CheckExceptions);
	InstallObserver(AntiDebug::CheckHardwareBreakpoint);
	InstallObserver(AntiDebug::CheckInteractive);
	InstallObserver(AntiDebug::CheckMisc);
	InstallObserver(AntiDebug::CheckObjectHandles);
	InstallObserver(AntiDebug::CheckProcessMemory);
	InstallObserver(AntiDebug::CheckTiming);
	InstallObserver(AntiDebug::InstallCodeIntegrityObservers);
	InstallObserver(AntiDebug::Patch_DbgBreakPoint);
	InstallObserver(AntiDebug::Patch_DbgUiRemoteBreakin);

	CreateThread(NULL, oxorany(NULL), ObserveCodeChanges, NULL, NULL, NULL);
}

bool HideThreadFromDebugger(HANDLE hThread) {
	HMODULE hNtdll = GetModuleHandleA(oxorany("ntdll.dll"));
	if (!hNtdll) return false;
	NtSetInformationThread_t NtSetInformationThread = (NtSetInformationThread_t)GetProcAddress(hNtdll, oxorany("NtSetInformationThread"));
	if (!NtSetInformationThread) return false;

	return NtSetInformationThread(hThread, ThreadHideFromDebugger, 0, 0) == 0;
}

DWORD WINAPI AntiDebugRoutine(LPVOID) {
	std::string reason;
	if (AntiDebug::IsDebugged(reason)) {
		// log + optionally notify server
		MessageBoxA(nullptr, reason.c_str(), oxorany("Anti-debug system triggered"), MB_OK);
		ExitProcess(1);
	}
	return 0;
}

DWORD __stdcall AntiDebug::AntiDebugWatchdog(LPVOID)
{
	while (true) {
		HANDLE hThread = CreateThread(nullptr, 0, AntiDebugRoutine, nullptr, 0, nullptr);
		if (hThread) {
			HideThreadFromDebugger(hThread); // optional, defense-in-depth
			WaitForSingleObject(hThread, 2000);
			CloseHandle(hThread);
		}
		Sleep(5000); // adjust as needed
	}
	return 0;
}

void AntiDebug::Init() {
	Patch_DbgBreakPoint();
	Patch_DbgUiRemoteBreakin();

	InstallCodeIntegrityObservers();


	std::thread([] {
		HANDLE h = CreateThread(nullptr, 0, AntiDebugWatchdog, nullptr, 0, nullptr);
		if (h) {
			HideThreadFromDebugger(h); // harden the watchdog thread itself
			CloseHandle(h);
		}
		}).detach();
}

// Category: Debug Flags
bool AntiDebug::CheckDebugFlags() {
	// 1. IsDebuggerPresent()
	// The most common and basic check.
	if (IsDebuggerPresent()) {
		return true;
	}

	// 2. CheckRemoteDebuggerPresent()
	// Checks if a remote debugger is attached.
	BOOL isRemoteDebuggerPresent = FALSE;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &isRemoteDebuggerPresent);
	if (isRemoteDebuggerPresent) {
		return true;
	}

	// 3. PEB (Process Environment Block) Checks
#ifdef _WIN64
	PPEB pPeb = (PPEB)__readgsqword(0x60);
#else
	PPEB pPeb = (PPEB)__readfsdword(0x30);
#endif
	// PEB->BeingDebugged
	if (pPeb->BeingDebugged) {
		return true;
	}
	// PEB->NtGlobalFlag
	// This flag is set when a process is launched under a debugger.
#ifdef _WIN64
	DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE)pPeb + oxorany(0xBC));
#else
	DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE)pPeb + 0x68);
#endif
	if (dwNtGlobalFlag & 0x70) {
		return true;
	}

	// 4. NtQueryInformationProcess
	// Using ntdll functions to query process information.
	HMODULE hNtdll = LoadLibraryA(oxorany("ntdll.dll"));
	if (hNtdll) {
		PNtQueryInformationProcess pNtQueryInformationProcess = (PNtQueryInformationProcess)GetProcAddress(hNtdll, oxorany("NtQueryInformationProcess"));
		if (pNtQueryInformationProcess) {
			// ProcessDebugPort: if a debugger is attached, this will be non-zero.
			DWORD_PTR dwDebugPort = 0;
			NTSTATUS status = pNtQueryInformationProcess(GetCurrentProcess(), ProcessDebugPort, &dwDebugPort, sizeof(dwDebugPort), NULL);
			if (NT_SUCCESS(status) && dwDebugPort != 0) {
				FreeLibrary(hNtdll);
				return true;
			}

			// ProcessDebugFlags: Another flag that indicates debugging. The value should be 1 if not debugging.
			ULONG dwDebugFlags = 0;
			status = pNtQueryInformationProcess(GetCurrentProcess(), (PROCESSINFOCLASS)0x1f, &dwDebugFlags, sizeof(dwDebugFlags), NULL); // ProcessDebugFlags is 31 (0x1f)
			if (NT_SUCCESS(status) && dwDebugFlags == 0) {
				FreeLibrary(hNtdll);
				return true;
			}
		}
		FreeLibrary(hNtdll);
	}

	return false;
}

// Category: Object Handles
bool AntiDebug::CheckObjectHandles() {
	// 1. CloseHandle on an invalid handle
	// Under a debugger, this raises a specific exception (STATUS_INVALID_HANDLE).
	// Without a debugger, it returns FALSE and GetLastError() is ERROR_INVALID_HANDLE.
	__try {
		CloseHandle((HANDLE)0xDEADBEEF);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		return true;
	}

	// 2. NtQueryObject with a null handle
	// Probing for the existence of Debugger objects.
	HMODULE hNtdll = GetModuleHandleA(oxorany("ntdll.dll"));
	if (hNtdll) {
		auto pNtQueryObject = (decltype(&NtQueryObject))GetProcAddress(hNtdll, oxorany("NtQueryObject"));
		if (pNtQueryObject) {
			char buffer[sizeof(OBJECT_TYPE_INFORMATION) + sizeof(WCHAR) * 32];
			if (NT_SUCCESS(pNtQueryObject(NULL, ObjectTypeInformation, &buffer, 0, (PULONG)&buffer)))
			{
				// This is an error condition that often indicates a debugger.
				// A successful call with a NULL handle should not happen.
				return true;
			}
		}
	}
	return false;
}


// Category: Exceptions
bool AntiDebug::CheckExceptions() {
	// 1. INT 3 (__debugbreak)
	__try {
		__debugbreak();
	}
	__except (GetExceptionCode() == EXCEPTION_BREAKPOINT ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		return false;
	}
	return true;

	// 2. INT 2D via runtime shellcode
	BYTE int2d_code[] = { 0xCD, 0x2D, 0xC3 }; // int 2Dh; ret
	void* int2d_func = VirtualAlloc(nullptr, sizeof(int2d_code), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!int2d_func) return false;
	memcpy(int2d_func, int2d_code, sizeof(int2d_code));

	__try {
		((void(*)())int2d_func)();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		VirtualFree(int2d_func, 0, MEM_RELEASE);
		return false;
	}

	VirtualFree(int2d_func, 0, MEM_RELEASE);
	return true;

	// 3. REP INT 3 via runtime shellcode
	BYTE prefix_code[] = { 0xF3, 0xCC, 0xC3 }; // rep int3; ret
	void* prefix_func = VirtualAlloc(nullptr, sizeof(prefix_code), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!prefix_func) return false;
	memcpy(prefix_func, prefix_code, sizeof(prefix_code));

	__try {
		((void(*)())prefix_func)();
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		VirtualFree(prefix_func, 0, MEM_RELEASE);
		return false;
	}

	VirtualFree(prefix_func, 0, MEM_RELEASE);
	return true;
}

// Category: Timing
bool AntiDebug::CheckTiming() {
	const int runs = 10;
	std::vector<ULONGLONG> deltas;

	for (int i = 0; i < runs; ++i) {
		ULONGLONG start_time = __rdtsc();

		// Simulate a NOP-equivalent block
		Sleep(0); // Slight delay to simulate lightweight operation

		ULONGLONG end_time = __rdtsc();
		ULONGLONG delta = end_time - start_time;

		deltas.push_back(delta);
		Sleep(10);
	}

	std::sort(deltas.begin(), deltas.end());
	ULONGLONG median = deltas[runs / 2];

	// Log values for debug
	//LOGI("Timing: Deltas: ");
	for (auto d : deltas) //LOGI("  %llu", d);
		//LOGI("Timing: Median RDTSC delta = %llu", median);

		// Use a relaxed threshold, e.g., median must be below 5000 cycles
		if (median > 5000) {
			//LOGI("Timing: Triggered (median %llu > 5000)", median);
			return true;
		}

	return false;
}

void AntiDebug::Patch_DbgBreakPoint() {
	HMODULE hNtdll = GetModuleHandleA(oxorany("ntdll.dll"));
	if (!hNtdll)
		return;

	FARPROC pDbgBreakPoint = GetProcAddress(hNtdll, oxorany("DbgBreakPoint"));
	if (!pDbgBreakPoint)
		return;

	DWORD dwOldProtect;
	if (!VirtualProtect(pDbgBreakPoint, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		return;

	*(PBYTE)pDbgBreakPoint = (BYTE)0xC3; // ret
}

#pragma pack(push, 1)
struct DbgUiRemoteBreakinPatch
{
	WORD  push_0;
	BYTE  push;
	DWORD CurrentPorcessHandle;
	BYTE  mov_eax;
	DWORD TerminateProcess;
	WORD  call_eax;
};
#pragma pack(pop)

void AntiDebug::Patch_DbgUiRemoteBreakin() {
	HMODULE hNtdll = GetModuleHandleA(oxorany("ntdll.dll"));
	if (!hNtdll)
		return;

	FARPROC pDbgUiRemoteBreakin = GetProcAddress(hNtdll, oxorany("DbgUiRemoteBreakin"));
	if (!pDbgUiRemoteBreakin)
		return;

	HMODULE hKernel32 = GetModuleHandleA(oxorany("kernel32.dll"));
	if (!hKernel32)
		return;

	FARPROC pTerminateProcess = GetProcAddress(hKernel32, oxorany("TerminateProcess"));
	if (!pTerminateProcess)
		return;

	DbgUiRemoteBreakinPatch patch = { 0 };
	patch.push_0 = '\x6A\x00';
	patch.push = '\x68';
	patch.CurrentPorcessHandle = 0xFFFFFFFF;
	patch.mov_eax = '\xB8';
	patch.TerminateProcess = (DWORD)pTerminateProcess;
	patch.call_eax = '\xFF\xD0';

	DWORD dwOldProtect;
	if (!VirtualProtect(pDbgUiRemoteBreakin, sizeof(DbgUiRemoteBreakinPatch), PAGE_READWRITE, &dwOldProtect))
		return;

	::memcpy_s(pDbgUiRemoteBreakin, sizeof(DbgUiRemoteBreakinPatch),
		&patch, sizeof(DbgUiRemoteBreakinPatch));
	VirtualProtect(pDbgUiRemoteBreakin, sizeof(DbgUiRemoteBreakinPatch), dwOldProtect, &dwOldProtect);
}


// Category: Process Memory
bool AntiDebug::CheckProcessMemory() {
	// 1. Scanning for software breakpoints (0xCC)
	// Debuggers place 0xCC bytes (INT 3) to create breakpoints. We can scan for them in critical functions.
	HMODULE hKernel32 = GetModuleHandleA(oxorany("kernel32.dll"));
	if (hKernel32) {
		FARPROC pCreateThread = GetProcAddress(hKernel32, oxorany("CreateThread"));
		if (pCreateThread) {
			BYTE* pFirstByte = (BYTE*)pCreateThread;
			if (*pFirstByte == 0xCC) {
				return true;
			}
		}
	}

	// 2. Memory guards / PAGE_GUARD
	// A debugger stepping over a guarded page will trigger an exception that it might handle differently.
	SYSTEM_INFO sysInfo = { 0 };
	GetSystemInfo(&sysInfo);
	const DWORD pageSize = sysInfo.dwPageSize;

	PVOID pMem = VirtualAlloc(NULL, pageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!pMem) return false;

	DWORD oldProtect;
	if (!VirtualProtect(pMem, pageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &oldProtect)) {
		VirtualFree(pMem, 0, MEM_RELEASE);
		return false;
	}

	__try {
		// Accessing the guarded page will raise a STATUS_GUARD_PAGE_VIOLATION exception.
		// A debugger might swallow this exception or handle it in a way that differs from normal execution.
		*(int*)pMem = 123;
	}
	__except (GetExceptionCode() == STATUS_GUARD_PAGE_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		// Normal execution path, no debugger detected by this method.
		VirtualFree(pMem, 0, MEM_RELEASE);
		return false;
	}

	// If we reach here, the exception was handled by a debugger, not our handler.
	VirtualFree(pMem, 0, MEM_RELEASE);
	return true;
}

bool AntiDebug::CheckAssembly() {
	////LOGD("Assembly: Starting Trap Flag check...");

	CONTEXT ctx = {};
	ctx.ContextFlags = CONTEXT_CONTROL;

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, GetCurrentThreadId());
	if (!hThread) {
		////LOGE("Assembly: Failed to get real thread handle. Error: %lu", GetLastError());
		return false;
	}
	//LOGD("Assembly: hThread = %p", hThread);

	if (!GetThreadContext(hThread, &ctx)) {
		//LOGE("Assembly: GetThreadContext failed. Error: %lu", GetLastError());
		CloseHandle(hThread);
		return false;
	}

	ctx.EFlags |= 0x100; // Set Trap Flag

	if (!SetThreadContext(hThread, &ctx)) {
		//LOGE("Assembly: SetThreadContext failed. Error: %lu", GetLastError());
		CloseHandle(hThread);
		return false;
	}

	//LOGD("Assembly: before triggering EXCEPTION_SINGLE_STEP");

	__try {
		Sleep(1); // should trigger EXCEPTION_SINGLE_STEP
	}
	__except (GetExceptionCode() == EXCEPTION_SINGLE_STEP ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
		//LOGI("Assembly: Trap Flag triggered and exception handled — clean");
		CloseHandle(hThread);
		return false;
	}

	//LOGI("Assembly: Trap Flag did not trigger — suspicious");
	CloseHandle(hThread);
	return true;
}


bool AntiDebug::CheckHardwareBreakpoint() {
	g_bHardwareBreakpoint = false;
	PVOID pHandler = AddVectoredExceptionHandler(1, HardwareBreakpointHandler);

	g_context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if (!GetThreadContext(GetCurrentThread(), &g_context)) {
		//LOGE("Assembly: GetThreadContext for HW breakpoint failed");
		RemoveVectoredExceptionHandler(pHandler);
		return false;
	}

	g_context.Dr0 = (DWORD_PTR)&g_context;
	g_context.Dr7 = 1;

	if (!SetThreadContext(GetCurrentThread(), &g_context)) {
		//LOGE("Assembly: SetThreadContext for HW breakpoint failed");
		RemoveVectoredExceptionHandler(pHandler);
		return false;
	}

	// Note: No delay here to allow exception to be triggered passively
	RemoveVectoredExceptionHandler(pHandler);

	if (g_bHardwareBreakpoint) {
		//LOGI("Assembly: Hardware breakpoint was triggered");
		return true;
	}

	//LOGI("Assembly: No hardware breakpoint detected");
	return false;
}

// Category: Interactive
bool AntiDebug::CheckInteractive() {
	// 1. FindWindow for common debugger class names
	std::vector<const char*> windowClasses = {
		oxorany("OLLYDBG"), oxorany("WinDbgFrameClass"), oxorany("Zeta Debugger"),
		oxorany("x64dbg"), oxorany("x32dbg"), oxorany("TitanEngine"), oxorany("Rock Debugger")
	};

	for (const auto& cls : windowClasses) {
		if (FindWindowA(cls, NULL)) {
			return true;
		}
	}

	// 2. EnumProcesses for debugger process names
	std::vector<std::wstring> processNames = {
		oxorany(L"ollydbg.exe"), oxorany(L"windbg.exe"), oxorany(L"x64dbg.exe"), oxorany(L"x32dbg.exe"),
		oxorany(L"idaq.exe"), oxorany(L"idaq64.exe"), oxorany(L"cheatengine-x86_64.exe")
	};

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32W pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32FirstW(hSnap, &pe32)) {
			do {
				std::wstring procName(pe32.szExeFile);
				std::transform(procName.begin(), procName.end(), procName.begin(), ::tolower);
				for (const auto& name : processNames) {
					if (procName == name) {
						CloseHandle(hSnap);
						return true;
					}
				}
			} while (Process32NextW(hSnap, &pe32));
		}
		CloseHandle(hSnap);
	}

	return false;
}

// Category: Misc
bool AntiDebug::CheckMisc() {
	// 1. Thread Hiding (NtSetInformationThread with ThreadHideFromDebugger)
	// This is a powerful technique. A process can ask the kernel not to inform debuggers about this thread.
	// If the call succeeds, it's a strong indication we are not being debugged.
	// A debugger might hook this function to prevent it from working.
	HMODULE hNtdll = GetModuleHandleA(oxorany("ntdll.dll"));
	if (hNtdll) {
		PNtSetInformationThread pNtSetInformationThread = (PNtSetInformationThread)GetProcAddress(hNtdll, oxorany("NtSetInformationThread"));
		if (pNtSetInformationThread) {
			NTSTATUS status = pNtSetInformationThread(GetCurrentThread(), (THREADINFOCLASS)oxorany(0x11), NULL, 0); // ThreadHideFromDebugger = 0x11
			// If we are under a debugger, this call might fail or be intercepted.
			// A successful call is a good sign, but not foolproof.
			// The check is tricky: some debuggers might cause this to fail.
			// Let's assume failure indicates a problem.
			if (!NT_SUCCESS(status)) {
				return true;
			}
		}
	}

	// 2. Parent Process Check
	DWORD myPid = GetCurrentProcessId();
	DWORD parentPid = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32W pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32FirstW(hSnap, &pe32)) {
			do {
				if (pe32.th32ProcessID == myPid) {
					parentPid = pe32.th32ParentProcessID;
					break;
				}
			} while (Process32NextW(hSnap, &pe32));
		}

		if (parentPid != 0) {
			if (Process32FirstW(hSnap, &pe32)) {
				do {
					if (pe32.th32ProcessID == parentPid) {
						//std::wcout << "Parent process: " << std::wstring(pe32.szExeFile) << std::endl;
						if (_wcsicmp(pe32.szExeFile, oxorany(L"explorer.exe")) != 0 &&
							_wcsicmp(pe32.szExeFile, oxorany(L"HD-MultiInstanceManager.exe")) != 0 &&
							_wcsicmp(pe32.szExeFile, oxorany(L"cmd.exe")) != 0) {
							CloseHandle(hSnap);
							return true; // Suspicious
						}
						break;
					}
				} while (Process32NextW(hSnap, &pe32));
			}
		}
		CloseHandle(hSnap);
	}

	return false;
}