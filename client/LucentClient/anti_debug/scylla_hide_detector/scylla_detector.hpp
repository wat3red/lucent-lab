#pragma once

namespace scylla{
	bool is_attached();

    struct HookedFunction {
        const wchar_t* dllname;
        const char* name;
    };

    enum opcodes {
#ifdef WIN32
        JMP_OPCODE = 0xE9,
#else
        JMP_OPCODE = 0x90,
#endif   
    };

    constexpr HookedFunction hooked_syscalls[] = {
        {L"ntdll.dll","NtSetInformationThread"},
        {L"ntdll.dll","NtSetInformationProcess"},
        {L"ntdll.dll","NtQuerySystemInformation"},
        {L"ntdll.dll","NtQueryInformationProcess"},
        {L"ntdll.dll","NtQueryObject"},
        {L"ntdll.dll","NtYieldExecution"},
        {L"ntdll.dll","NtCreateThreadEx"},
        {L"ntdll.dll","NtSetDebugFilterState"},
        {L"ntdll.dll","NtClose"},
        {L"ntdll.dll","NtGetContextThread"},
        {L"ntdll.dll","NtSetContextThread"},
        {L"ntdll.dll","NtContinue"},
        {L"ntdll.dll","NtQueryPerformanceCounter"},
        {L"win32u.dll","NtUserBlockInput"},
        {L"win32u.dll","NtUserFindWindowEx"},
        {L"win32u.dll","NtUserBuildHwndList"},
        {L"win32u.dll","NtUserQueryWindow"},
        {L"win32u.dll","NtUserGetForegroundWindow"},
    };

    constexpr HookedFunction hooked_functions[] = {
        {L"ntdll.dll","KiUserExceptionDispatcher"},
        {L"KERNELBASE.dll","OutputDebugStringA"},
        {L"KERNELBASE.dll","GetTickCount"},
        {L"KERNELBASE.dll","GetTickCount64"},
        {L"KERNELBASE.dll","GetLocalTime"},
        {L"KERNELBASE.dll","GetSystemTime"},
    };
}
