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
#include "skins.h"

#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#pragma pack(push, 1)
enum ItemFlag {
	Nothing = 2,
	TCt = 3,
	T = 10,
	Ct = 6,
	NewNothing = 0,
	NewTCt = 1,
	NewT = 8,
	NewCt = 4
};

// Helper function to get flag name
inline std::string GetFlagName(ItemFlag flag) {
	switch (flag) {
	case Nothing:     return oxorany("Nothing");
	case TCt:         return oxorany("TCt");
	case T:           return oxorany("T");
	case Ct:          return oxorany("Ct");
	case NewNothing:  return oxorany("NewNothing");
	case NewTCt:      return oxorany("NewTCt");
	case NewT:        return oxorany("NewT");
	case NewCt:       return oxorany("NewCt");
	default:          return oxorany("Unknown");
	}
}


struct SkinChangerSettings {
	int id; // id of item in inventory dict, -1 here means add new skin
	int targetDefinitionId;
	ItemFlag flags;
};

struct ItemDefinition {
	// Constructor with parameters
	ItemDefinition(const char* itemName, int itemId) {
		strncpy_s(name, sizeof(name), itemName, _TRUNCATE);
		name[sizeof(name) - 1] = '\0';  // Ensure null termination
		definitionId = itemId;
	}

	// Default constructor
	ItemDefinition() {
		name[0] = '\0';  // Empty string
		definitionId = 0;
	}

	char name[64];
	int definitionId;
};

struct ItemData {
	ItemData(ItemDefinition definition, int _id, int _flags) {
		itemDefinition = definition; flags = _flags; id = _id;
	}
	ItemData() {
		itemDefinition = {}; flags = 0;
	}
	ItemDefinition itemDefinition;
	int id;
	int flags;
};
#pragma pack(pop)

class SkinChanger : public Function {
public:
	SkinChanger() {
		manualSend = true;
	}

	std::string GetName() {
		return oxorany("Inventory");
	}

	uint32_t GetId() const override { return 4; }

	size_t GetSettingsSize() const override { return sizeof(SkinChangerSettings); }

	void RenderInventoryList() {
		ImGui::Text(oxorany("Inventory"));

		ImGui::SameLine();
		ImGui::Spacing();
		ImGui::SameLine();
		if (ImGui::Button(oxorany("Save Skins"))) {
			SaveConfig();
		}
		if (!settings.skinChanger.autoApplyAfterLoading) {
			ImGui::SameLine();
			if (ImGui::Button(oxorany("Load Skins"))) {
				LoadConfig();
			}
		}

		ImGui::Separator();

		std::lock_guard<std::mutex> lock(dataMutex);
		if (currentItems.empty()) {
			ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), oxorany("No items in inventory"));
			ImGui::Text(oxorany("Add skins using the browser on the right"));
			return;
		}

		// Улучшенная таблица с компактным дизайном
		if (ImGui::BeginTable(oxorany("InventoryTable"), 3,
			ImGuiTableFlags_Borders |
			ImGuiTableFlags_RowBg |
			ImGuiTableFlags_ScrollY,
			ImVec2(0, ImGui::GetContentRegionAvail().y - 5)))
		{
			ImGui::TableSetupColumn(oxorany("Item"), ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn(oxorany("Team"), ImGuiTableColumnFlags_WidthFixed, 80);
			ImGui::TableSetupColumn(oxorany("Actions"), ImGuiTableColumnFlags_WidthFixed, 80);
			ImGui::TableHeadersRow();

			for (size_t i = 0; i < currentItems.size(); ++i) {
				auto& item = currentItems[i];
				if (item.itemDefinition.definitionId == 0) continue;

				ImGui::PushID(i);
				ImGui::TableNextRow();

				// Название предмета
				ImGui::TableSetColumnIndex(0);
				ImGui::Text(oxorany("%s"), item.itemDefinition.name);

				// Выбор флага команды
				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1);
				if (ImGui::BeginCombo(oxorany("##teamFlag"), GetFlagName(static_cast<ItemFlag>(item.flags)).c_str(), ImGuiComboFlags_HeightSmall)) {
					const ItemFlag flagOptions[] = { NewNothing, NewTCt, NewT, NewCt, Nothing, TCt, T, Ct };
					for (const auto& flag : flagOptions) {
						bool isSelected = (static_cast<ItemFlag>(item.flags)) == flag;
						if (ImGui::Selectable(GetFlagName(flag).c_str(), isSelected)) {
							item.flags = static_cast<int>(flag);
							SendRequest(item.itemDefinition.definitionId, flag, item.id);
						}
						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}

				// Кнопка удаления
				ImGui::TableSetColumnIndex(2);
				if (ImGui::Button(oxorany("Remove"), ImVec2(-1, 0))) {
					SendRequest(0, static_cast<ItemFlag>(item.flags), item.id);
					currentItems.erase(currentItems.begin() + i);
					i--;
				}

				ImGui::PopID();
			}
			ImGui::EndTable();
		}
	}

	const char* FindSkinNameById(int definitionId) {
		for (int i = 0; i < skinsUIDataSize; i++) {
			if (skinsUIData[i].skinId == definitionId) {
				return skinsUIData[i].name;
			}
		}
		static const std::string str = oxorany("Unknown Skin");
		return str.c_str(); // Возвращаем, если скин не найден
	}

	void RenderSkinBrowser() {
		// Поисковая строка
		ImGui::Text(oxorany("Add Skins"));
		ImGui::PushItemWidth(-1);
		if (ImGui::InputTextWithHint(oxorany("##SearchSkins"), oxorany("Search skins..."), searchQuery, sizeof(searchQuery))) {
			// Автоматический сброс выбора при изменении поиска
			if (searchQuery[0] != '\0') selectedDefinitionId = nullptr;
		}
		ImGui::PopItemWidth();

		// Основная область списка скинов
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		float browserHeight = contentRegion.y - 125; // Оставляем место для опций

		ImGui::BeginChild(oxorany("SkinList"), ImVec2(0, browserHeight), true);
		int matchCount = 0;
		for (int i = 0; i < skinsUIDataSize; i++) {
			auto& skin = skinsUIData[i];
			if (searchQuery[0] == '\0' || StrStrIA(skin.name, searchQuery)) {
				bool isSelected = (selectedDefinitionId == &skin);

				// Компактные элементы списка
				ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.0f, 0.5f));
				if (ImGui::Selectable(skin.name, isSelected, ImGuiSelectableFlags_AllowDoubleClick)) {
					selectedDefinitionId = &skin;
					if (ImGui::IsMouseDoubleClicked(0)) {
						AddSelectedSkin();
					}
				}
				ImGui::PopStyleVar();

				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
				matchCount++;
			}
		}

		if (matchCount == 0) {
			ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
				searchQuery[0] == '\0' ? oxorany("Enter skin name to search") : oxorany("No skins found matching '%s'"), searchQuery);
		}
		ImGui::EndChild();

		if (selectedDefinitionId) {
			// Информация о выбранном скине
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.4f, 1.0f), oxorany("Selected: %s"), selectedDefinitionId->name);
			ImGui::Separator();

			// Выбор флага
			ImGui::SetNextItemWidth(150);
			if (ImGui::BeginCombo(oxorany("Team Flag"), GetFlagName(static_cast<ItemFlag>(selectedFlag)).c_str(), ImGuiComboFlags_HeightSmall)) {
				const ItemFlag flagOptions[] = { NewNothing, NewTCt, NewT, NewCt, Nothing, TCt, T, Ct };
				for (const auto& flag : flagOptions) {
					bool isSelected = (static_cast<ItemFlag>(selectedFlag) == flag);
					if (ImGui::Selectable(GetFlagName(flag).c_str(), isSelected)) {
						selectedFlag = static_cast<int>(flag);
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			// Кнопка добавления
			ImGui::Dummy(ImVec2(0, 10));
			if (ImGui::Button(oxorany("Add to Inventory"), ImVec2(150, 30))) {
				AddSelectedSkin();
			}
			ImGui::SameLine();
			if (ImGui::Button(oxorany("Add && Clear"), ImVec2(150, 30))) {
				AddSelectedSkin();
				searchQuery[0] = '\0';
				selectedDefinitionId = nullptr;
			}
		}
		else {
			ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), oxorany("Select a skin from the list above"));
		}
	}

	void AddSelectedSkin() {
		if (!selectedDefinitionId) return;

		ItemData newItem;
		newItem.id = -1; // новый предмет
		newItem.itemDefinition = ItemDefinition(selectedDefinitionId->name, selectedDefinitionId->skinId);
		newItem.flags = selectedFlag;

		{
			std::lock_guard<std::mutex> lock(dataMutex);
			currentItems.push_back(newItem);
		}

		SendRequest(selectedDefinitionId->skinId, static_cast<ItemFlag>(selectedFlag), newItem.id);
	}

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable skin changer"), settings.skinChanger.enabled);
		if (settings.skinChanger.enabled) {
			ImGui::Indent();

			

			ImGuiEx::Checkbox(oxorany("Auto-apply after loading"), settings.skinChanger.autoApplyAfterLoading, oxorany("Automatically replace the current inventory in the game with the one in the config.\nThe changes will take effect after the next loading of the cheat."));

			// Вкладки
			if (ImGui::BeginTabBar(oxorany("SkinChangerTabs"), ImGuiTabBarFlags_FittingPolicyResizeDown)) {

				// Вкладка Inventory
				if (ImGui::BeginTabItem(oxorany("Inventory"))) {
					//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
					//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));
					//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

					ImGui::BeginChild(oxorany("InventoryChild"), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
					RenderInventoryList();
					ImGui::EndChild();

					//ImGui::PopStyleVar(3);
					ImGui::EndTabItem();
				}

				// Вкладка Skin Browser
				if (ImGui::BeginTabItem(oxorany("Skin Browser"))) {
					//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
					//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));
					//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

					ImGui::BeginChild(oxorany("SkinBrowserChild"), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysUseWindowPadding);
					RenderSkinBrowser();
					ImGui::EndChild();

					//ImGui::PopStyleVar(3);
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::Unindent();
		}
	}

	/*void RenderBackgroundUI() override {
		if (!settings.skinChanger.enabled) return;
	}*/

	void SaveConfig() {
		std::lock_guard<std::mutex> lock(dataMutex); // Потокобезопасность

		// 1. Очищаем старые записи в конфиге
		settings.skinChanger.skins.clear();

		// 2. Проходим по текущему инвентарю и заполняем структуру конфига
		for (const auto& item : currentItems) {
			if (item.itemDefinition.definitionId == 0) continue;

			Settings::SkinChanger::SkinEntry entry;
			entry.definitionId = item.itemDefinition.definitionId;
			entry.flags = item.flags;
			entry.inventoryId = item.id;

			settings.skinChanger.skins.push_back(entry);
		}

		// 3. Вызываем метод сохранения у структуры конфига
		settings.skinChanger.Save();
	}

	void LoadConfig() {
		std::lock_guard<std::mutex> lock(dataMutex);

		// Загружаем сохранённую конфигурацию
		settings.skinChanger.Load();

		// Очищаем текущий инвентарь
		for (const auto& item : currentItems) {
			// Отправляем oxorany("обнуление") на каждый предмет (удаление)
			SendRequest(0, static_cast<ItemFlag>(item.flags), item.id);
		}
		currentItems.clear();

		// Добавляем все скины из сохранённой конфигурации
		for (const auto& entry : settings.skinChanger.skins) {
			ItemData newItem;
			newItem.id = entry.inventoryId;
			newItem.flags = entry.flags;
			newItem.itemDefinition.definitionId = entry.definitionId;

			// Поиск имени скина
			const char* skinName = FindSkinNameById(entry.definitionId);
			strncpy_s(newItem.itemDefinition.name, sizeof(newItem.itemDefinition.name), skinName, _TRUNCATE);

			// Добавляем в текущий список и отправляем на эмулятор
			currentItems.push_back(newItem);
			SendRequest(entry.definitionId, static_cast<ItemFlag>(entry.flags), entry.inventoryId);
		}
	}

	bool SendRequest(int selectedItemDefinitionId, ItemFlag flags, int id) {
		settingsForSending.targetDefinitionId = selectedItemDefinitionId;
		settingsForSending.flags = flags;
		settingsForSending.id = id;

		const size_t size = GetSettingsSize();
		std::vector<uint8_t> buffer(sizeof(FunctionPacket) + size);
		FunctionPacket* packet = reinterpret_cast<FunctionPacket*>(buffer.data());
		packet->functionId = GetId();
		packet->dataSize = size;

		GetCurrentSettings(buffer.data() + sizeof(FunctionPacket));
		g_NetworkManager.Send(buffer.data(), buffer.size());
		return UpdateSettingsBase(settingsForSending, previousSettings);
	}

	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(SkinChangerSettings));
		previousSettings = settingsForSending;
	}

	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(SkinChangerSettings));
	}

	void ApplyData(const void* data, size_t size) override {
		static bool isFirstPacket = true;

		if (size < sizeof(uint32_t)) return;

		const uint8_t* ptr = static_cast<const uint8_t*>(data);
		uint32_t count;
		memcpy(&count, ptr, sizeof(uint32_t));
		ptr += sizeof(uint32_t);
		{

			std::lock_guard<std::mutex> lock(dataMutex);
			currentItems.resize(count);

			memcpy(currentItems.data(), ptr, count * sizeof(ItemData));
		}

		if (settings.skinChanger.autoApplyAfterLoading && isFirstPacket && count > 0) {
			LoadConfig();
			isFirstPacket = false;
		}

		lastUpdateTime = std::chrono::steady_clock::now();
	}

	void Init() override {
	}
private:
	std::chrono::steady_clock::time_point lastUpdateTime;
	char searchQuery[64];
	SkinUI* selectedDefinitionId;
	int selectedFlag = TCt;

	SkinChangerSettings settingsForSending;
	SkinChangerSettings previousSettings;

	std::vector<ItemData> currentItems; // in the game

	std::mutex dataMutex;
};