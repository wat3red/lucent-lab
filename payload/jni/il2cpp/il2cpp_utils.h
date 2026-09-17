#pragma once

#include "../il2cpp/il2cpp_structs.h"

#include "../sdk/functions.h"
#include "../logger.h"

namespace Il2CppUtils {
    extern std::unordered_map<std::string, Il2CppClass*> cachedKlasses;

    MethodInfo* FindMethod(Il2CppClass* klass, const char* methodName, int params = -1, std::initializer_list<std::string> paramTypeNames = { });

    Il2CppClass* FindKlass(const char* namespaceName, const char* className);

    void* ReplaceVirtualMethod(const char* namespaceName, const char* className, uint8_t oldSlot, uint8_t newSlot);
    void* ReplaceVirtualMethod(Il2CppClass* klass, uint8_t oldSlot, uint8_t newSlot);
    void* ReplaceVirtualMethod(const char* namespaceName, const char* className, uint8_t slot, void* hook);
    void* ReplaceVirtualMethod(Il2CppClass* klass, uint8_t slot, void* hook);

    int FindVirtualMethodSlot(Il2CppClass* klass, const char* methodName, int params);

    template <typename H, typename O>
    bool MethodHook(Il2CppClass* klass, const char* methodName, H hook, O orig = nullptr) {
        if (!klass) {
            LOGD("Class pointer is null");
            return false;
        }

        if (!methodName) {
            LOGD("Method name is null");
            return false;
        }

        LOGD("Attempting to hook method '%s'", methodName);

        if (!hook) {
            LOGD("Hook function is null");
            return false;
        }

        MethodInfo* method = FindMethod(klass, methodName);
        if (!method) {
            LOGD("Could not find method");
            return false;
        }

        // Create a pointer to the methodPointer member to modify it
        void*& methodPointer = method->methodPointer;
        if (!methodPointer) {
            LOGD("Method pointer is null");
            return false;
        }

        LOGD("Found method with %d parameters, offset: 0x%x ", static_cast<int>(method->parameters_count), (uintptr_t)method->methodPointer - g_LibUnityBase);

        if (orig) {
            *(void**)orig = methodPointer;

            // LOGD("Stored original method pointer");
        }

        methodPointer = (void*)hook;

        LOGD("Hook installed. New method pointer: %x", (uintptr_t)hook);

        return true;
    }
};
