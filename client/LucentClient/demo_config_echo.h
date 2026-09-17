#pragma once

#include "function.h"
#include "shared/protocol/function_ids.h"

#include "imgui/imgui.h"
#include <oxorany/oxorany_include.h>

#include <cstring>
#include <string>

#pragma pack(push, 1)
struct LabConfigEchoSettings {
	float value;
	int32_t mode;
	char label[32];
};

struct LabConfigEchoData {
	float value;
	int32_t mode;
	uint32_t applyCount;
	char label[32];
};
#pragma pack(pop)

class LabConfigEcho : public Function {
public:
	LabConfigEchoSettings settings{0.5f, 0, "echo"};
	LabConfigEchoSettings prev{};
	LabConfigEchoData last{};

	std::string GetName() override { return oxorany("Misc/Config Echo"); }
	uint32_t GetId() const override { return kFnConfigEcho; }
	size_t GetSettingsSize() const override { return sizeof(LabConfigEchoSettings); }

	bool UpdateSettings() override { return UpdateSettingsBase(settings, prev); }

	void GetCurrentSettings(void* out) override {
		if (!out) return;
		std::memcpy(out, &settings, sizeof(settings));
	}

	void RenderUI() override {
		ImGui::SliderFloat(oxorany("Value"), &settings.value, 0.f, 1.f);
		ImGui::SliderInt(oxorany("Mode"), &settings.mode, 0, 8);
		ImGui::InputText(oxorany("Label"), settings.label, sizeof(settings.label));
		ImGui::Text(oxorany("echo value=%.3f mode=%d applies=%u"),
			last.value, last.mode, last.applyCount);
		ImGui::Text(oxorany("echo label=%s"), last.label);
	}

	void ApplyData(const void* data, size_t size) override {
		if (!data || size < sizeof(LabConfigEchoData)) return;
		std::memcpy(&last, data, sizeof(last));
	}
};
