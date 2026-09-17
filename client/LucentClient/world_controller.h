#pragma once
#include "function.h"
#include "imgui/imgui.h"
#include "color.h"

#include "config/settings.h"
#include "config/imgui_config.h"

#include <vector>
#include <winsock2.h>

#pragma pack(push, 1)
struct WorldControllerSettings {
	bool enableFog;
	float fogEndDistance;
	float fogStartDistance;
	Color fogColor;

	bool enableWorldColor;
	Color worldColor;

	bool enableSkyColor;
	Color skyColor;
};
#pragma pack(pop)

class WorldController : public Function {
public:
	std::string GetName() {
		return oxorany("Visuals/World");
	}
	uint32_t GetId() const override { return 7; }
	size_t GetSettingsSize() const override { return sizeof(WorldControllerSettings); }

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable world settings"), settings.worldController.enable);

		if (settings.worldController.enable) {
			ImGui::Indent();

			/*ImGuiEx::Checkbox(oxorany("Enable fog"), settings.worldController.enableFog);
			if (settings.worldController.enableFog) {
				ImGui::Indent();
				ImGuiEx::SliderFloat(oxorany("fogEndDistance"), settings.worldController.fogEndDistance, 0.1f, 150.f);
				ImGuiEx::SliderFloat(oxorany("fogStartDistance"), settings.worldController.fogStartDistance, 0.f, 150.f);
				ImGuiEx::ColorEdit4(oxorany("fogColor"), settings.worldController.fogColor);
				ImGui::Unindent();
			}*/

			ImGuiEx::Checkbox(oxorany("Enable world color"), settings.worldController.enableWorldColor);
			ImGuiEx::ColorEdit4(oxorany("worldColor"), settings.worldController.worldColor);

			ImGuiEx::Checkbox(oxorany("Enable sky color"), settings.worldController.enableSkyColor);
			ImGuiEx::ColorEdit4(oxorany("skyColor"), settings.worldController.skyColor);
			ImGui::Unindent();

		}
	}

	bool UpdateSettings() override {
		settingsForSending.enableFog = settings.worldController.enableFog;
		settingsForSending.fogEndDistance = settings.worldController.fogEndDistance;
		settingsForSending.fogStartDistance = settings.worldController.fogStartDistance;
		settingsForSending.fogColor.r = settings.worldController.fogColor.GetValue().x;
		settingsForSending.fogColor.g = settings.worldController.fogColor.GetValue().y;
		settingsForSending.fogColor.b = settings.worldController.fogColor.GetValue().z;
		settingsForSending.fogColor.a = settings.worldController.fogColor.GetValue().w;

		settingsForSending.enableWorldColor = settings.worldController.enableWorldColor;
		settingsForSending.worldColor.r = settings.worldController.worldColor.GetValue().x;
		settingsForSending.worldColor.g = settings.worldController.worldColor.GetValue().y;
		settingsForSending.worldColor.b = settings.worldController.worldColor.GetValue().z;
		settingsForSending.worldColor.a = settings.worldController.worldColor.GetValue().w;


		settingsForSending.enableSkyColor = settings.worldController.enableSkyColor;
		settingsForSending.skyColor.r = settings.worldController.skyColor.GetValue().x;
		settingsForSending.skyColor.g = settings.worldController.skyColor.GetValue().y;
		settingsForSending.skyColor.b = settings.worldController.skyColor.GetValue().z;
		settingsForSending.skyColor.a = settings.worldController.skyColor.GetValue().w;

		return UpdateSettingsBase(settingsForSending, previousSettings);
	}

	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(WorldControllerSettings));
		previousSettings = settingsForSending;
	}
	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(WorldControllerSettings));
	}
private:
	WorldControllerSettings settingsForSending;
	WorldControllerSettings previousSettings;
};