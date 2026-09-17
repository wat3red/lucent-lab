#pragma once
#include "function.h"
#include "imgui/imgui.h"
#include <hotkey.h>

#include "config/settings.h"
#include "config/imgui_config.h"
#include "resources/themes.h"

#include <vector>
#include <winsock2.h>
#include <chrono>

class SettingsUI : public Function {
public:
	std::string GetName() override { return oxorany("Settings"); }
	uint32_t GetId() const override { return 9; }
	size_t GetSettingsSize() const override { return 0; }

	void RenderHotkeysTab() {
		auto& hk = settings.settingsUI.hotkeys;

		struct HotkeyItem { const char* name; ConfigVar<int>& key; };
		const HotkeyItem hotkeyList[] = {
			{ oxorany("Open menu"),			hk.openMenu },
			{ oxorany("Aimbot"),			hk.aim },
			{ oxorany("ESP"),				hk.esp },
			{ oxorany("Chams"),				hk.chams },
			{ oxorany("Gun controls"),		hk.gunControls },
			{ oxorany("FOV changer"),		hk.fov },
			{ oxorany("Arms position"),		hk.armsPos },
			{ oxorany("Third person view"),	hk.thirdPersonView },
			{ oxorany("Air jump"),			hk.airJump },
			{ oxorany("Strafes"),			hk.strafes },
			{ oxorany("Move before time"),	hk.moveBeforeTime },
			{ oxorany("Noclip"),			hk.noclip },
			{ oxorany("Anti-aim"),			hk.antiaim },
			{ oxorany("Ghost mode"),		hk.ghostMode }
		};

		ImGui::Text(oxorany("Assign hotkeys for each feature:"));
		ImGui::Separator();

		for (auto& [name, key] : hotkeyList)
			ImGuiEx::Hotkey(oxorany(name), key);
	}

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable background blur"), settings.settingsUI.enableBlur);
		ImGuiEx::SliderFloat(oxorany("Menu scale"), settings.settingsUI.menuScale, 0.f, 1.f);

		ImGui::Text(oxorany("Theme Browser"));
		ImGui::Separator();

		ImGui::BeginChild(oxorany("ThemeList"), ImVec2(0, 400), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
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

			ImGui::Separator();
			ImGui::PopID();
		}
		ImGui::EndChild();

		RenderHotkeysTab();
	}

	static void ProcessInput(WPARAM pressedKey) {
		auto& hk = settings.settingsUI.hotkeys;
		auto& misc = settings.misc;

		static auto lastToggle = std::chrono::steady_clock::now();
		auto now = std::chrono::steady_clock::now();
		if ((now - lastToggle) < std::chrono::milliseconds(200)) return;

		struct ToggleItem { ConfigVar<int>& key; ConfigVar<bool>& var; };
		const ToggleItem toggles[] = {
			{ hk.ghostMode, misc.ghostMode },
			{ hk.esp, settings.esp.enabled },
			{ hk.chams, settings.chams.enable },
			{ hk.aim, settings.aim.enabled },
			{ hk.gunControls, settings.gunControls.enable },
			{ hk.thirdPersonView, misc.thirdPersonView },
			{ hk.fov, misc.enableFov },
			{ hk.armsPos, misc.enableArmsPos },
			{ hk.airJump, misc.enableAirJump },
			{ hk.strafes, misc.enableStrafes },
			{ hk.moveBeforeTime, misc.moveBeforeTime },
			{ hk.noclip, misc.noclip },
			{ hk.antiaim, misc.enableAntiAim }
		};

		for (auto& [key, var] : toggles) {
			if (key == pressedKey) {
				var = !var;
				lastToggle = now;
				break;
			}
		}
	}
};
