#include "memory.h"

#include <oxorany/oxorany_include.h>

// Function to get module information
MODULEINFO GetModuleInfo(const char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(szModule);
	if (hModule == NULL)
		return modinfo;

	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

// Function to convert a hexadecimal string to a byte
UINT StrHexToByte(const std::string& str)
{
	UINT iOutNumber;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> iOutNumber;
	return iOutNumber;
}

// Function to split a string by a delimiter
std::vector<std::string> SplitString(const std::string& str, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delimiter))
	{
		tokens.push_back(token);
	}

	return tokens;
}

// Function to get the signature bytes from a string
std::vector<BYTE> GetSignatureBytes(const std::string& str)
{
	std::vector<std::string> strs = SplitString(str, ' ');
	std::vector<BYTE> bytes;

	for (const auto& strHex : strs)
	{
		if (strHex == oxorany("??") || strHex == oxorany("?"))
		{
			bytes.push_back('\?');
		}
		else
		{
			bytes.push_back(StrHexToByte(strHex));
		}
	}

	return bytes;
}

const char* strcasestr(const char* haystack, const char* needle) {
	if (!*needle) return haystack;
	for (; *haystack; ++haystack) {
		if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
			const char* h = haystack, * n = needle;
			for (; *h && *n; ++h, ++n) {
				if (tolower((unsigned char)*h) != tolower((unsigned char)*n)) {
					break;
				}
			}
			if (!*n) return haystack;
		}
	}
	return nullptr;
}

#include <chrono>
#include <iostream>
// Function to find a pattern in the memory
std::vector<uintptr_t> Memory::FindPattern(const char* moduleName, const char* signature)
{
	auto startTime = std::chrono::steady_clock::now(); // Start timer

	uintptr_t base = 0x0;
	uintptr_t size = 0xffffffff;

	if (!strcasestr(moduleName, oxorany(".exe"))) {
		MODULEINFO mInfo = GetModuleInfo(moduleName);
		base = (uintptr_t)mInfo.lpBaseOfDll;
		size = (uintptr_t)mInfo.SizeOfImage;
	}

	auto pattern = GetSignatureBytes(signature);
	uintptr_t patternLength = pattern.size();

	uintptr_t currentAddress = base;
	uintptr_t endAddress = base + size;

	std::vector<uintptr_t> output = { };

	while (currentAddress < endAddress)
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (VirtualQuery((LPCVOID)currentAddress, &mbi, sizeof(mbi)) == 0)
		{
			break;
		}

		if (mbi.State == MEM_COMMIT &&
			(mbi.Protect & (PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_READONLY | PAGE_READWRITE)) &&
			!(mbi.Protect & PAGE_GUARD))
		{
			uintptr_t regionBase = (uintptr_t)mbi.BaseAddress;
			uintptr_t regionSize = mbi.RegionSize;
			uintptr_t regionEnd = regionBase + regionSize;

			for (uintptr_t i = regionBase; i < regionEnd - patternLength; ++i)
			{
				bool found = true;
				for (uintptr_t j = 0; j < patternLength; ++j)
				{
					if (pattern[j] != '\?' && pattern[j] != *(BYTE*)(i + j))
					{
						found = false;
						break;
					}
				}
				if (found)
				{
					auto endTime = std::chrono::steady_clock::now(); // Stop timer
					std::chrono::duration<double> elapsedTime = endTime - startTime;
					std::cout << oxorany("Search completed in ") << elapsedTime.count() << oxorany(" seconds") << std::endl;
					output.push_back(i);
				}
			}
		}

		// Move to the next region
		currentAddress += mbi.RegionSize;
	}

	return { };
}