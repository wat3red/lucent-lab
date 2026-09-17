#pragma once

#include <vector>
#include <mutex>
#include <array>
#include <algorithm>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "function.h"
#include "imgui/imgui.h"
#include "vector3.h"

#include "config/settings.h"
#include "config/imgui_config.h"


#pragma pack(push, 1)
enum class GrenadeType : uint8_t {
	HE = 91,
	Smoke = 92,
	Flashbang = 93,
	Molotov = 94,
	Incendiary = 95
};

struct GrenadeControllerSettings {
	bool enabled;
	bool enableGrenadePractice;

	int heDamage;
	bool heShowIndicator;

	int incendiaryDamage;
	bool incendiaryShowIndicator;

	bool disableFlashbang;
	bool flashbangShowIndicator;

	bool disableSmoke;
	bool smokeShowIndicator;
};

struct GrenadeData {
	Vector3 screenPosition;
	char name[64];
	GrenadeType type;

	int androidWidth;
	int androidHeight;

	float distanceToLocalPlayer;

	float remainingTime; // осталось до взрыва
	float totalTime;     // общее время детонации
};
#pragma pack(pop)

extern ImFont* weaponFontLeft;

const std::unordered_map<GrenadeType, char> grenadesIcons = {
	{ GrenadeType::HE, 'c' },
	{ GrenadeType::Smoke, 'd' },
	{ GrenadeType::Flashbang, 'e' },
	{ GrenadeType::Molotov, 'f' },
	{ GrenadeType::Incendiary, 'g' },
};

// Example client implementation
class GrenadeController : public Function {
public:
	std::string GetName() {
		return oxorany("Combat/Grenades");
	}

	std::chrono::steady_clock::time_point lastUpdateTime;
	uint32_t GetId() const override { return 8; }
	size_t GetSettingsSize() const override { return sizeof(GrenadeControllerSettings); }

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable grenade controller"), settings.grenadeController.enable);
		ImGuiEx::Checkbox(oxorany("Enable grenade practice"), settings.grenadeController.enableGrenadePractice, oxorany("The changes will appear after restarting the match."));

		if (!settings.grenadeController.enable) return;

		ImGui::Indent();

		static int selectedGrenade = 0;
		const char* grenadeNames[] = { "HE", "Smoke", "Flashbang", "Incendiary" };
		ImGui::Combo(oxorany("Grenade type"), &selectedGrenade, grenadeNames, IM_ARRAYSIZE(grenadeNames));

		auto renderIndicatorConfig = [](const char* name, Settings::GrenadeIndicatorConfig& cfg, auto& extraField) {
			if constexpr (std::is_same_v<std::decay_t<decltype(extraField)>, ConfigVar<int>>)
				ImGuiEx::SliderInt((std::string(name) + " Damage").c_str(), extraField, 0, 300);
			else if constexpr (std::is_same_v<std::decay_t<decltype(extraField)>, ConfigVar<bool>>)
				ImGuiEx::Checkbox((std::string(name) + " Disable effects").c_str(), extraField);

			ImGuiEx::Checkbox((std::string(name) + " enable indicator").c_str(), cfg.enable);
			if (cfg.enable) {
				ImGuiEx::Checkbox((std::string(name) + " show timer").c_str(), cfg.showTimer);
				ImGuiEx::ColorEdit4((std::string(name) + " timer color").c_str(), cfg.timerColor);
				const char* types[3] = { "None", "Icon", "Name" };
				ImGuiEx::Combo((std::string(name) + " identificator type").c_str(),
					cfg.identificatorType, types, IM_ARRAYSIZE(types));
				ImGuiEx::ColorEdit4((std::string(name) + " identificator color").c_str(), cfg.identificatorColor, true);
			}
			};

		switch (selectedGrenade) {
		case 0: // HE
			renderIndicatorConfig("HE", settings.grenadeController.heIndicator,
				settings.grenadeController.heDamage);
			break;
		case 1: // Smoke
			renderIndicatorConfig("Smoke", settings.grenadeController.smokeIndicator,
				settings.grenadeController.smokeDisableEffects);
			break;
		case 2: // Flashbang
			renderIndicatorConfig("Flashbang", settings.grenadeController.flashbangIndicator,
				settings.grenadeController.flashbangDisableEffects);
			break;
		case 3: // Incendiary
			renderIndicatorConfig("Incendiary", settings.grenadeController.incendiaryIndicator,
				settings.grenadeController.incendiaryDamage);
			break;
		}

		ImGui::Unindent();
	}

	void DrawCircleProgress(ImDrawList* drawList, ImVec2 center, float radius, float progress, ImColor color) {
		const int segments = 64;
		float start_angle = -IM_PI / 2; // сверху
		float end_angle = start_angle + progress * IM_PI * 2.0f;

		drawList->PathClear();
		drawList->PathArcTo(center, radius, start_angle, end_angle, segments);
		drawList->PathStroke(color, false, 4.0f);
	}

	float GetScaleFromDistance(float distance, float minSize, float maxSize) {
		// Define distance range
		constexpr float minDistance = 5.0f;   // closest enemy (larger font)
		constexpr float maxDistance = 100.0f; // farthest enemy (smaller font)

		// Normalize and invert distance (closer = bigger)
		float t = 1.0f - std::clamp((distance - minDistance) / (maxDistance - minDistance), 0.0f, 1.0f);
		return minSize + t * (maxSize - minSize);
	}

	void RenderBackgroundUI() override {
		if (!settings.grenadeController.enable) return;

		if (std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::steady_clock::now() - lastUpdateTime).count() > 1)
			return;

		std::lock_guard<std::mutex> lock(dataMutex);
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();

		for (const auto& grenade : grenades) {
			if (grenade.screenPosition.z <= 0) continue;

			// --- Определяем настройки по типу гранаты ---
			Settings::GrenadeIndicatorConfig* indicatorCfg = nullptr;
			switch (grenade.type) {
			case GrenadeType::HE:         indicatorCfg = &settings.grenadeController.heIndicator; break;
			case GrenadeType::Smoke:      indicatorCfg = &settings.grenadeController.smokeIndicator; break;
			case GrenadeType::Flashbang:  indicatorCfg = &settings.grenadeController.flashbangIndicator; break;
			case GrenadeType::Incendiary:
			case GrenadeType::Molotov:    indicatorCfg = &settings.grenadeController.incendiaryIndicator; break;
			default: continue;
			}

			if (!indicatorCfg || !indicatorCfg->enable) continue;

			// --- Позиция на экране ---
			ImVec2 screenScale = GetScreenScale(grenade);
			ImVec2 pos = {
				grenade.screenPosition.x * screenScale.x,
				grenade.screenPosition.y * screenScale.y
			};

			// --- Базовые параметры ---
			float size = GetScaleFromDistance(grenade.distanceToLocalPlayer, 10.f, 28.f);
			ImColor textColor = indicatorCfg->identificatorColor.GetValue();
			ImColor timerColor = indicatorCfg->timerColor.GetValue();

			// --- Индикатор (Icon / Name) ---
			std::string label;
			ImVec2 textSize{};
			if (indicatorCfg->identificatorType == 1) { // Icon
				const auto it = grenadesIcons.find(grenade.type);
				if (it != grenadesIcons.end()) {
					label = std::string(1, it->second);
					ImGui::PushFont(weaponFontRight);
					textSize = ImGui::CalcTextSize(label.c_str());
					ImGui::PopFont();
				}
			}
			else if (indicatorCfg->identificatorType == 2) { // Name
				label = grenade.name;
				textSize = ImGui::CalcTextSize(label.c_str());
			}

			// --- Размер текста ---
			ImVec2 boxMin = { pos.x - textSize.x / 2.f, pos.y - textSize.y / 2.f };
			ImVec2 boxMax = { pos.x + textSize.x / 2.f, pos.y + textSize.y / 2.f };

			const float radius = textSize.x * 2.f; // подбираем радиус под текст
			drawList->AddCircleFilled(pos, radius, IM_COL32(20, 20, 20, 180));
			drawList->AddCircle(pos, radius, IM_COL32(80, 80, 80, 220), 64, 2.0f);

			// Icon/Label
			ImVec2 textPos(pos.x - textSize.x * 0.5f, pos.y - textSize.y * 0.5f);
			if (!label.empty()) {
				if (indicatorCfg->identificatorType == 1) { // Icon
					ImGui::PushFont(weaponFontLeft);
					drawList->AddText(ImGui::GetFont(), size, textPos, textColor, label.c_str());
					ImGui::PopFont();
				}
				else if (indicatorCfg->identificatorType == 2) { // Name
					drawList->AddText(textPos, textColor, label.c_str());
				}
			}

			// Timer
			if (indicatorCfg->showTimer && grenade.totalTime > 0) {
				float progress = grenade.remainingTime / grenade.totalTime;
				DrawCircleProgress(drawList, pos, radius, progress, timerColor);
			}
		}
	}

	void ApplyData(const void* data, size_t size) override {
		if (size < sizeof(uint32_t)) return;

		const uint8_t* ptr = static_cast<const uint8_t*>(data);
		uint32_t count;
		memcpy(&count, ptr, sizeof(uint32_t));
		ptr += sizeof(uint32_t);

		std::lock_guard<std::mutex> lock(dataMutex);
		grenades.resize(count);

		memcpy(grenades.data(), ptr, count * sizeof(GrenadeData));

		lastUpdateTime = std::chrono::steady_clock::now();
	}

	bool UpdateSettings() override {
		settingsForSending.enabled = settings.grenadeController.enable;
		settingsForSending.enableGrenadePractice = settings.grenadeController.enableGrenadePractice;

		// HE
		settingsForSending.heDamage = settings.grenadeController.heDamage;
		settingsForSending.heShowIndicator = settings.grenadeController.heIndicator.enable;

		// Smoke
		settingsForSending.disableSmoke = settings.grenadeController.smokeDisableEffects;
		settingsForSending.smokeShowIndicator = settings.grenadeController.smokeIndicator.enable;

		// Flashbang
		settingsForSending.disableFlashbang = settings.grenadeController.flashbangDisableEffects;
		settingsForSending.flashbangShowIndicator = settings.grenadeController.flashbangIndicator.enable;

		// Incendiary
		settingsForSending.incendiaryDamage = settings.grenadeController.incendiaryDamage;
		settingsForSending.incendiaryShowIndicator = settings.grenadeController.incendiaryIndicator.enable;

		return UpdateSettingsBase(settingsForSending, previousSettings);
	}

	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(GrenadeControllerSettings));
		previousSettings = settingsForSending;
	}

	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(GrenadeControllerSettings));
	}

private:
	// Android->PC
	ImVec2 GetScreenScale(const GrenadeData& grenade) {
		ImVec2 screenSize = ImGui::GetMainViewport()->Size;
		return ImVec2(screenSize.x / grenade.androidWidth, screenSize.y / grenade.androidHeight);
	}

	GrenadeControllerSettings settingsForSending;
	GrenadeControllerSettings previousSettings;
	std::mutex dataMutex;
	std::vector<GrenadeData> grenades;
};