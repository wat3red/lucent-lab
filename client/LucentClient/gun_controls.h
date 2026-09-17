#pragma once
#include "function.h"
#include "imgui/imgui.h"

#include "config/settings.h"
#include "config/imgui_config.h"

#include <vector>
#include <winsock2.h>

#pragma pack(push, 1)
struct GunControlsSettings {
	bool enable;

	bool rapidFire;
	bool infinityAmmo;
	bool disableRecoil;
	bool accurateShooting;
	bool damageHack;
	bool wallShot;
};
#pragma pack(pop)

class GunControls : public Function {
public:
	GunControls() : previousSettings{ false } {}
	std::string GetName() {
		return oxorany("Combat/Guns");
	}
	uint32_t GetId() const override { return 1; }
	size_t GetSettingsSize() const override { return sizeof(GunControlsSettings); }

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable"), settings.gunControls.enable);
		if (settings.gunControls.enable) {
			ImGui::Indent();
			ImGuiEx::Checkbox(oxorany("Accurate shooting"), settings.gunControls.accurateShooting);
			ImGuiEx::Checkbox(oxorany("Damage hack"), settings.gunControls.damageHack);
			ImGuiEx::Checkbox(oxorany("No recoil"), settings.gunControls.disableRecoil);
			ImGuiEx::Checkbox(oxorany("Infinity ammo"), settings.gunControls.infinityAmmo);
			ImGuiEx::Checkbox(oxorany("Rapid fire"), settings.gunControls.rapidFire);
			ImGuiEx::Checkbox(oxorany("WallShot"), settings.gunControls.wallShot);
			ImGui::Unindent();
		}
	}

	bool UpdateSettings() override {
		settingsForSending.enable = settings.gunControls.enable;

		settingsForSending.rapidFire = settings.gunControls.rapidFire;
		settingsForSending.infinityAmmo = settings.gunControls.infinityAmmo;
		settingsForSending.disableRecoil = settings.gunControls.disableRecoil;
		settingsForSending.accurateShooting = settings.gunControls.accurateShooting;
		settingsForSending.damageHack = settings.gunControls.damageHack;
		settingsForSending.wallShot = settings.gunControls.wallShot;

		return UpdateSettingsBase(settingsForSending, previousSettings);
	}

	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(GunControlsSettings));
		previousSettings = settingsForSending;
	}
	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(GunControlsSettings));
	}
private:
	GunControlsSettings settingsForSending;
	GunControlsSettings previousSettings;
};