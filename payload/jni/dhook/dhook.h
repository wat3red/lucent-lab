#pragma once

#include <cstdint>
#include <sys/mman.h> // Needed for mmap constants

#define PAGE_SIZE 4096

typedef void* p_void;
typedef unsigned char byte;

enum MEMPROTECT_STATUS {
    MEMPROTECT_OK = 0,
    MEMPROTECT_FAILED = -1
};

/**
 * @brief A container to hold all necessary information for a single hook.
 */
struct DContainer {
    // The bytes for the branch to the detour function.
    byte branchDetourBytes[8];
    
    // A copy of the original 8 bytes from the hooked function.
    byte originalBytes[8];

    // A pointer to a page-aligned, executable memory region holding the trampoline.
    p_void trampoline;

    // The address of the detour function.
    p_void detourFunction;
};

// Function prototypes
void DHook(p_void origin, p_void detour, p_void* trampoline);
void DUnhook(p_void origin);
void DDisableAllHooks();
void DEnableAllHooks();
void DUnhookAll();