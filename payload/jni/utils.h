#pragma once

#include "il2cpp/il2cpp_structs.h"

#include "sdk/functions.h"
#include "logger.h"

#include <thread>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <unistd.h>
#include <vector>

uintptr_t GetInstanceFromTypeInfo(uintptr_t typeInfoStruct);
uintptr_t GetInstanceFromTypeInfo(const char* namespaceName, const char* className);
uintptr_t GetModuleBase(const std::string& moduleName);
size_t GetModuleSize(const std::string& moduleName);

void* FindPattern(const uint8_t* base, size_t size, const std::string& pattern, bool isXref = false);
