#pragma once

#include "function.h"
#include "shared/protocol/function_ids.h"

#include "imgui/imgui.h"
#include <oxorany/oxorany_include.h>

#include <cstring>
#include <string>

#pragma pack(push, 1)
struct LabPingSettings {
	uint32_t intervalMs;
	uint32_t enabled;
};

struct LabPingData {
	uint64_t tick;
	uint32_t uptimeMs;
	uint32_t version;
};
#pragma pack(pop)

class LabPing : public Function {
public:
	LabPingSettings settings{1000, 1};
	LabPingSettings prev{};
	LabPingData last{};

	std::string GetName() override { return oxorany("Misc/Lab Ping"); }
	uint32_t GetId() const override { return kFnPing; }
	size_t GetSettingsSize() const override { return sizeof(LabPingSettings); }

	bool UpdateSettings() override { return UpdateSettingsBase(settings, prev); }

	void GetCurrentSettings(void* out) override {
		if (!out) return;
		std::memcpy(out, &settings, sizeof(settings));
	}

	void RenderUI() override {
		bool enabled = settings.enabled != 0;
		if (ImGui::Checkbox(oxorany("Enabled"), &enabled))
			settings.enabled = enabled ? 1u : 0u;
		int interval = static_cast<int>(settings.intervalMs);
		if (ImGui::SliderInt(oxorany("Interval ms"), &interval, 50, 5000))
			settings.intervalMs = static_cast<uint32_t>(interval);
		ImGui::Text(oxorany("tick=%llu  uptime=%u ms  proto=v%u"),
			static_cast<unsigned long long>(last.tick), last.uptimeMs, last.version);
	}

	void ApplyData(const void* data, size_t size) override {
		if (!data || size < sizeof(LabPingData)) return;
		std::memcpy(&last, data, sizeof(last));
	}
};
