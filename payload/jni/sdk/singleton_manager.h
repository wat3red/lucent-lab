#pragma once

#include "utils.h"
#include "sdk/axlebolt_classes.h"

class SingletonManager {
public:
    static SingletonManager& Instance() {
        static SingletonManager instance;
        return instance;
    }

    PlayerManager* GetPlayerManager() {
        return playerManager;
    }

    InventoryManager* GetInventoryManager() {
        return inventoryManager;
    }

    MainController* GetMainController() {
        return mainController;
    }

    GrenadeEffectEmitter* GetGrenadeEffectEmitter() {
        return grenadeEffectEmitter;
    }

    GrenadeManager* GetGrenadeManager() {
        return grenadeManager;
    }

    AntiCheatManager* GetAntiCheatManager() {
        return antiCheatManager;
    }

    GameSettingsManager* GetGameSettingsManager() {
        return gameSettingsManager;
    }

    GameController* GetGameController() {
        return gameController;
    }

    void Update() {
        static Il2CppClass* playerManagerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Player"), oxorany("PlayerManager"));
        static Il2CppClass* inventoryManagerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Main.Inventory"), oxorany("InventoryManager"));
        static Il2CppClass* mainControllerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Main"), oxorany("MainController"));
        static Il2CppClass* grenadeManagerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Inventory.Grenade"), oxorany("GrenadeManager"));
        static Il2CppClass* grenadeEffectEmitterKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Inventory.Grenade"), oxorany("GrenadeEffectEmitter"));
        static Il2CppClass* antiCheatManagerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Anitcheat"), oxorany("AntiCheatManager"));
        static Il2CppClass* gameSettingsManagerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Settings.Game"), oxorany("GameSettingsManager"));
        static Il2CppClass* gameControllerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Game"), oxorany("GameController"));

        playerManager = (PlayerManager*)GetLazySingletonInstance(playerManagerKlass);
        inventoryManager = (InventoryManager*)GetLazySingletonInstance(inventoryManagerKlass);
        mainController = *(MainController**)((uintptr_t)(mainControllerKlass->static_fields) + 0x4);
        grenadeManager = (GrenadeManager*)GetLazySingletonInstance(grenadeManagerKlass);
        grenadeEffectEmitter = (GrenadeEffectEmitter*)GetLazySingletonInstance(grenadeEffectEmitterKlass);
        antiCheatManager = (AntiCheatManager*)GetLazySingletonInstance(antiCheatManagerKlass);
        gameSettingsManager = *(GameSettingsManager**)((uintptr_t)(gameSettingsManagerKlass->static_fields) + 0x4);
        gameController = *(GameController**)((uintptr_t)(gameControllerKlass->static_fields) + 0x4);
    }
private:
    PlayerManager* playerManager = nullptr;
    InventoryManager* inventoryManager = nullptr;
    MainController* mainController = nullptr;
    GrenadeManager* grenadeManager = nullptr;
    GrenadeEffectEmitter* grenadeEffectEmitter = nullptr;
    AntiCheatManager* antiCheatManager = nullptr;
    GameSettingsManager* gameSettingsManager = nullptr;
    GameController* gameController = nullptr;

    SingletonManager() {}

    Il2CppObject* GetLazySingletonInstance(Il2CppClass* klass) {
        if (klass == nullptr) {
            LOGE("GetLazySingletonInstance: klass is null!");
            return nullptr;
        }

        if (klass->_1.parent == nullptr) {
            LOGE("GetLazySingletonInstance: klass->_1.parent is null for class %s!", klass->_1.name);
            return nullptr;
        }

        if (klass->_1.parent->static_fields == nullptr) {
            LOGE("GetLazySingletonInstance: static_fields is null for class %s!", klass->_1.parent->_1.name);
            return nullptr;
        }

        Il2CppObject* instance = *(Il2CppObject**)(klass->_1.parent->static_fields);
        return instance;
    }


    SingletonManager(const SingletonManager&) = delete;
    SingletonManager& operator=(const SingletonManager&) = delete;
};
