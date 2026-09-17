#pragma once
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include "network_manager/network_manager.h"
#include "shared/protocol/function_packet.h"

extern NetworkManager g_NetworkManager;

class Function {
public:
	bool manualSend = false;

	Function() = default;
	virtual ~Function() {};
	virtual void Init() { return; }
	virtual void Update() {}
	virtual bool UpdateSettings() { return 0; }
	virtual void RenderUI() {}
	virtual void RenderBackgroundUI() {}
	virtual std::string GetName() { return ""; }

	template<typename T>
	bool UpdateSettingsBase(T& currentSettings, T& previousSettings) {
		bool changed = memcmp(&currentSettings, &previousSettings, sizeof(T)) != 0;
		if (changed) {
			previousSettings = currentSettings;
		}
		return changed;
	}

	virtual void GetCurrentSettings(void* out) {}
	virtual void GetData(std::vector<uint8_t>& out) {}
	virtual uint32_t GetId() const = 0;
	virtual size_t GetSettingsSize() const { return 0; }

	virtual void ApplyData(const void* data, size_t size) {}
	virtual void ApplySettings(const void* data) {}
};
