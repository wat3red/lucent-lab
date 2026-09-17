#pragma once

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

#pragma pack(push, 1)
struct SkinChangerSettings {
    int id;
    int targetDefinitionId;
    ItemFlag flags;
};

struct ItemDefinition {
    ItemDefinition(const char* itemName, int itemId) {
        strncpy(name, itemName, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        definitionId = itemId;
    }

    ItemDefinition() {
        name[0] = '\0';
        definitionId = 0;
    }

    char name[64];
    int definitionId;
};

struct ItemData {
    ItemData(ItemDefinition definition, int _id, int _flags) {
        itemDefinition = definition;
        flags = _flags;
        id = _id;
    }

    ItemData() {
        itemDefinition = {};
        flags = 0;
    }

    ItemDefinition itemDefinition;
    int id;
    int flags;
};
#pragma pack(pop)

class SkinChanger : public Function {
public:
    uint32_t GetId() const override { return 4; }
    size_t GetSettingsSize() const override { return sizeof(SkinChangerSettings); }

    void GetInventoryData() {
        auto now = std::chrono::steady_clock::now();
        if (inventoryCacheValid && std::chrono::duration_cast<std::chrono::milliseconds>(now - lastInventoryUpdate).count() < 1000) {
            return; // Используем кэш
        }

        currentItems.clear();
        auto inventoryService = GetInventoryService();
        if (!inventoryService) return;

        auto items = inventoryService->GetBoltInventoryItems();
        auto definitions = ExtractItemDefinitions(inventoryService->GetSomeThings1());

        std::unordered_map<int, ItemDefinition> defMap;
        for (auto& def : definitions)
            defMap[def.definitionId] = def;

        for (auto* item : items->GetValues()) {
            if (!item) continue;
            auto it = defMap.find(item->definitionId);
            ItemDefinition def = (it != defMap.end()) ? it->second : ItemDefinition{};
            currentItems.emplace_back(def, item->id, item->flags);
        }

        lastInventoryUpdate = now;
        inventoryCacheValid = true;
    }

    void ApplySettings(const void* data) override {
        memcpy(&settings, data, sizeof(SkinChangerSettings));
        InventoryService* inventoryService = GetInventoryService();
        if (!inventoryService) return;

        auto items = inventoryService->GetBoltInventoryItems();
        if (UpdateExistingItem(items)) return;

        AddNewItem(items);

        inventoryCacheValid = false; // сбросим кэш
    }

    void GetData(std::vector<uint8_t>& out) override {
        std::lock_guard<std::mutex> lock(dataMutex);

        GetInventoryData(); // Кэшируем, если нужно

        size_t count = currentItems.size();
        out.resize(sizeof(uint32_t) + count * sizeof(ItemData));
        memcpy(out.data(), &count, sizeof(uint32_t));

        if (count > 0) {
            memcpy(out.data() + sizeof(uint32_t), currentItems.data(), count * sizeof(ItemData));
        }
    }

    void GetCurrentSettings(void* out) override {
        memcpy(out, &settings, sizeof(SkinChangerSettings));
    }

private:
    std::chrono::steady_clock::time_point lastInventoryUpdate = std::chrono::steady_clock::now() - std::chrono::seconds(2);
    bool inventoryCacheValid = false;

    InventoryService* GetInventoryService() {
        static Il2CppClass* klass = Il2CppUtils::FindKlass("", INVENTORY_CLASS_NAME);
        if (!klass)
        {
            LOGE("Inventory klass is null!");
            return nullptr;
        }
        Il2CppClass* parent = (Il2CppClass*)(klass->_1.parent);
        if (parent) {
            void* staticFields = parent->static_fields;
            if (staticFields)
                return *(InventoryService**)staticFields;
        }
        return nullptr;
    }

    std::vector<ItemDefinition> ExtractItemDefinitions(Dictionary<int, tempName1*>* allDefinitions) {
        std::vector<ItemDefinition> definitions;
        if (!allDefinitions) return definitions;

        auto defs = allDefinitions->GetValues();
        definitions.reserve(defs.size());

        for (auto* definition : defs) {
            if (!definition || !definition->str1 || definition->str1->length == 0) continue;

            const char* str = definition->str1->c_str();
            if (!str || str[0] == '\0') continue;

            try {
                definitions.emplace_back(str, definition->int1);
                // free((void*)str);
            }
            catch (...) {
                LOGE("Error processing item definition.");
            }
        }

        return definitions;
    }

    ItemDefinition FindDefinition(const std::vector<ItemDefinition>& definitions, int id) {
        for (const auto& def : definitions) {
            if (def.definitionId == id) {
                return def;
            }
        }
        return {};
    }

    bool UpdateExistingItem(Dictionary<int, BoltInventoryItem*>* items) {
        for (auto* item : items->GetValues()) {
            if (!item) continue;
            if (item->id == settings.id && settings.id != -1) {
                item->definitionId = settings.targetDefinitionId;
                item->flags = settings.flags;
                return true;
            }
        }
        return false;
    }

    void AddNewItem(Dictionary<int, BoltInventoryItem*>* items) {
        auto keys = items->GetKeys();
        // LOGD("Retrieved keys from dictionary. Total keys: %zu", keys.size());

        if (keys.empty()) {
            LOGE("No keys in the dictionary. Cannot find template item.");
            return;
        }

        int maxId = *std::max_element(keys.begin(), keys.end());
        // LOGD("Max ID found in dictionary: %d", maxId);

        BoltInventoryItem* templateItem = nullptr;
        int templateId = maxId;

        // Try to find a valid template item by decrementing the ID
        while (templateId >= 0) {
            templateItem = items->GetValue(templateId);
            // LOGD("Trying to get template item at ID %d: %p", templateId, templateItem);

            if (templateItem) break;
            templateId--;
        }

        if (!templateItem) {
            LOGE("No valid template item found in dictionary. Aborting.");
            return;
        }

        static Il2CppClass* boltItemKlass = Il2CppUtils::FindKlass("", BOLT_ITEM_CLASS_NAME);
        // LOGD("Found BoltInventoryItem class: %p", boltItemKlass);

        Il2CppObject* newObj = il2cpp_object_new(boltItemKlass);
        // LOGD("Allocated new Il2CppObject: %p", newObj);

        auto* newItem = (BoltInventoryItem*)newObj;
        // LOGD("Casted new object to BoltInventoryItem*: %p", newItem);

        if (!newItem) {
            LOGE("Failed to allocate new item.");
            return;
        }

        newItem->definitionId = settings.targetDefinitionId;
        // LOGD("Set newItem->definitionId = %d", settings.targetDefinitionId);

        newItem->flags = settings.flags;
        // LOGD("Set newItem->flags = %d", settings.flags);

        newItem->id = templateItem->id + 1;
        // LOGD("Set newItem->id = %d (template id: %d)", newItem->id, templateItem->id);

        newItem->quantity = 1;
        // LOGD("Set newItem->quantity = 1");

        newItem->klass = templateItem->klass;
        // LOGD("Copied klass pointer from template: %p", newItem->klass);

        newItem->monitor = templateItem->monitor;
        // LOGD("Copied monitor pointer from template: %p", newItem->monitor);

        newItem->props = templateItem->props;
        // LOGD("Copied props pointer from template: %p", newItem->props);

        newItem->unk = templateItem->unk;
        // LOGD("Copied unk pointer from template: %p", newItem->unk);

        static MethodInfo* tryInsert = Il2CppUtils::FindMethod(items->klass, oxorany("TryInsert"), 3);
        // LOGD("Found method TryInsert: %p", tryInsert);

        typedef bool(*Dictionary_int_object_TryInsert_t)(
            Dictionary<int, Il2CppObject*>* __this, int key, Il2CppObject* value, uint8_t behavior, MethodInfo* method);

        ((Dictionary_int_object_TryInsert_t)tryInsert->methodPointer)((Dictionary<int, Il2CppObject*>*)items, newItem->id, newObj, 1, tryInsert);

        // LOGD("Inserted new item with ID %d into dictionary", newItem->id);
    }

private:
    static inline SkinChangerSettings settings;
    std::vector<ItemData> currentItems;
    std::mutex dataMutex;
};