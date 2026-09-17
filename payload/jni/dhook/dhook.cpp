#include "dhook.h"
#include "../logger.h" // Your logger

#include <unordered_map>
#include <mutex>
#include <cstring> // For memcpy
#include <sys/mman.h>

// --- Globals ---
std::unordered_map<p_void, DContainer*> hookMap;
std::mutex hookMapMutex;

// --- Helper Functions ---
byte ldr_pc_pc_4_arm[4] = { 0x04, 0xF0, 0x1F, 0xE5 }; // LDR PC, [PC, #-4]

void copyBranchBytes(byte* bytes, p_void target) {
    memcpy(bytes, ldr_pc_pc_4_arm, 4);
    *reinterpret_cast<p_void*>(&bytes[4]) = target;
}

MEMPROTECT_STATUS setPageProtection(void* target, int protection) {
    p_void page_start = (p_void)((uintptr_t)target & -PAGE_SIZE);
    return (MEMPROTECT_STATUS)mprotect(page_start, PAGE_SIZE, protection);
}

void placeBytes(void* source, void* dest, int len) {
    memcpy(dest, source, len);
}

// --- Main Hooking Logic ---

p_void DHookTrampolineARM(p_void origin, p_void detour) {
    // 1. Allocate DContainer on the heap to store metadata
    DContainer* dhc = (DContainer*)malloc(sizeof(DContainer));
    if (!dhc) {
        LOGE("DERR: Failed to malloc DContainer");
        return NULL;
    }
    dhc->detourFunction = detour;
    LOGD("DDBG: Ready to Hook: %p -> %p", origin, detour);

    // 2. Allocate an executable page for the trampoline using mmap
    // The trampoline contains: [8 bytes of original code] + [8 bytes of jump-back code]
    dhc->trampoline = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (dhc->trampoline == MAP_FAILED) {
        LOGE("DERR: mmap failed for trampoline allocation, errno: %d", errno);
        free(dhc);
        return NULL;
    }
    
    // 3. Save original bytes and construct the trampoline
    placeBytes(origin, dhc->originalBytes, 8); // Save original bytes into our container
    placeBytes(dhc->originalBytes, dhc->trampoline, 8); // Place original bytes at the start of the trampoline

    p_void returnAddress = (p_void)((uintptr_t)origin + 8);
    byte jumpBackBytes[8];
    copyBranchBytes(jumpBackBytes, returnAddress);
    placeBytes(jumpBackBytes, (p_void)((uintptr_t)dhc->trampoline + 8), 8); // Place jump-back after original bytes

    LOGD("DDBG: Trampoline created at %p", dhc->trampoline);

    // 4. Create the branch from the original function to our detour
    copyBranchBytes(dhc->branchDetourBytes, detour);

    // 5. Make the origin page writable and place the hook
    if (setPageProtection(origin, PROT_READ | PROT_WRITE | PROT_EXEC) == MEMPROTECT_FAILED) {
        LOGE("DERR: mprotect failed on origin page");
        munmap(dhc->trampoline, PAGE_SIZE);
        free(dhc);
        return NULL;
    }
    placeBytes(dhc->branchDetourBytes, origin, 8);

    // 6. Clear CPU instruction cache to ensure our changes are seen
    __builtin___clear_cache((char*)origin, (char*)origin + 8);
    __builtin___clear_cache((char*)dhc->trampoline, (char*)dhc->trampoline + 16);

    LOGD("DDBG: Placed Detour");

    // 7. Add to hook map
    {
        std::lock_guard<std::mutex> lock(hookMapMutex);
        hookMap[origin] = dhc;
    }

    // The "original function pointer" is now the pointer to our executable trampoline
    return dhc->trampoline;
}

void DHook(p_void origin, p_void detour, p_void* trampoline) {
    if ((reinterpret_cast<uintptr_t>(origin) & 1) == 0) {
        *trampoline = DHookTrampolineARM(origin, detour);
    } else {
        LOGE("DERR: Thumb-2 hooking not implemented.");
        *trampoline = NULL;
    }
}

// --- Enable/Disable/Unhook Functions ---

void DDisableAllHooks() {
    std::lock_guard<std::mutex> lock(hookMapMutex);
    LOGD("DDBG: Disabling all hooks...");
    for (auto const& [origin, dhc] : hookMap) {
        setPageProtection(origin, PROT_READ | PROT_WRITE | PROT_EXEC);
        placeBytes(dhc->originalBytes, origin, 8);
        __builtin___clear_cache((char*)origin, (char*)origin + 8);
    }
    LOGD("DDBG: All hooks have been disabled.");
}

void DEnableAllHooks() {
    std::lock_guard<std::mutex> lock(hookMapMutex);
    LOGD("DDBG: Enabling all hooks...");
    for (auto const& [origin, dhc] : hookMap) {
        setPageProtection(origin, PROT_READ | PROT_WRITE | PROT_EXEC);
        placeBytes(dhc->branchDetourBytes, origin, 8);
        __builtin___clear_cache((char*)origin, (char*)origin + 8);
    }
    LOGD("DDBG: All hooks have been enabled.");
}

void DUnhook(p_void origin) {
    std::lock_guard<std::mutex> lock(hookMapMutex);
    auto it = hookMap.find(origin);
    if (it == hookMap.end()) return;

    DContainer* dhc = it->second;
    setPageProtection(origin, PROT_READ | PROT_WRITE | PROT_EXEC);
    placeBytes(dhc->originalBytes, origin, 8);
    __builtin___clear_cache((char*)origin, (char*)origin + 8);

    munmap(dhc->trampoline, PAGE_SIZE); // Free the mmap'd page
    free(dhc);                         // Free the container
    hookMap.erase(it);
    LOGD("DDBG: Unhooked and freed resources for %p", origin);
}

void DUnhookAll() {
    std::lock_guard<std::mutex> lock(hookMapMutex);
    if(hookMap.empty()) return;

    for (auto const& [origin, dhc] : hookMap) {
        setPageProtection(origin, PROT_READ | PROT_WRITE | PROT_EXEC);
        placeBytes(dhc->originalBytes, origin, 8);
         __builtin___clear_cache((char*)origin, (char*)origin + 8);
        munmap(dhc->trampoline, PAGE_SIZE);
        free(dhc);
    }
    hookMap.clear();
    LOGD("DDBG: All hooks have been permanently removed.");
}