#pragma once

#include <Windows.h>
#include <string>

/**
 * @class AntiDebug
 * @brief A class to detect if the current process is being debugged.
 *
 * This class implements a variety of anti-debugging techniques documented by Check Point
 * in their oxorany("Anti-Debug Tricks") research. It is designed for Windows (x64) and aims
 * to provide multiple layers of detection.
 *
 * References:
 * - https://anti-debug.checkpoint.com/
 */
class AntiDebug {
public:
	/**
	 * @brief Runs all implemented anti-debugging checks.
	 * @return True if a debugger is detected, false otherwise.
	 */
	static bool IsDebugged(std::string& reason);
	
	static void Init();
private:
	static void Patch_DbgBreakPoint();
	static void Patch_DbgUiRemoteBreakin();
	static void InstallCodeIntegrityObservers();

	static DWORD WINAPI AntiDebugWatchdog(LPVOID);
	
	// These methods correspond to the categories on the Check Point website.
	// Category: Debug Flags
	// https://anti-debug.checkpoint.com/techniques/debug-flags.html
	static bool CheckDebugFlags();

	// Category: Object Handles
	// https://anti-debug.checkpoint.com/techniques/object-handles.html
	static bool CheckObjectHandles();

	// Category: Exceptions
	// https://anti-debug.checkpoint.com/techniques/exceptions.html
	static bool CheckExceptions();

	// Category: Timing
	// https://anti-debug.checkpoint.com/techniques/timing.html
	static bool CheckTiming();

	// Category: Process Memory
	// https://anti-debug.checkpoint.com/techniques/process-memory.html
	static bool CheckProcessMemory();

	// Category: Assembly
	// https://anti-debug.checkpoint.com/techniques/assembly.html
	static bool CheckAssembly();

	// Category: Assembly
	// https://anti-debug.checkpoint.com/techniques/assembly.html
	static bool CheckHardwareBreakpoint();

	// Category: Interactive
	// https://anti-debug.checkpoint.com/techniques/interactive.html
	static bool CheckInteractive();

	// Category: Misc
	// https://anti-debug.checkpoint.com/techniques/misc.html
	static bool CheckMisc();
};