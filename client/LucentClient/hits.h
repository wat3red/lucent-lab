#pragma once

#include <vector>
#include <mutex>
#include <array>
#include <algorithm>
#include <winsock2.h>

#include "function.h"
#include "imgui/imgui.h"
#include "vector3.h"

#include "config/settings.h"
#include "config/imgui_config.h"

#include "config/imgui_config.h"


#pragma pack(push, 1)
struct HitsSettings {
	bool hitlogs;
};

struct HitLog {
	int androidHeight;
	int androidWidth;
	float distanceToLocalPlayer;
	Vector3 positionOnScreen;
	int32_t damage;
	char nickname[32];
	char boneName[32];
};
#pragma pack(pop)

// Добавьте этот enum перед структурой Notification
enum class NotificationType {
	Info,
	Success,
	Error
};

struct Notification {
	std::string text;
	std::chrono::steady_clock::time_point created;
	int durationMs;
	NotificationType type; // Добавили тип
	float width = 0.0f;    // Будем кешировать ширину для анимации
};

extern ImFont* cyrillicFont;

// Example client implementation
class Hits : public Function {
public:
	std::string GetName() {
		return oxorany("Visuals/Hits");
	}

	std::chrono::steady_clock::time_point lastUpdateTime;
	uint32_t GetId() const override { return 10; }
	size_t GetSettingsSize() const override { return sizeof(HitsSettings); }

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable hitlogs"), settings.hits.hitlogs);
		if (settings.hits.hitlogs) {
			ImGui::Indent();
			ImGuiEx::Checkbox(oxorany("Enable damage label"), settings.hits.damageLabel);
			ImGuiEx::ColorEdit4(oxorany("Color of damage label"), settings.hits.damageLabelColor);
			ImGuiEx::SliderFloat(oxorany("Size of damage label"), settings.hits.damageLabelSize, 8.f, 22.f);
			ImGui::Unindent();
		}
	}

	float GetScaleFromDistance(float distance, float minSize, float maxSize) {
		// Define distance range
		constexpr float minDistance = 5.0f;   // closest enemy (larger font)
		constexpr float maxDistance = 100.0f; // farthest enemy (smaller font)

		// Normalize and invert distance (closer = bigger)
		float t = 1.0f - std::clamp((distance - minDistance) / (maxDistance - minDistance), 0.0f, 1.0f);
		return minSize + t * (maxSize - minSize);
	}

	void DrawDamageLabel(ImDrawList* drawList, const HitLog& hit, float scaleX, float scaleY) {
		float size = GetScaleFromDistance(hit.distanceToLocalPlayer, 8.0f, settings.hits.damageLabelSize);

		// Calculate position above head
		ImVec2 textPos(
			hit.positionOnScreen.x * scaleX,
			hit.positionOnScreen.y * scaleY
		);

		std::string damageLabel = std::to_string(hit.damage);

		// Calculate text size for centering
		ImVec2 textSize = cyrillicFont->CalcTextSizeA(size, FLT_MAX, 0.0f, damageLabel.c_str());

		textPos.x -= textSize.x / 2.0f;  // Center text horizontally 
		textPos.y -= textSize.y;

		// Draw main text
		drawList->AddText(cyrillicFont, size, textPos, settings.hits.damageLabelColor.GetValue(), damageLabel.c_str());
	}

	std::vector<Notification> notifications;

	// Немного изменим AddNotification
	void AddNotification(const std::string& text, int durationMs = 5000, NotificationType type = NotificationType::Info) {
		Notification n;
		n.text = text;
		n.durationMs = durationMs;
		n.created = std::chrono::steady_clock::now();
		n.type = type;
		notifications.push_back(n);
	}

	void DrawNotifications() {
		ImDrawList* drawList = ImGui::GetBackgroundDrawList();
		const ImVec2 screenSize = ImGui::GetMainViewport()->Size;

		// --- Настройки внешнего вида ---
		constexpr float PADDING_X = 12.0f;             // Отступ по горизонтали
		constexpr float PADDING_Y = 12.0f;             // Отступ по вертикали
		constexpr float SPACING_Y = 8.0f;              // Расстояние между уведомлениями
		constexpr float FADE_DURATION = 300.0f;        // Длительность анимации появления/исчезновения (в мс)
		constexpr float PROGRESS_BAR_HEIGHT = 4.0f;    // Высота полосы прогресса

		// Позиция для первого уведомления (самого верхнего)
		ImVec2 currentPos = ImVec2(screenSize.x / 2, screenSize.y * 0.75f);

		// Удаляем устаревшие уведомления
		notifications.erase(std::remove_if(notifications.begin(), notifications.end(),
			[&](const Notification& n) {
				auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - n.created).count();
				return elapsed > n.durationMs;
			}), notifications.end());


		// Рисуем от старых к новым, чтобы новые появлялись сверху
		for (auto& notification : notifications) {
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - notification.created).count();
			float remaining = static_cast<float>(notification.durationMs) - elapsed;

			ImVec2 textSize = cyrillicFont->CalcTextSizeA(
				cyrillicFont->FontSize, FLT_MAX, 0.0f, notification.text.c_str()
			);

			float notificationWidth = textSize.x * 1.5f;

			// alpha
			float alpha = 1.0f;
			if (elapsed < FADE_DURATION) {
				// Fade-in
				alpha = elapsed / FADE_DURATION;
			}
			else if (remaining < FADE_DURATION) {
				// Fade-out
				alpha = remaining / FADE_DURATION;
			}
			alpha = std::clamp(alpha, 0.0f, 1.0f);

			// slide-in
			float slide_offset = 0.0f;
			if (elapsed < FADE_DURATION) {
				float t = elapsed / FADE_DURATION;
				slide_offset = (1.0f - (1.0f - t) * (1.0f - t)) * notificationWidth;
			}
			else {
				slide_offset = notificationWidth;
			}

			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4* colors = style.Colors;

			ImVec4 textColor = colors[ImGuiCol_Text]; // Белый текст
			ImVec4 bgColor = colors[ImGuiCol_WindowBg];   // Темно-серый фон
			ImVec4 progressBarColor = colors[ImGuiCol_Button]; // Синий для прогресса

			float notificationHeight = textSize.y + PADDING_Y * 2.0f + PROGRESS_BAR_HEIGHT;
			ImVec2 notificationPos = ImVec2(currentPos.x - slide_offset + notificationWidth * 0.5f, currentPos.y);
			ImVec2 notificationSize = ImVec2(notificationWidth, notificationHeight);

			// --- Отрисовка ---
			// 1. Фон
			drawList->AddRectFilled(
				notificationPos,
				ImVec2(notificationPos.x + notificationSize.x, notificationPos.y + notificationSize.y),
				ImGui::GetColorU32(ImVec4(bgColor.x, bgColor.y, bgColor.z, bgColor.w * alpha)),
				8.0f // Радиус скругления углов
			);

			// 2. Текст
			drawList->AddText(
				cyrillicFont,
				cyrillicFont->FontSize,
				ImVec2(notificationPos.x + PADDING_X, notificationPos.y + PADDING_Y),
				ImGui::GetColorU32(ImVec4(textColor.x, textColor.y, textColor.z, textColor.w * alpha)),
				notification.text.c_str(),
				nullptr, // end of text
				notificationWidth - PADDING_X * 2.0f // wrap width
			);

			// 3. Полоса прогресса
			float progress = 1.0f - (static_cast<float>(elapsed) / notification.durationMs);
			ImVec2 progressBarStart = ImVec2(notificationPos.x, notificationPos.y + notificationSize.y - PROGRESS_BAR_HEIGHT);
			ImVec2 progressBarEnd = ImVec2(notificationPos.x + notificationSize.x * progress, notificationPos.y + notificationSize.y);

			drawList->AddRectFilled(
				progressBarStart,
				progressBarEnd,
				ImGui::GetColorU32(ImVec4(progressBarColor.x, progressBarColor.y, progressBarColor.z, progressBarColor.w * alpha)),
				8.0f, // Радиус
				ImDrawFlags_RoundCornersBottom // Скругляем только нижние углы
			);

			// Смещаем позицию для следующего уведомления
			currentPos.y += notificationHeight + SPACING_Y;
		}
	}

	void RenderBackgroundUI() override {
		DrawNotifications();
	}

	void ApplyData(const void* data, size_t size) override {
		if (size < sizeof(uint32_t)) return;

		const uint8_t* ptr = static_cast<const uint8_t*>(data);
		uint32_t count;
		memcpy(&count, ptr, sizeof(uint32_t));
		ptr += sizeof(uint32_t);
		std::lock_guard<std::mutex> lock(dataMutex);

		hits.resize(count);

		if (count != 0) {
			memcpy(hits.data(), ptr, count * sizeof(HitLog));

			if (!settings.hits.hitlogs) return;

			for (const auto& hit : hits) {
				AddNotification(
					std::string("Hit ") + hit.nickname + " in " + hit.boneName +
					" dealing " + std::to_string(hit.damage) + " damage"
				);
			}
		}
	}

	bool UpdateSettings() override {
		settingsForSending.hitlogs = settings.hits.hitlogs;

		return UpdateSettingsBase(settingsForSending, previousSettings);
	}

	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(HitsSettings));
		previousSettings = settingsForSending;
	}

	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(HitsSettings));
	}

private:
	// Android->PC
	ImVec2 GetScreenScale(const HitLog& hit) {
		ImVec2 screenSize = ImGui::GetMainViewport()->Size;
		return ImVec2(screenSize.x / hit.androidWidth, screenSize.y / hit.androidHeight);
	}

	HitsSettings settingsForSending;
	HitsSettings previousSettings;
	std::mutex dataMutex;
	std::vector<HitLog> hits;
};