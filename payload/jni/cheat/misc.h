#pragma once

#define PI 3.141592653f 

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
    uint32_t GetId() const override { return 5; }
    size_t GetSettingsSize() const override { return sizeof(MiscSettings); }

    static inline void (*AfkKickHud_Update_orig)(void* _this) = nullptr;
    static void AfkKickHud_Update_hook(void* _this) {
        if (!settings.disableKickForAFK) {
            AfkKickHud_Update_orig(_this);
        }
    }

    static inline bool (*CharacterController_get_isGrounded_orig)(CharacterController* _this);
    static bool CharacterController_get_isGrounded_hook(CharacterController* _this) {
        if (CharacterController_get_isGrounded_orig == nullptr) {
            CharacterController_get_isGrounded_orig = (decltype(CharacterController_get_isGrounded_orig))il2cpp_resolve_icall(
                oxorany("UnityEngine.CharacterController::get_isGrounded()"));
        }
        return settings.enableAirJump ? true : CharacterController_get_isGrounded_orig(_this);
    }

    static void ProcessNoclip(PlayerController* localPlayer) {
        MovementController* movementController = localPlayer->m_MovementController;
        if (!movementController) return;

        CharacterController* characterController = movementController->_characterController;
        if (!characterController) return;
        characterController->SetEnabled(!settings.noclip);

        if (!settings.noclip) return;

        PlayerTranslationData* translationData = movementController->translationData;
        if (!translationData) return;

        Transform* localTransform = localPlayer->m_Transform;
        if (!localTransform) return;

        if (!localPlayerInputs) return;

        float delta = Time::GetDeltaTime();

        Vector3 currentPos = localTransform->GetPosition();

        Camera* mainCamera = Camera::GetMain();
        if (!mainCamera) return;

        Transform* mainCameraTransform = mainCamera->GetTransform();
        if (!mainCameraTransform) return;

        Vector3 camForward = mainCameraTransform->GetForward();
        camForward.y = 0.f;
        camForward.Normalize();

        Vector3 camRight = mainCameraTransform->GetRight();
        camRight.y = 0.f;
        camRight.Normalize();

        float forwardMove = localPlayerInputs->m_fForwardMove;
        float sideMove = localPlayerInputs->m_fSideMove;
        float upMove = 0.f;

        Vector3 moveVec = camForward * (forwardMove * settings.horizontalSpeed * delta)
            + camRight * (sideMove * settings.horizontalSpeed * delta);

        if (localPlayerInputs->m_bJump)
            moveVec.y += settings.verticalSpeed * delta;
        if (localPlayerInputs->m_bCrouch)
            moveVec.y -= settings.verticalSpeed * delta;

        moveVec.y += upMove * settings.verticalSpeed * delta;

        Vector3 targetPos = currentPos + moveVec;

        translationData->m_vecCurrent = targetPos;
        translationData->m_vecPrev = targetPos;

        localTransform->SetPosition(targetPos);
    }

    inline static Vector3 originalFpCamPlaceholderPos;
    static void ProcessArmsPosition(PlayerController* localPlayer) {
        ArmsAnimationController* armsAnimationController = localPlayer->m_ArmsAnimationController;
        if (!armsAnimationController || !settings.enableArmsPos) return;

        if (originalFpCamPlaceholderPos == Vector3::zero) {
            originalFpCamPlaceholderPos = armsAnimationController->fpCameraPlaceholder->GetLocalPosition();
        }
        armsAnimationController->fpCameraPlaceholder->SetLocalPosition((originalFpCamPlaceholderPos + settings.armsOffset));
    }

    static void LateUpdateCallback() {
        PlayerManager* playerManager = SingletonManager::Instance().GetPlayerManager();
        if (!playerManager) return;

        PlayerController* localPlayer = playerManager->GetLocalPlayer();
        if (!localPlayer || !localPlayer->IsAlive()) return;

        Transform* cameraTransToReset = Camera::GetMain()->GetTransform();

        if (settings.enableAntiAim) {
            if (localPlayer->m_ViewMode == ViewMode::FPS)
                cameraTransToReset = localPlayer->m_AimController->FPSP_go;
            if (cameraTransToReset)
                cameraTransToReset->SetEulerAngles(Vector3{ realAngles.x, realAngles.y, 0.f });
        }

        ProcessTPV(localPlayer);

        ProcessArmsPosition(localPlayer);

        ProcessNoclip(localPlayer);

        Global::OverwrittenValues::g_CameraRotation = cameraTransToReset->GetRotation();
        Global::OverwrittenValues::g_CameraPosition = cameraTransToReset->GetPosition();
    }

    static void ForceMove(PlayerController* player, const Vector2& vecDirection) {
        MovementController* movementController = player->m_MovementController;
        if (movementController) {
            PlayerTranslationParameters* pars = movementController->translationParameters;
            PlayerTranslationData* data = movementController->translationData;
            if (data && pars) {
                PlayerTranslationParameters_WalkParameters* walkPars = pars->walkParameters;
                PlayerTranslationParameters_CrouchParameters* crouchPars = pars->crouchParameters;
                BlendedValue<Vector2>* dir = data->m_vecDampedRelativeDirection;

                AimController* aimController = player->m_AimController;
                if (dir && crouchPars && aimController) {
                    dir->m_actual = (vecDirection * (aimController->_moveState->curState == MoveState::Crouch ? crouchPars->m_fCrouchSpeedMultiplier : walkPars->m_WalkSpeedMultiplier));
                }
            }
        }
    }

    static void MovementFix(PlayerController* player, PlayerInputs* cmd, const float& cameraYaw, const float& realYaw) {
        if (cmd->m_fForwardMove != 0.f || cmd->m_fSideMove != 0.f) {
            float yawDelta = cameraYaw - realYaw;

            float f1{}, f2{};

            if (cameraYaw < 0.f)
                f1 = 360.f + cameraYaw;
            else
                f1 = cameraYaw;

            if (realYaw < 0.f)
                f2 = 360.f + realYaw;
            else
                f2 = realYaw;

            if (f2 < f1)
                yawDelta = abs(f2 - f1);
            else
                yawDelta = 360.0f - abs(f1 - f2);

            yawDelta = 360.0f - yawDelta;

            const Vector2 oldMove{
                Global::Hooks::oldCmd.m_fSideMove,
                Global::Hooks::oldCmd.m_fForwardMove
            };

            const Vector2 fixed{
                cosf(deg2rad(yawDelta)) * oldMove.x + cosf(deg2rad(yawDelta + 90)) * oldMove.y,
                sinf(deg2rad(yawDelta)) * oldMove.x + sinf(deg2rad(yawDelta + 90)) * oldMove.y,
            };

            ForceMove(player, fixed);

            cmd->m_fForwardMove = fixed.y;
            cmd->m_fSideMove = fixed.x;
        }
    }

    static void ProcessGhostMode(PlayerController* localPlayer) {
        localPlayer->m_PhotonView->prefixBackup = settings.ghostMode;
        localPlayer->m_PhotonView->group = settings.ghostMode;
    }

    static inline Vector2 realAngles;
    static inline PlayerInputs* localPlayerInputs = nullptr;
    static void CreateMoveCallback(Il2CppObject* obj, PlayerInputs* cmd) {
        localPlayerInputs = cmd;

        if (!settings.enableAntiAim)
            return;

        PlayerManager* playerManager = SingletonManager::Instance().GetPlayerManager();
        if (!playerManager) return;
        PlayerController* localPlayer = playerManager->GetLocalPlayer();
        if (!localPlayer || !localPlayer->IsAlive()) return;

        AimController* aimController = localPlayer->m_AimController;
        if (!aimController) return;

        if (realAngles == Vector2{}) {
            realAngles.x = aimController->aimingData->angPitch.x - Global::Hooks::oldCmd.m_vecDeltaAimAngles.x;
            realAngles.y = aimController->aimingData->angYaw.y + Global::Hooks::oldCmd.m_vecDeltaAimAngles.y;
        }
        else {
            realAngles.x -= Global::Hooks::oldCmd.m_vecDeltaAimAngles.x;
            realAngles.y += Global::Hooks::oldCmd.m_vecDeltaAimAngles.y;
        }


        if (realAngles.x > 70.f) realAngles.x = 70.f;
        if (realAngles.x < -70.f) realAngles.x = -70.f;

        // Нормализуем Yaw, чтобы он оставался в пределах -180/180
        while (realAngles.y > 180.f) realAngles.y -= 360.f;
        while (realAngles.y < -180.f) realAngles.y += 360.f;

        Vector2 fakeAngles = realAngles;
        static float spinYaw = 0.f;
        static bool jitterFlip = false;
        switch (settings.antiAimType) {
        case 0: // Static
            fakeAngles.x = settings.antiAimAngles.x;
            fakeAngles.y = settings.antiAimAngles.y;
            break;
        case 1: // Jitter
            fakeAngles.x = settings.antiAimAngles.x;
            fakeAngles.y = settings.antiAimAngles.y + (jitterFlip ? settings.jitterRange : -settings.jitterRange);
            jitterFlip = !jitterFlip;
            break;
        case 2: // Spin
            fakeAngles.x = settings.antiAimAngles.x;
            spinYaw += settings.spinSpeed;
            if (spinYaw > 180.f) spinYaw -= 360.f;
            if (spinYaw < -180.f) spinYaw += 360.f;
            fakeAngles.y = spinYaw;
            break;
        }

        aimController->aimingData->angPitch.x = fakeAngles.x;
        aimController->aimingData->angYaw.y = fakeAngles.y;

        if (!settings.noclip) {
            MovementFix(localPlayer, cmd, realAngles.y, aimController->aimingData->angYaw.y);
        }
    }

    static void ProcessIncognitoMode(PlayerController* player) {
        if (!player || !settings.incognitoMode) return;

        PhotonPlayer* photonPlayer = player->m_PhotonPlayer;
        if (!photonPlayer) return;

        photonPlayer->m_CustomProperties->SetValue<Il2CppString*>(oxorany("uid"), Il2CppString::CreateString(oxorany("1337")));

        photonPlayer->m_NameField = Il2CppString::CreateString((oxorany("Player N") + std::to_string(photonPlayer->m_CustomProperties->GetValue<int32_t>(oxorany("player_number")))).c_str());

        SingletonManager::Instance().GetGameSettingsManager()->gameSettings->HideAvatars = true;

        // static Il2CppClass* systemByteKlass = Il2CppUtils::FindKlass(oxorany("System"), oxorany("Byte"));
        // LOGD("systemByteKlass: %p", systemByteKlass);
        // Il2CppArray<uint8_t>* oldAvatar = photonPlayer->customProperties->GetValue<Il2CppArray<uint8_t>*>(oxorany("avatar"));
        // if (!oldAvatar) continue;
        // LOGD("oldAvatar: %p", oldAvatar);
        // LOGD("oldAvatar->size: %d", oldAvatar->size);
        // if (oldAvatar->size <= 0) continue;
        // auto newAvatar = (Il2CppArray<uint8_t>*)il2cpp_array_new(systemByteKlass, oldAvatar->size + 1);
        // LOGD("newAvatar: %p", newAvatar);

        // photonPlayer->customProperties->SetValue<Il2CppArray<uint8_t>*>(oxorany("avatar"), newAvatar);
    }

    static inline void (*RankedConfirmationDialog_OnOpen_orig)(RankedConfirmationDialog* _this);
    static void RankedConfirmationDialog_OnOpen_hook(RankedConfirmationDialog* _this) {
        RankedConfirmationDialog_OnOpen_orig(_this);
        RankedConfirmationDialog_Confirm((void*)_this);
    }

    void Init() override {
        Global::Hooks::AddPlayerUpdateCallback(PlayerUpdateCallback);
        Global::Hooks::AddGameUpdateCallback(GameUpdateCallback);
        Global::Hooks::AddLateUpdateCallback(LateUpdateCallback);
        Global::Hooks::AddCreateMoveCallback(CreateMoveCallback);

        *(void**)(g_LibUnityBase + ISGROUNDED_HOOK_ADDRESS) = (void*)&CharacterController_get_isGrounded_hook;

        Il2CppClass* playerControllerClass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Player"), oxorany("PlayerController"));

        Il2CppClass* afkKickHudKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Game.UI.AfkKick"), oxorany("AfkKickHud"));
        Il2CppUtils::MethodHook(afkKickHudKlass, oxorany("Update"), AfkKickHud_Update_hook, &AfkKickHud_Update_orig);

        static Il2CppClass* rankedConfirmationDialogKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Main.Play"), oxorany("RankedConfirmationDialog"));
        int onOpenSlot = Il2CppUtils::FindVirtualMethodSlot(rankedConfirmationDialogKlass, oxorany("OnOpen"), 0);
        RankedConfirmationDialog_OnOpen_orig = (decltype(RankedConfirmationDialog_OnOpen_orig))Il2CppUtils::ReplaceVirtualMethod(rankedConfirmationDialogKlass, onOpenSlot, (void*)&RankedConfirmationDialog_OnOpen_hook);
    }

    static void ProcessFov() {
        if (settings.enableFov) {
            Camera::GetMain()->SetFOV(settings.fovValue);
        }
        else {
            Camera::GetMain()->SetFOV(60.f);
        }
    }

    static void ProcessMovement(PlayerController* _this) {
        MovementController* movementController = _this->m_MovementController;
        if (!movementController) return;

        PlayerTranslationParameters* translationParameters = movementController->translationParameters;
        if (!translationParameters) return;

        PlayerTranslationData* translationData = movementController->translationData;
        if (!translationData) return;

        if (settings.enableStrafes) {
            translationData->m_VecMoveDirection = Vector3{};

            translationParameters->moveDirectionChangeSpeed = 100.f;
            translationParameters->mecanimDirectionChangeSpeed = 100.f;
            translationParameters->jumpParameters->JumpMoveSpeed = settings.jumpMoveSpeed;

            translationParameters->jumpParameters->LandDelayCurve->ClearKeys();
            translationParameters->jumpParameters->LandDelayDurationCurve->ClearKeys();
            translationParameters->jumpParameters->LandDelayMultCurve->ClearKeys();
        }
        else {
            translationParameters->moveDirectionChangeSpeed = 4.f;
            translationParameters->mecanimDirectionChangeSpeed = 4.f;
            translationParameters->jumpParameters->JumpMoveSpeed = 0.4f;
        }
    }

    static void ProcessTPV(PlayerController* localPlayer) {
        static bool wasTPS = false;

        Transform* anchor = localPlayer->m_MainCameraHolder;
        if (!anchor) return;

        if (wasTPS && !settings.thirdPersonView) {
            PlayerController_SetFPSView(localPlayer);
        }

        if (settings.thirdPersonView) {
            PlayerController_SetTPSView(localPlayer);

            Camera* mainCam = Camera::GetMain();
            if (!mainCam) return;

            Transform* mainCameraTrans = mainCam->GetTransform();
            if (!mainCameraTrans) return;

            Vector3 anchorPos = mainCameraTrans->GetPosition();
            Vector3 backward = -mainCameraTrans->GetForward();

            Vector3 camOffset = backward * settings.value + Vector3{ 0, settings.verticalOffset, 0 };
            Vector3 camPos = anchorPos + camOffset;

            if (!settings.noclip) {
                RaycastHit out;
                if (Physics_Linecast(anchorPos, camPos, &out, 16384)) {
                    Collider* collider = out.GetCollider();
                    if (collider) {
                        const SurfaceType type = (SurfaceType)SurfaceTypeUtility_FromComponent(collider);
                        if (type != SurfaceType::Character) {
                            camPos = out.m_Point;
                        }
                    }
                }
            }

            mainCameraTrans->SetPosition(camPos);

            wasTPS = settings.thirdPersonView;
        }
    }

    static MiscSettings GetSettings() {
        return settings;
    }

    static void GameUpdateCallback(GameController* _this) {

        if (settings.moveBeforeTime) {
            PlayerControls* playerControls = _this->_playerControls;

            if (playerControls) {
                if (playerControls->_disableMovementRequests)
                    playerControls->_disableMovementRequests->Clear();

                if (playerControls->_disableFiringRequests)
                    playerControls->_disableFiringRequests->Clear();

                if (playerControls->_disableDropRequests)
                    playerControls->_disableDropRequests->Clear();
            }
        }
    }

    static void PlayerUpdateCallback(PlayerController* _this) {
        // Timer timer(oxorany("Misc::PlayerUpdateCallback"));
        ProcessIncognitoMode(_this);

        if (!_this->m_PhotonPlayer->m_IsLocal || !_this->IsAlive()) return;

        ProcessFov();

        ProcessGhostMode(_this);

        ProcessMovement(_this);
    }

    void ApplySettings(const void* data) override {
        memcpy(&settings, data, sizeof(MiscSettings));
    }
    void GetCurrentSettings(void* out) override {
        memcpy(out, &settings, sizeof(MiscSettings));
    }
    static MiscSettings GetCurrentSettings() {
        return settings;
    }
private:
    inline static MiscSettings settings;
};