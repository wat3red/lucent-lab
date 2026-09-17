#pragma once
#include "function.h"
#include "imgui/imgui.h"
#include "config/settings.h"
#include "config/imgui_config.h"

#include <vector>
#include <winsock2.h>

#pragma pack(push, 1)
struct ChamsModelSettings {
	bool enable;
	float visibleEmissionColor[4];
	float invisibleEmissionColor[4];
	float outlineColor[4];
	float outlineWidth;
	int outlineVisibilityMode; // OnBothParts - 0, OnVisiblePart - 2, OnInvisiblePart - 5
};

struct ChamsSettings {
	bool enable;

	ChamsModelSettings playerArms;
	ChamsModelSettings playerGloves;
	ChamsModelSettings enemy;
	ChamsModelSettings ally;
};
#pragma pack(pop)

class Chams : public Function {
public:
	Chams() : previousSettings{ false } {}

	std::string GetName() {
		return oxorany("Visuals/Chams");
	}
	uint32_t GetId() const override { return 6; }
	size_t GetSettingsSize() const override { return sizeof(ChamsSettings); }

	void RenderModelSettings(Settings::ChamsModelConfig& s, const std::string& id) {
		ImGuiEx::Checkbox((oxorany("Enable##") + id).c_str(), s.enable);
		if (s.enable) {
			ImGuiEx::ColorEdit4((oxorany("Visible emission Color (A = Strength)##") + id).c_str(), s.visibleEmissionColor, true);
			ImGuiEx::ColorEdit4((oxorany("Invisible emission Color (A = Strength)##") + id).c_str(), s.invisibleEmissionColor, true);
			ImGuiEx::ColorEdit4((oxorany("Outline Color##") + id).c_str(), s.outlineColor, true);
			ImGuiEx::SliderFloat((oxorany("Outline Width##") + id).c_str(), s.outlineWidth, 0.0f, 0.1f);
			const char* modes[] = { oxorany("On both"), oxorany("On visible"), oxorany("On invisible") };
			ImGuiEx::Combo((oxorany("Outline visibility mode##") + id).c_str(), s.outlineVisibilityMode, modes, 3);
		}
	}

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable Chams"), settings.chams.enable);

		if (!settings.chams.enable)
			return;

		static int selectedPart = 0;
		const char* parts[] = {
			oxorany("Player Arms"),
			oxorany("Player Gloves"),
			oxorany("Enemy"),
			oxorany("Ally")
		};

		ImGui::Indent();

		// Выбор модели
		ImGui::Combo(oxorany("Model type"), &selectedPart, parts, IM_ARRAYSIZE(parts));

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Отрисовка настроек выбранной модели
		switch (selectedPart) {
		case 0:
			RenderModelSettings(settings.chams.playerArms, oxorany("playerArms"));
			break;
		case 1:
			RenderModelSettings(settings.chams.playerGloves, oxorany("playerGloves"));
			break;
		case 2:
			RenderModelSettings(settings.chams.enemy, oxorany("enemy"));
			break;
		case 3:
			RenderModelSettings(settings.chams.ally, oxorany("ally"));
			break;
		}

		ImGui::Unindent();
	}

	bool UpdateSettings() override {
		settingsForSending.enable = settings.chams.enable;

		auto& src = settings.chams;

		auto copyModel = [](const Settings::ChamsModelConfig& from, ChamsModelSettings& to) {
			to.enable = from.enable;
			to.outlineVisibilityMode = from.outlineVisibilityMode;
			to.outlineWidth = from.outlineWidth;

			ImVec4 visibleEmission = from.visibleEmissionColor;
			ImVec4 invisibleEmission = from.invisibleEmissionColor;
			ImVec4 outline = from.outlineColor;
			memcpy(to.visibleEmissionColor, &visibleEmission, sizeof(to.visibleEmissionColor));
			memcpy(to.invisibleEmissionColor, &invisibleEmission, sizeof(to.invisibleEmissionColor));
			memcpy(to.outlineColor, &outline, sizeof(to.outlineColor));
			};

		copyModel(src.playerArms, settingsForSending.playerArms);
		copyModel(src.playerGloves, settingsForSending.playerGloves);
		copyModel(src.enemy, settingsForSending.enemy);
		copyModel(src.ally, settingsForSending.ally);

		return UpdateSettingsBase(settingsForSending, previousSettings);
	}

	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(ChamsSettings));
		previousSettings = settingsForSending;
	}

	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(ChamsSettings));
	}

private:
	ChamsSettings settingsForSending;
	ChamsSettings previousSettings;
};