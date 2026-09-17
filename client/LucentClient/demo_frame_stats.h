#pragma once

#include "function.h"
#include "shared/protocol/function_ids.h"

#include "imgui/imgui.h"
#include <oxorany/oxorany_include.h>

#include <cstring>
#include <string>

#pragma pack(push, 1)
struct LabFrameStatsSettings {
	uint32_t enabled;
};

struct LabFrameStatsData {
	float dtMs;
	float fps;
	uint32_t frames;
};
#pragma pack(pop)

class LabFrameStats : public Function {
public:
	LabFrameStatsSettings settings{1};
	LabFrameStatsSettings prev{};
	LabFrameStatsData last{};

	std::string GetName() override { return oxorany("Misc/Frame Stats"); }
	uint32_t GetId() const override { return kFnFrameStats; }
	size_t GetSettingsSize() const override { return sizeof(LabFrameStatsSettings); }

	bool UpdateSettings() override { return UpdateSettingsBase(settings, prev); }

	void GetCurrentSettings(void* out) override {
		if (!out) return;
		std::memcpy(out, &settings, sizeof(settings));
	}

	void RenderUI() override {
		bool enabled = settings.enabled != 0;
		if (ImGui::Checkbox(oxorany("Enabled"), &enabled))
			settings.enabled = enabled ? 1u : 0u;
		ImGui::Text(oxorany("fps=%.1f  dt=%.2f ms  frames=%u"),
			last.fps, last.dtMs, last.frames);
	}

	void ApplyData(const void* data, size_t size) override {
		if (!data || size < sizeof(LabFrameStatsData)) return;
		std::memcpy(&last, data, sizeof(last));
	}
};
