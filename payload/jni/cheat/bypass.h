#pragma once

#include <map>

typedef Il2CppObject* (*FDBDAFHHCFDGHCH_t)(void* pThis);
static std::map<std::string, FDBDAFHHCFDGHCH_t> g_OriginalFDBDAFHHCFDGHCH;

class Bypass : public Function {
    uint32_t GetId() const override { return 9; }

    // static void GameUpdateCallback(GameController* _this) {
    void Update() override {
        AntiCheatManager* antiCheatManager = SingletonManager::Instance().GetAntiCheatManager();
        // static Type* antiCheatManagerType = Type::GetType(Il2CppString::CreateString(oxorany("Axlebolt.Standoff.Anitcheat.AntiCheatManager, Assembly-CSharp.dll")));
        // Il2CppArray<AntiCheatManager*>* antiCheatManagers = (Il2CppArray<AntiCheatManager*>*)Object_FindObjectsByType(antiCheatManagerType, false, false);
        // LOGD("antiCheatManagerType: %p", antiCheatManagerType);
        // LOGD("antiCheatManagers->size: %p", antiCheatManagers->size);
        LOGD("antiCheatManager: %p", antiCheatManager);
        if (!antiCheatManager) return;

        // 0x1C 
        List<AntiCheatDetector*>* detectorList1 = antiCheatManager->GetDetectors1();
        if (!detectorList1) return;
        Il2CppArray<AntiCheatDetector*>* detectors1 = detectorList1->items;
        if (!detectors1) return;
        // LOGI("Processing %d detectors from DetectorList1", detectors1->size);
        for (int i = 0; i < detectors1->size; ++i) {
            AntiCheatDetector* detector = detectors1->items[i];
            if (detector) {
                // LOGD("DetectorList1[%d] -> undetected set to true", i);
                detector->undetected = true;
            }
        }

        // 0x20 
        List<AntiCheatDetector*>* detectorList2 = antiCheatManager->GetDetectors2();
        if (!detectorList2) return;
        Il2CppArray<AntiCheatDetector*>* detectors2 = detectorList2->items;
        if (!detectors2) return;
        // LOGI("Processing %d detectors from DetectorList2", detectors2->size);
        for (int i = 0; i < detectors2->size; ++i) {
            AntiCheatDetector* detector = detectors2->items[i];
            if (detector) {
                // LOGD("DetectorList2[%d] -> undetected set to true", i);
                detector->undetected = true;
            }
        }
    };

    void Init() override {
        // Global::Hooks::AddGameUpdateCallback(GameUpdateCallback);
    }
};