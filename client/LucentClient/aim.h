#pragma once

#include <vector>
#include <mutex>
#include <array>
#include <algorithm>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "function.h"
#include "imgui/imgui.h"
#include "vector2.h"

#include "config/settings.h"
#include "config/imgui_config.h"


enum class TargetSelectionMode : int {
	FOV = 0,
	Distance = 1,
	LowestHealth = 2
};

enum class HitboxSelectionMode : int {
	Priority = 0,
	Closest = 1,
	HighestDamage = 2
};

enum class VisibilityCheckMode : int {
	Multipoint = 0,
	CenterOnly = 1
};

enum class PreferredBone : int {
	Head = 0,
	Body = 1,
	Limbs = 2
};

static const int MAX_DEBUG_POINTS = 128;
#pragma pack(push, 1)
struct AimSettings {
	bool enabled;
	bool disableVisibilityCheck;
	bool autoFire;
	bool drawMarker;
	float fov;

	TargetSelectionMode targetSelectionMode;
	HitboxSelectionMode hitboxSelectionMode;
	VisibilityCheckMode visibilityCheckMode;

	PreferredBone preferredBone;
};
struct EmulatorData {
	int screenWidth;
	int screenHeight;
	int debugPointCount;
	Vector2 currentTarget;
	Vector2 debugPoints[MAX_DEBUG_POINTS];
};
#pragma pack(pop)

// Example client implementation
class Aim : public Function {
public:
	std::chrono::steady_clock::time_point lastUpdateTime;

	std::string GetName() {
		return oxorany("Combat/Aim");
	}
	uint32_t GetId() const override { return 3; }
	size_t GetSettingsSize() const override { return sizeof(AimSettings); }

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable silent aim"), settings.aim.enabled);
		if (settings.aim.enabled) {
			ImGui::Indent();

			// --- Основные настройки ---
			ImGuiEx::Checkbox(oxorany("Auto fire"), settings.aim.autoFire);
			
			ImGuiEx::Checkbox(oxorany("Draw marker"), settings.aim.drawMarker);
			if (settings.aim.drawMarker) {
				ImGui::Indent();
				ImGuiEx::SliderFloat(oxorany("Marker size"), settings.aim.markerSize, 0.1f, 30.f);
				ImGui::Unindent();
			}

			ImGuiEx::Checkbox(oxorany("Disable visibility check"), settings.aim.disableVisibilityCheck, oxorany("Recommended to use with WallShot."));
			ImGuiEx::SliderFloat(oxorany("FOV"), settings.aim.fov, 0.1f, ImGui::GetMainViewport()->Size.x / 2);
			ImGuiEx::SliderFloat(oxorany("FOV circle thickness"), settings.aim.limiterThickness, 1.0f, 10.0f);

			// --- Настройки выбора цели ---
			const char* targetModes[] = { oxorany("Closest to FOV"), oxorany("Closest by distance"), oxorany("Lowest health") };
			ImGuiEx::Combo(oxorany("Target selection"), settings.aim.targetSelectionMode, targetModes, IM_ARRAYSIZE(targetModes));

			const char* hitboxModes[] = { oxorany("Priority group"), oxorany("Closest to FOV"), oxorany("Highest damage") };
			ImGuiEx::Combo(oxorany("Hitbox selection"), settings.aim.hitboxSelectionMode, hitboxModes, IM_ARRAYSIZE(hitboxModes));

			// Настройка приоритетной кости, если выбран режим Priority
			if (settings.aim.hitboxSelectionMode.GetValue() == 0) {
				const char* bones[] = { oxorany("Head"), oxorany("Body"), oxorany("Limbs") };
				ImGuiEx::Combo(oxorany("Preferred bone group"), settings.aim.preferredBone, bones, IM_ARRAYSIZE(bones));
			}

			// --- Настройки производительности и точности ---
			/*if (ImGui::CollapsingHeader(oxorany("Performance & Accuracy"))) {
				const char* visModes[] = { oxorany("Multipoint"), oxorany("Center Only") };
				ImGuiEx::Combo(oxorany("Visibility Check"), settings.aim.visibilityCheckMode, visModes, IM_ARRAYSIZE(visModes));
			}*/

			ImGui::Unindent();
		}
	}


	void RenderBackgroundUI() override {
		if (!settings.aim.enabled) return;

		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		ImVec2 screenSize = ImGui::GetMainViewport()->Size;

		// fov
		drawList->AddCircle(ImVec2(screenSize.x / 2, screenSize.y / 2), settings.aim.fov, settings.aim.fovLimiterColor.GetValue(), 0, settings.aim.limiterThickness);

		// currnet target(marker)
		if (settings.aim.drawMarker) {
			const ImVec2 scale = GetEmulatorToPCScale(emulatorData);
			const ImVec2 targetCenter = ImVec2(scale.x * emulatorData.currentTarget.x, scale.y * emulatorData.currentTarget.y);
			const float halfSize = settings.aim.markerSize / 2;
			/*drawList->AddCircle(targetCenter,
				20, settings.aim.fovLimiterColor.GetValue(), 0, settings.aim.limiterThickness);*/

			drawList->AddLine(ImVec2(targetCenter.x - halfSize, targetCenter.y - halfSize), ImVec2(targetCenter.x + halfSize, targetCenter.y + halfSize), settings.aim.markerColor.GetValue(), 2);
			drawList->AddLine(ImVec2(targetCenter.x + halfSize, targetCenter.y - halfSize), ImVec2(targetCenter.x - halfSize, targetCenter.y + halfSize), settings.aim.markerColor.GetValue(), 2);
		}

		// debug points view
		//for (int i = 0; i < emulatorData.debugPointCount; ++i) {
		//	ImVec2 pos = ImVec2(
		//		GetEmulatorToPCScale(emulatorData).x * emulatorData.debugPoints[i].x,
		//		GetEmulatorToPCScale(emulatorData).y * emulatorData.debugPoints[i].y
		//	);
		//	drawList->AddCircleFilled(pos, 2.0f, IM_COL32(255, 255, 0, 180)); // жёлтая точка
		//}
	}

	void ApplyData(const void* data, size_t size) override {
		if (size < sizeof(EmulatorData)) return;

		std::lock_guard<std::mutex> lock(dataMutex);
		memcpy(&emulatorData, data, sizeof(EmulatorData));
	}

	bool UpdateSettings() override {
		settingsForSending.enabled = settings.aim.enabled;
		settingsForSending.autoFire = settings.aim.autoFire;
		settingsForSending.disableVisibilityCheck = settings.aim.disableVisibilityCheck;
		settingsForSending.drawMarker = settings.aim.drawMarker;

		settingsForSending.fov = settings.aim.fov * GetPCToEmulatorScale(emulatorData).x;

		// Передача новых настроек
		settingsForSending.targetSelectionMode = (TargetSelectionMode)settings.aim.targetSelectionMode.GetValue();
		settingsForSending.hitboxSelectionMode = (HitboxSelectionMode)settings.aim.hitboxSelectionMode.GetValue();
		settingsForSending.visibilityCheckMode = (VisibilityCheckMode)settings.aim.visibilityCheckMode.GetValue();
		settingsForSending.preferredBone = (PreferredBone)settings.aim.preferredBone.GetValue();

		return UpdateSettingsBase(settingsForSending, previousSettings);
	}


	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(AimSettings));
		previousSettings = settingsForSending;
	}

	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(AimSettings));
	}

private:
	// Emulator->PC
	ImVec2 GetEmulatorToPCScale(const EmulatorData& emulatorData) {
		ImVec2 screenSize = ImGui::GetMainViewport()->Size;
		return ImVec2(screenSize.x / emulatorData.screenWidth, screenSize.y / emulatorData.screenHeight);
	}

	// PC->Emulator
	ImVec2 GetPCToEmulatorScale(const EmulatorData& emulatorData) {
		ImVec2 screenSize = ImGui::GetMainViewport()->Size;
		return ImVec2(emulatorData.screenWidth / screenSize.x, emulatorData.screenHeight / screenSize.y);
	}

	AimSettings settingsForSending;
	AimSettings previousSettings;

	std::mutex dataMutex;
	EmulatorData emulatorData;
};