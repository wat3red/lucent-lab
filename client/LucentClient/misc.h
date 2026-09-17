#pragma once
#include "function.h"
#include "imgui/imgui.h"
#include "hotkey.h"

#include "config/settings.h"
#include "config/imgui_config.h"

#include <vector>
#include <winsock2.h>

#pragma pack(push, 1)
struct MiscSettings {
	bool disableKickForAFK;

	bool enableFov;
	float fovValue;

	bool enableArmsPos;
	Vector3 armsOffset;

	bool thirdPersonView;
	float verticalOffset;
	float value;

	bool enableAirJump;
	bool enableStrafes;
	float jumpMoveSpeed;

	bool moveBeforeTime;

	bool noclip;
	float horizontalSpeed;
	float verticalSpeed;

	bool enableAntiAim;
	int antiAimType; // 0: Static, 1: Jitter, 2: Spin
	Vector2 antiAimAngles; // Pitch & Yaw для статического режима
	float jitterRange;
	float spinSpeed;

	bool incognitoMode;

	bool ghostMode;
};
#pragma pack(pop)

class Misc : public Function {
public:
	Misc() : previousSettings{ false } {}
	std::string GetName() {
		return oxorany("Misc");
	}
	uint32_t GetId() const override { return 5; }
	size_t GetSettingsSize() const override { return sizeof(MiscSettings); }

	void RenderBackgroundUI() override {
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always); // позиция окна

		ImGui::Begin("##StatusWindow", nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_NoNav);

		ImGui::Text("Ghost mode: %s", settings.misc.ghostMode ? "ON" : "OFF");
		ImGui::End();
	}

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("FOV changer"), settings.misc.enableFov);
		if (settings.misc.enableFov) {
			ImGui::Indent();
			ImGuiEx::SliderFloat(oxorany("FOV value"), settings.misc.fovValue, 0.1f, 180.f);
			ImGui::Unindent();
		}

		ImGuiEx::Checkbox(oxorany("Disable kick for AFK"), settings.misc.disableKickForAFK);

		ImGuiEx::Checkbox(oxorany("Move before time"), settings.misc.moveBeforeTime);

		ImGuiEx::Checkbox(oxorany("Arms position changer"), settings.misc.enableArmsPos);
		if (settings.misc.enableArmsPos) {
			ImGui::Indent();
			ImGuiEx::SliderFloat3(oxorany("Offset"), settings.misc.armsOffset, -1.f, 1.f);
			ImGui::Unindent();
		}

		ImGuiEx::Checkbox(oxorany("Air jump"), settings.misc.enableAirJump);
		ImGuiEx::Checkbox(oxorany("Strafes"), settings.misc.enableStrafes);
		if (settings.misc.enableStrafes) {
			ImGui::Indent();
			ImGuiEx::SliderFloat(oxorany("Jump move speed"), settings.misc.jumpMoveSpeed, 0.f, 7.f);
			ImGui::Unindent();
		}

		ImGuiEx::Checkbox(oxorany("Third person view"), settings.misc.thirdPersonView);
		if (settings.misc.thirdPersonView) {
			ImGui::Indent();
			ImGuiEx::SliderFloat(oxorany("Horizontal offset"), settings.misc.value, 0.1f, 5.f);
			ImGuiEx::SliderFloat(oxorany("Vertical offset"), settings.misc.verticalOffset, 0.1f, 5.f);
			ImGui::Unindent();
		}

		ImGuiEx::Checkbox(oxorany("No clip"), settings.misc.noclip);
		if (settings.misc.noclip) {
			ImGui::Indent();
			ImGuiEx::SliderFloat(oxorany("Horizontal speed"), settings.misc.horizontalSpeed, 0.1f, 10.f);
			ImGuiEx::SliderFloat(oxorany("Vertical speed"), settings.misc.verticalSpeed, 0.1f, 10.f);
			ImGui::Unindent();
		}

		ImGuiEx::Checkbox(oxorany("Anti-aim"), settings.misc.enableAntiAim);
		if (settings.misc.enableAntiAim) {
			ImGui::Indent();

			const char* antiAimTypes[] = { oxorany("Static"), oxorany("Jitter"), oxorany("Spin") };
			ImGuiEx::Combo(oxorany("Type"), settings.misc.antiAimType, antiAimTypes, IM_ARRAYSIZE(antiAimTypes));

			ImGuiEx::SliderFloat2(oxorany("Angles"), settings.misc.antiAimAngles, -180.f, 180.f);
			if (settings.misc.antiAimType == 1)
				ImGuiEx::SliderFloat(oxorany("Jitter range"), settings.misc.jitterRange, 0.f, 100.f);
			if (settings.misc.antiAimType == 2)
				ImGuiEx::SliderFloat(oxorany("Spin speed"), settings.misc.spinSpeed, 0.f, 100.f);

			ImGui::Unindent();
		}

		ImGuiEx::Checkbox(oxorany("Incognito mode"), settings.misc.incognitoMode);

		ImGuiEx::Checkbox(oxorany("Ghost mode"), settings.misc.ghostMode);
		ImGui::Indent();
		ImGuiEx::Hotkey(oxorany("Ghost mode bind"), settings.misc.clumsyBind);
		ImGui::Unindent();
	}

	bool UpdateSettings() override {
		settingsForSending.enableFov = settings.misc.enableFov;
		settingsForSending.fovValue = settings.misc.fovValue;

		settingsForSending.enableArmsPos = settings.misc.enableArmsPos;
		settingsForSending.armsOffset = settings.misc.armsOffset;

		settingsForSending.disableKickForAFK = settings.misc.disableKickForAFK;

		settingsForSending.thirdPersonView = settings.misc.thirdPersonView;
		settingsForSending.value = settings.misc.value;
		settingsForSending.verticalOffset = settings.misc.verticalOffset;

		settingsForSending.enableAirJump = settings.misc.enableAirJump;
		settingsForSending.enableStrafes = settings.misc.enableStrafes;
		settingsForSending.jumpMoveSpeed = settings.misc.jumpMoveSpeed;

		settingsForSending.moveBeforeTime = settings.misc.moveBeforeTime;

		settingsForSending.noclip = settings.misc.noclip;
		settingsForSending.verticalSpeed = settings.misc.verticalSpeed;
		settingsForSending.horizontalSpeed = settings.misc.horizontalSpeed;

		settingsForSending.enableAntiAim = settings.misc.enableAntiAim;
		settingsForSending.antiAimAngles = settings.misc.antiAimAngles;
		settingsForSending.antiAimType = settings.misc.antiAimType;
		settingsForSending.jitterRange = settings.misc.jitterRange;
		settingsForSending.spinSpeed = settings.misc.spinSpeed;

		settingsForSending.incognitoMode = settings.misc.incognitoMode;

		settingsForSending.ghostMode = settings.misc.ghostMode;

		return UpdateSettingsBase(settingsForSending, previousSettings);
	}

	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(MiscSettings));
		previousSettings = settingsForSending;
	}
	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(MiscSettings));
	}
private:
	MiscSettings settingsForSending;
	MiscSettings previousSettings;
};