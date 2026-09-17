#pragma once
#include "../sdk/axlebolt_classes.h"
#include "../cheat/functions_manger.h"
#include "../il2cpp/il2cpp_utils.h"
#include <vector>
#include "timer.h" // Include Timer here

namespace Global {
    namespace Hooks {
        FunctionsManager* functionsManager;

        void SetFunctionsManager(FunctionsManager* m) {
            functionsManager = m;
        }

        std::vector<void(*)(Scene)> sceneUnloadedCallbacks;
        inline void (*SceneManager_Internal_SceneUnloaded_orig)(Scene scene);
        void SceneManager_Internal_SceneUnloaded_hook(Scene scene) {
            SceneManager_Internal_SceneUnloaded_orig(scene);
            for (auto callback : sceneUnloadedCallbacks) {
                callback(scene);
            }
        }

        std::vector<void(*)(Scene, int)> sceneLoadedCallbacks;
        inline void (*SceneManager_Internal_SceneLoaded_orig)(Scene scene, int mode);
        void SceneManager_Internal_SceneLoaded_hook(Scene scene, int mode) {
            SceneManager_Internal_SceneLoaded_orig(scene, mode);
            for (auto callback : sceneLoadedCallbacks) {
                callback(scene, mode);
            }
        }

        std::vector<void(*)(Il2CppObject*, PlayerInputs*)> createMoveCallbacks;
        inline PlayerInputs oldCmd;
        inline void (*PlayerControls_CreateMove_orig)(Il2CppObject* obj, PlayerInputs* cmd);
        void PlayerControls_CreateMove_hook(Il2CppObject* obj, PlayerInputs* cmd) {
            if (cmd)
                oldCmd = *cmd;
            PlayerControls_CreateMove_orig(obj, cmd);
            for (auto callback : createMoveCallbacks) {
                callback(obj, cmd);
            }
        }

        std::vector<void(*)(PlayerHitController*, PlayerController*, HitData*)> onLocalCharacterHitCallbacks;
        inline void (*PlayerHitController_OnLocalCharacterHit_orig)(PlayerHitController* _this, PlayerController* player, HitData* hitData);
        void PlayerHitController_OnLocalCharacterHit_hook(PlayerHitController* _this, PlayerController* player, HitData* hitData) {
            PlayerHitController_OnLocalCharacterHit_orig(_this, player, hitData);
            for (auto callback : onLocalCharacterHitCallbacks) {
                callback(_this, player, hitData);
            }
        }

        std::vector<void(*)(GameController*)> gameControllerUpdateCallbacks;
        inline void (*GameController_Update_orig)(GameController* _this);
        void GameController_Update_hook(GameController* _this) {
            GameController_Update_orig(_this);

            for (auto callback : gameControllerUpdateCallbacks) {
                callback(_this);
            }

            PlayerControls* playerControls = _this->_playerControls;
            if (playerControls) {
                Action* playerInputsDelegate = playerControls->_playerInputsFilter;
                if (PlayerControls_CreateMove_orig == nullptr)
                    PlayerControls_CreateMove_orig = (decltype(PlayerControls_CreateMove_orig))playerInputsDelegate->method_ptr;
                if (playerInputsDelegate) {
                    playerInputsDelegate->invoke_impl = (void*)(&PlayerControls_CreateMove_hook);
                }
            }
        }

        std::vector<void(*)(PlayerController*)> playerControllerUpdateCallbacks;
        inline void (*PlayerController_Update_orig)(PlayerController* _this);
        void PlayerController_Update_hook(PlayerController* _this) {
            PlayerController_Update_orig(_this);

            for (auto callback : playerControllerUpdateCallbacks) {
                callback(_this);
            }

            if (_this->m_PhotonPlayer && _this->m_PhotonPlayer->m_IsLocal) {
                Action* onHitDelegate = _this->m_PlayerHitController->OnHit;

                if (onHitDelegate) {
                    if (!PlayerHitController_OnLocalCharacterHit_orig)
                        PlayerHitController_OnLocalCharacterHit_orig = (decltype(PlayerHitController_OnLocalCharacterHit_orig))onHitDelegate->method_ptr;
                    onHitDelegate->method_ptr = (int32_t)(&PlayerHitController_OnLocalCharacterHit_hook);
                }
            }
        }

        std::vector<void(*)()> lateUpdateCallbacks;
        inline void (*LateUpdate_orig)(void* _this);
        void LateUpdate_hook(void* _this) {
            LateUpdate_orig(_this);
            for (auto callback : lateUpdateCallbacks) {
                callback();
            }
        }

        void InitLateUpdate() {
            static Il2CppClass* inputFieldNoInsertKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Game.UI.Chat"), oxorany("InputFieldNoInsert"));
            Il2CppUtils::MethodHook(inputFieldNoInsertKlass, oxorany("LateUpdate"), LateUpdate_hook, (void*)&LateUpdate_orig);
            static Il2CppClass* gameObjectKlass = Il2CppUtils::FindKlass(oxorany("UnityEngine"), oxorany("GameObject"));
            GameObject* go = (GameObject*)il2cpp_object_new(gameObjectKlass);
            go->Initialize(Il2CppString::CreateString(oxorany("lateUpdateHolder")));
            go->AddComponent(Type::GetType(Il2CppString::CreateString(
                oxorany("Axlebolt.Standoff.Game.UI.Chat.InputFieldNoInsert, Assembly-CSharp.dll")
            )));
            go->DontDestroyOnLoad();
        }

        void InitHooks() {
            InitLateUpdate();

            Il2CppClass* playerControllerClass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Player"), oxorany("PlayerController"));
            Il2CppUtils::MethodHook(playerControllerClass, oxorany("Update"), PlayerController_Update_hook, &PlayerController_Update_orig);

            Il2CppClass* gameControllerClass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Game"), oxorany("GameController"));
            Il2CppUtils::MethodHook(gameControllerClass, oxorany("Update"), GameController_Update_hook, &GameController_Update_orig);

            Il2CppClass* sceneManagerKlass = Il2CppUtils::FindKlass(oxorany("UnityEngine.SceneManagement"), oxorany("SceneManager"));
            Il2CppUtils::MethodHook(sceneManagerKlass, oxorany("Internal_SceneLoaded"), SceneManager_Internal_SceneLoaded_hook,
                &SceneManager_Internal_SceneLoaded_orig);
            Il2CppUtils::MethodHook(sceneManagerKlass, oxorany("Internal_SceneUnloaded"), SceneManager_Internal_SceneUnloaded_hook,
                &SceneManager_Internal_SceneUnloaded_orig);
        }

        void AddSceneLoadedCallback(void (*callback)(Scene scene, int mode)) {
            sceneLoadedCallbacks.push_back(callback);
        }

        void AddSceneUnloadedCallback(void (*callback)(Scene scene)) {
            sceneUnloadedCallbacks.push_back(callback);
        }

        void AddGameUpdateCallback(void (*callback)(GameController*)) {
            gameControllerUpdateCallbacks.push_back(callback);
        }

        void AddPlayerUpdateCallback(void (*callback)(PlayerController*)) {
            playerControllerUpdateCallbacks.push_back(callback);
        }

        void AddLateUpdateCallback(void (*callback)()) {
            lateUpdateCallbacks.push_back(callback);
        }

        void AddCreateMoveCallback(void (*callback)(Il2CppObject*, PlayerInputs*)) {
            createMoveCallbacks.push_back(callback);
        }

        void AddLocalPlayerHitCallback(void (*callback)(PlayerHitController*, PlayerController*, HitData*)) {
            onLocalCharacterHitCallbacks.push_back(callback);
        }
    };
}