#pragma once

#include <Windows.h>
#include <sstream>
#include <Psapi.h>
#include <cstring>
#include <vector>

namespace Memory
{
	std::vector<uintptr_t> FindPattern(const char* moduleName, const char* signature);
}