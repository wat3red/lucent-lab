#pragma once
#include "function.h"
#include "network_manager/network_manager.h"
#include "imgui/imgui_internal.h"
#include "utils/easing_functions.h"

#include <vector>
#include <time.h>

#include <winsock2.h>
#include <thread>
#include <mutex>
#include <ws2tcpip.h>
#include <algorithm>

extern NetworkManager g_NetworkManager;
extern ImFont* labelFont;
extern ImFont* mainFont;
extern ImFont* glyphsFont;
extern ImFont* headerFont;

class FunctionsManager {
	std::vector<Function*> functions;

public:
	void AddFunction(Function* func) {
		functions.push_back(func);
	}

	void RenderAllBackgroundUI() {
		for (auto* func : functions) func->RenderBackgroundUI();
	}

	void InitAllFunctions() {
		for (auto* func : functions) func->Init();
	}

	static inline ImVec2 g_CardStart;
	void BeginCard() {
		g_CardStart = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->ChannelsSplit(2);
		ImGui::GetWindowDrawList()->ChannelsSetCurrent(1); // Content goes to the foreground channel

		ImVec2 padding = ImGui::GetStyle().WindowPadding;
		ImGui::Dummy(ImVec2(0.0f, padding.y)); // Top padding
		ImGui::Indent(padding.x);             // Left padding (also creates right padding by reducing width)
	}

	void EndCard(Function* func) {
		static std::unordered_map<Function*, float> hoverAnim;
		ImVec2 padding = ImGui::GetStyle().WindowPadding;

		ImGui::Unindent(padding.x);            // Remove horizontal padding
		ImGui::Dummy(ImVec2(0.0f, padding.y));  // Bottom padding

		ImVec2 pMin = g_CardStart;
		// After un-indenting, GetContentRegionAvail().x is the full column width again
		ImVec2 pMax = ImVec2(g_CardStart.x + ImGui::GetContentRegionAvail().x, ImGui::GetCursorScreenPos().y);

		// Place the invisible button over the entire card area for hover detection
		ImGui::SetCursorScreenPos(pMin);
		ImGui::InvisibleButton(("##hover_" + std::to_string((uintptr_t)func)).c_str(),
			ImVec2(pMax.x - pMin.x, pMax.y - pMin.y));

		bool hovered = ImGui::IsItemHovered();

		float& t = hoverAnim[func];
		float dt = ImGui::GetIO().DeltaTime;
		if (hovered)
			t = min(1.0f, t + dt * 6.0f);
		else
			t = max(0.0f, t - dt * 6.0f);

		float alphaMul = easeOutQuint(t);

		ImVec4 color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
		color.x = min(color.x + 0.06f + 0.03f * alphaMul, 1.0f);
		color.y = min(color.y + 0.06f + 0.03f * alphaMul, 1.0f);
		color.z = min(color.z + 0.06f + 0.03f * alphaMul, 1.0f);
		color.w = ImGui::GetStyle().Alpha;

		// Draw background in the background channel
		ImGui::GetWindowDrawList()->ChannelsSetCurrent(0);
		ImGui::GetWindowDrawList()->AddRectFilled(pMin, pMax, ImGui::ColorConvertFloat4ToU32(color), ImGui::GetStyle().WindowRounding);
		ImGui::GetWindowDrawList()->ChannelsMerge();

		// Manually position cursor at the bottom of the card for the next element
		ImGui::SetCursorScreenPos(ImVec2(g_CardStart.x, pMax.y));

		// Add final spacing between this card and the next
		ImGui::Dummy(ImVec2(0.0f, ImGui::GetStyle().ItemSpacing.y));
	}

	void RenderCategoryContent(const char* currentCategory) {
		std::unordered_map<std::string, Function*> grouped;
		std::vector<Function*> fullTabFuncs;

		// Группируем функции по категориям (Grouping functions by category)
		for (Function* func : functions) {
			if (!func || func->GetName().empty()) continue;
			std::string name(func->GetName());
			size_t slash = name.find('/');

			if (slash == std::string::npos) {
				if (name == currentCategory) {
					fullTabFuncs.push_back(func);
				}
			}
			else {
				std::string category = name.substr(0, slash);
				if (category == currentCategory) {
					std::string subname = name.substr(slash + 1);
					grouped[subname] = func;
				}
			}
		}

		if (!fullTabFuncs.empty()) {
			// Для полноразмерных вкладок просто рендерим контент (For full-sized tabs, just render content)
			for (Function* f : fullTabFuncs) {
				f->RenderUI();
			}
		}
		else {
			// 1. Разделим все группы на два списка: для левой и правой колонки
			// (Divide all groups into two lists: for the left and right columns)
			using FuncGroup = std::pair<const std::string, Function*>;
			std::vector<FuncGroup> leftColumnGroups;
			std::vector<FuncGroup> rightColumnGroups;

			bool assignToLeftColumn = true;
			for (const auto& groupPair : grouped) {
				if (assignToLeftColumn) {
					leftColumnGroups.push_back(groupPair);
				}
				else {
					rightColumnGroups.push_back(groupPair);
				}
				assignToLeftColumn = !assignToLeftColumn;
			}

			float fullWidth = ImGui::GetContentRegionAvail().x;
			float spacing = ImGui::GetStyle().ItemSpacing.x;
			float columnWidth = (fullWidth - spacing) / 2.0f;

			// --- LEFT COLUMN ---
			ImGui::BeginChild(oxorany("##left_column_container"), ImVec2(columnWidth, 0), false);
			for (const auto& [name, func] : leftColumnGroups) {
				ImGui::PushID(name.c_str());

				BeginCard();

				ImGui::PushFont(headerFont);
				ImGui::Text(oxorany("%s"), name.c_str());
				ImGui::PopFont();

				ImGui::Separator();
				ImGui::PushFont(mainFont);

				func->RenderUI();

				ImGui::PopFont();

				EndCard(func);

				ImGui::PopID();
			}
			ImGui::EndChild();

			// Ставим курсор на ту же линию для начала второй колонки
			// (Place cursor on the same line to start the second column)
			ImGui::SameLine();

			// --- RIGHT COLUMN ---
			ImGui::BeginChild(oxorany("##right_column_container"), ImVec2(columnWidth, 0), false);
			for (const auto& [name, func] : rightColumnGroups) {
				ImGui::PushID(name.c_str());

				BeginCard();

				ImGui::PushFont(headerFont);
				ImGui::Text(oxorany("%s"), name.c_str());
				ImGui::PopFont();

				ImGui::Separator();
				ImGui::PushFont(mainFont);

				func->RenderUI();

				ImGui::PopFont();

				EndCard(func);

				ImGui::PopID();
			}
			ImGui::EndChild();
		}
	}

	struct CategoryAnimConfig {
		float fadeInSpeed = 2.0f;    // скорость появления
		float fadeOutSpeed = 4.0f;   // скорость исчезновения
		float slideDistance = 100.0f; // пикселей для смещения при перелистывании
		float slidePow = 1.0f;       // множитель кривой смещения
		std::function<float(float)> inEasing = easeInOutCubic;
		std::function<float(float)> outEasing = easeOutQuint;
	};

	void RenderAllUI() {
		static int previousCategory = -1;
		static bool isSwitchingCategory = false;
		static std::unordered_map<int, float> categoryAnimationProgress;
		const char* categories[] = { oxorany("Visuals"), oxorany("Combat"), oxorany("Misc"), oxorany("Inventory"), oxorany("Settings") };
		const ImWchar categoryIcons[] = { oxorany(0xe801), oxorany(0xe800), oxorany(0xe802), oxorany(0xe804), oxorany(0xe803) };
		static int selectedCategory = 0;
		static const CategoryAnimConfig animCfg;

		ImVec2 fullSize = ImGui::GetContentRegionAvail();

		// Вычисляем максимально нужную ширину под категорию (иконка + текст)
		ImGui::PushFont(glyphsFont);
		float iconWidth = ImGui::CalcTextSize(oxorany("\uE801")).x; // одна иконка
		ImGui::PopFont();

		float maxCategoryWidth = 0.f;
		ImGui::PushFont(headerFont);
		for (int i = 0; i < IM_ARRAYSIZE(categories); ++i) {
			float w = ImGui::CalcTextSize(categories[i]).x;
			if (w > maxCategoryWidth)
				maxCategoryWidth = w;
		}
		ImGui::PopFont();

		ImGuiStyle& style = ImGui::GetStyle();

		float categoryPanelWidth = maxCategoryWidth + iconWidth
			+ style.FramePadding.x * 4.0f
			+ style.WindowPadding.x * 4.0f;

		float buttonHeight = (fullSize.y / IM_ARRAYSIZE(categories)) * 0.65f;

		ImGui::BeginChild(oxorany("CategoryPanel"), ImVec2(categoryPanelWidth, fullSize.y), true);
		{
			ImGui::PushFont(labelFont);
			ImVec2 labelPos = ImGui::GetCursorPos();
			const char* LABEL = oxorany("Lucent");
			const ImVec2 labelTextSize = ImGui::CalcTextSize(LABEL);
			ImGui::SetCursorPos(ImVec2(categoryPanelWidth / 2 - labelTextSize.x / 2, fullSize.y - labelTextSize.y * 1.2f));
			ImGui::Text(LABEL);
			ImGui::SetCursorPos(labelPos);
			ImGui::PopFont();

			for (int i = 0; i < IM_ARRAYSIZE(categories); ++i) {
				ImGui::PushID(i);
				bool isSelected = (selectedCategory == i);

				char iconUtf8[5] = {};
				ImTextCharToUtf8(iconUtf8, categoryIcons[i]);
				std::string label = std::string(iconUtf8) + " " + categories[i];

				ImGui::PushFont(headerFont);
				if (ImGui::Button(label.c_str(), ImVec2(categoryPanelWidth - style.FramePadding.x * 4, buttonHeight))) {
					if (selectedCategory != i) {
						previousCategory = selectedCategory;
						selectedCategory = i;
						isSwitchingCategory = true;
						for (auto& [_, value] : categoryAnimationProgress)
							value = 0.0f;
					}
				}
				ImGui::PopFont();

				ImGui::PopID();
			}

		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild(oxorany("MainContent"), ImVec2(fullSize.x - 150 - style.ItemSpacing.x, fullSize.y), true);
		{
			float dt = ImGui::GetIO().DeltaTime;

			// Новый контент (fade-in + slide-in)
			float& newProgress = categoryAnimationProgress[selectedCategory];
			if (!isSwitchingCategory) {
				newProgress += dt * animCfg.fadeInSpeed;
				newProgress = std::clamp(newProgress, 0.0f, 1.0f);
			}
			float newAlpha = animCfg.outEasing(newProgress);
			float newOffset = (previousCategory >= 0)
				? ((selectedCategory > previousCategory ? animCfg.slideDistance : -animCfg.slideDistance) *
					powf(1.0f - newAlpha, animCfg.slidePow))
				: 0.0f;

			// Старый контент (fade-out + slide-out)
			float oldAlpha = 0.0f, oldOffset = 0.0f;
			if (previousCategory >= 0 && previousCategory < IM_ARRAYSIZE(categories)) {
				float& oldProgress = categoryAnimationProgress[previousCategory];
				oldProgress += dt * animCfg.fadeOutSpeed;
				oldProgress = std::clamp(oldProgress, 0.0f, 1.0f);
				oldAlpha = animCfg.inEasing(1.0f - oldProgress);
				oldOffset = (selectedCategory > previousCategory ? -animCfg.slideDistance : animCfg.slideDistance) *
					powf(1.0f - oldAlpha, animCfg.slidePow);

				if (oldProgress >= 1.0f) {
					isSwitchingCategory = false;
					previousCategory = -1;
				}
			}

			ImVec2 contentPos = ImGui::GetCursorPos();

			// Рисуем старый (если есть)
			if (previousCategory >= 0 && oldAlpha > 0.0f) {
				ImGui::SetCursorPos(ImVec2(contentPos.x, contentPos.y + oldOffset));
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, oldAlpha);
				RenderCategoryContent(categories[previousCategory]);
				ImGui::PopStyleVar();
			}

			// Рисуем новый
			ImGui::SetCursorPos(ImVec2(contentPos.x, contentPos.y + newOffset));
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, newAlpha);
			RenderCategoryContent(categories[selectedCategory]);
			ImGui::PopStyleVar();

		}
		ImGui::EndChild();
	}

	void SendAllSettings() {
		for (auto* func : functions) {
			if (func->manualSend) continue;

			if (!func->UpdateSettings()) continue;

			const size_t size = func->GetSettingsSize();

			if (size == 0) continue;

			std::vector<uint8_t> buffer(sizeof(FunctionPacket) + size);
			FunctionPacket* packet = reinterpret_cast<FunctionPacket*>(buffer.data());
			packet->functionId = func->GetId();
			packet->dataSize = size;

			func->GetCurrentSettings(buffer.data() + sizeof(FunctionPacket));

			g_NetworkManager.Send(buffer.data(), buffer.size());
		}
	}

	void HandleIncomingData(const uint8_t* data, size_t size) {
		while (size >= sizeof(FunctionPacket)) {
			const FunctionPacket* packet = reinterpret_cast<const FunctionPacket*>(data);

			const size_t totalSize = sizeof(FunctionPacket) + packet->dataSize;

			if (size < totalSize) break;
			for (auto* func : functions) {
				if (func->GetId() == packet->functionId) {
					if (packet->type == PACKET_SETTINGS) {
						func->ApplySettings(data + sizeof(FunctionPacket));
					}
					else if (packet->type == PACKET_DATA) {
						func->ApplyData(data + sizeof(FunctionPacket), packet->dataSize);
					}
					break;
				}
			}

			data += totalSize;
			size -= totalSize;
		}
	}
};