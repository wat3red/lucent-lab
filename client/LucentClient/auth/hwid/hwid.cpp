#include "hwid.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <windows.h>
#include <iphlpapi.h>
#include <wincrypt.h>
#include "base64.hpp"
#include "../../oxorany/oxorany_include.h"

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "crypt32.lib")

std::string CalculateHash(const std::string& input) {
	std::hash<std::string> hasher;
	size_t hash = hasher(input);

	std::stringstream ss;
	ss << std::hex << hash;
	return ss.str();
}

std::string GetHDDSerial() {
	char volumeName[MAX_PATH + 1] = { 0 };
	DWORD serialNumber = 0;
	DWORD maxComponentLength = 0;
	DWORD fileSystemFlags = 0;
	char fileSystemName[MAX_PATH + 1] = { 0 };

	if (!GetVolumeInformationA(
		oxorany("C:\\"),
		volumeName,
		sizeof(volumeName),
		&serialNumber,
		&maxComponentLength,
		&fileSystemFlags,
		fileSystemName,
		sizeof(fileSystemName)
	)) {
		//throw std::runtime_error(oxorany("Failed to retrieve HDD serial number."));
	}

	return std::to_string(serialNumber);
}

std::string GetMac() {
	ULONG bufferSize = 0;
	DWORD result = GetAdaptersInfo(nullptr, &bufferSize);
	if (result != ERROR_BUFFER_OVERFLOW) {
		//throw std::runtime_error(oxorany("Failed to determine buffer size for adapter info."));
	}

	std::vector<char> buffer(bufferSize);
	PIP_ADAPTER_INFO pAdapterInfo = reinterpret_cast<PIP_ADAPTER_INFO>(buffer.data());

	result = GetAdaptersInfo(pAdapterInfo, &bufferSize);
	if (result != ERROR_SUCCESS) {
		//throw std::runtime_error(oxorany("Failed to retrieve adapter information."));
	}

	while (pAdapterInfo) {
		if (pAdapterInfo->AddressLength == 6) {
			std::string mac = oxorany("");
			char sbuf[4];
			for (int i = 0; i < 6; ++i) {
				sprintf_s(sbuf, sizeof(sbuf), oxorany("%02X"), pAdapterInfo->Address[i]);
				mac += sbuf;
				if (i != 5) {
					mac += oxorany("-");
				}
			}
			return mac;
		}
		pAdapterInfo = pAdapterInfo->Next;
	}

	//throw std::runtime_error(obfuscate(oxorany("No suitable network adapter found.")));
}

std::string ComputeSHA256(const std::string& data) {
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	BYTE hash[32];
	DWORD hashLen = 32;

	if (!CryptAcquireContext(&hProv, nullptr, nullptr, PROV_RSA_AES, CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
		//throw std::runtime_error(obfuscate(oxorany("CryptAcquireContext failed.")));
	}

	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
		CryptReleaseContext(hProv, 0);
		//throw std::runtime_error(obfuscate(oxorany("CryptCreateHash failed.")));
	}

	if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(data.c_str()), static_cast<DWORD>(data.size()), 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		//throw std::runtime_error(obfuscate(oxorany("CryptHashData failed.")));
	}

	if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		//throw std::runtime_error(obfuscate(oxorany("CryptGetHashParam failed.")));
	}

	std::string hashHex;
	hashHex.reserve(hashLen * 2);
	const char* hexDigits = oxorany("0123456789ABCDEF");
	for (DWORD i = 0; i < hashLen; ++i) {
		hashHex += hexDigits[(hash[i] >> 4) & 0xF];
		hashHex += hexDigits[hash[i] & 0xF];
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);

	return hashHex;
}

std::string GetHardwareID() {
	try {
		std::string hddSerial = GetHDDSerial();
		std::string macAddress = GetMac();
		std::string combined = hddSerial + oxorany(":") + macAddress;

		auto sha256 = ComputeSHA256(combined);
		auto base64 = base64::to_base64(sha256);
		auto hash = CalculateHash(base64);

		return hash;
	}
	catch (const std::exception& ex) {
		std::cerr << oxorany("Error generating Hardware ID: ") << ex.what() << std::endl;
		return oxorany("");
	}
}