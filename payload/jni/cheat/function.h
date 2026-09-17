#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <netinet/in.h>
#include <cstdint>
#include <cstring>

#include "shared/protocol/function_packet.h"

class Function {
public:
	virtual ~Function() = default;
	virtual void Init() {}
	virtual void Update() {}

	virtual void GetCurrentSettings(void* out) {}
	virtual void GetData(std::vector<uint8_t>& out) {}
	virtual uint32_t GetId() const = 0;
	virtual size_t GetSettingsSize() const { return 0; }

	virtual void ApplyData(const void* data, size_t size) {}
	virtual void ApplySettings(const void* data) {}
};
