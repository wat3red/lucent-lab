#pragma once

#include "function.h"
#include "imgui/imgui.h"
#include <oxorany/oxorany_include.h>

#include "config/settings.h"
#include "config/imgui_config.h"
#include "resources/themes.h"

#include <string>

// Portfolio lab settings: theme + menu only (no competitive hotkeys).
class LabSettingsUI : public Function {
public:
	std::string GetName() override { return oxorany("Settings"); }
	uint32_t GetId() const override { return 9; }
	size_t GetSettingsSize() const override { return 0; }

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable background blur"), settings.settingsUI.enableBlur);
		ImGuiEx::SliderFloat(oxorany("Menu scale"), settings.settingsUI.menuScale, 0.f, 1.f);

		ImGui::Text(oxorany("Theme Browser"));
		ImGui::Separator();

		ImGui::BeginChild(oxorany("ThemeList"), ImVec2(0, 320), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
		for (int i = 0; i < IM_ARRAYSIZE(allThemes); ++i) {
			MenuTheme& theme = allThemes[i];
			ImGui::PushID(i);

			ImGui::Text(oxorany("%s"), theme.name);

			constexpr float SIZE = 50.f;
			ImVec2 btnSize = { SIZE, SIZE / 2 };

			ImGui::ColorButton(oxorany("##bgDark"), theme.bgDark, ImGuiColorEditFlags_NoTooltip, btnSize);
			ImGui::SameLine();
			ImGui::ColorButton(oxorany("##bgMed"), theme.bgMed, ImGuiColorEditFlags_NoTooltip, { btnSize.x * 0.65f, btnSize.y });
			ImGui::SameLine();
			ImGui::ColorButton(oxorany("##accent"), theme.accent, ImGuiColorEditFlags_NoTooltip, { btnSize.x * 0.5f, btnSize.y });
			ImGui::SameLine();
			ImGui::ColorButton(oxorany("##text"), theme.text, ImGuiColorEditFlags_NoTooltip, { btnSize.x * 0.5f, btnSize.y });
			ImGui::SameLine();

			if (settings.settingsUI.theme == i)
				ImGui::Text(oxorany("Current theme"));
			else if (ImGui::Button(oxorany("Set Theme"))) {
				settings.settingsUI.theme = i;
				LoadImGuiStyle(theme);
			}

			ImGui::PopID();
		}
		ImGui::EndChild();

		ImGui::Text(oxorany("Lab build: competitive modules are not registered."));
	}
};
