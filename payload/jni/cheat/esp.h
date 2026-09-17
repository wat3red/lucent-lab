#pragma once

#pragma pack(push, 4)
struct BipedMapToSend {
    Vector3 Head;
    Vector3 Neck;
    Vector3 Spine;
    Vector3 Spine1;
    Vector3 Spine2;
    Vector3 LeftShoulder;
    Vector3 LeftUpperarm;
    Vector3 LeftForearm;
    Vector3 LeftHand;
    Vector3 RightShoulder;
    Vector3 RightUpperarm;
    Vector3 RightForearm;
    Vector3 RightHand;
    Vector3 Hip;
    Vector3 LeftUpLeg;
    Vector3 LeftLeg;
    Vector3 LeftFoot;
    Vector3 LeftToeBase;
    Vector3 RightUpLeg;
    Vector3 RightLeg;
    Vector3 RightFoot;
    Vector3 RightToeBase;
    Vector3 ControllerPos;

    Vector3 boxCorners[8]; // this line for 3D box corners
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ESPSettings {
    bool enabled;
};

struct EnemyData {
    int androidWidth = 0;
    int androidHeight = 0;
    float distanceToLocalPlayer = 0.0f;
    int health = 0; // Health bar
    int maxHealth = 0;
    int ammo = 0; // Armor bar
    int maxAmmo = 0;
    int armor = 0;
    BipedMapToSend bipedMap = {};
    WeaponId currentWeaponId = WeaponId::None;
    char nickName[128] = "";
    // Flags
    bool hasHelmet = false;
    Team team = Team::None;
    bool hasExplosiveRelatedItem = false; // has defuse kit or bomb, we'll just check the team and draw corresponding text
    bool isScoped = false;
    bool isReloading = false;
    bool isPlantingOrDefusing = false;
};
#pragma pack(pop)

class ESP : public Function {
public:
    uint32_t GetId() const override { return 2; }
    size_t GetSettingsSize() const override { return sizeof(ESPSettings); }

    void Compute3DBoxCorners(const Vector3& origin, const Vector3& camPos, const Quaternion& camRot, float fov, Vector3 outCorners[8]) {
        // Box half extents (adjust for your player model)
        float halfWidth = 0.3f;
        float halfDepth = 0.3f;
        float height = 1.8f;

        Vector3 topCenter = origin + Vector3(0, height, 0);
        Vector3 bottomCenter = origin;

        // Define 8 corners of the 3D bounding box in world space
        Vector3 corners[8] = {
            // Bottom square
            bottomCenter + Vector3(-halfWidth, 0, -halfDepth),
            bottomCenter + Vector3(halfWidth, 0, -halfDepth),
            bottomCenter + Vector3(halfWidth, 0, halfDepth),
            bottomCenter + Vector3(-halfWidth, 0, halfDepth),

            // Top square
            topCenter + Vector3(-halfWidth, 0, -halfDepth),
            topCenter + Vector3(halfWidth, 0, -halfDepth),
            topCenter + Vector3(halfWidth, 0, halfDepth),
            topCenter + Vector3(-halfWidth, 0, halfDepth)
        };

        // Convert to screen space
        for (int i = 0; i < 8; i++) {
            outCorners[i] = WorldToScreen_Internal(corners[i], camPos, camRot, fov);
        }
    }

    void Update() override {
        if (!settings.enabled) return;

        Camera* mainCamera = Camera::GetMain();
        if (!mainCamera) return;

        PlayerManager* playerManager = SingletonManager::Instance().GetPlayerManager();
        if (!playerManager) return;

        PlayerController* localPlayer = playerManager->GetLocalPlayer();
        if (!localPlayer) return;

        Transform* mainCameraTransform = mainCamera->GetTransform();
        if (!mainCameraTransform) return;

        Quaternion camRot = mainCameraTransform->GetRotation();
        Vector3 camPos = mainCameraTransform->GetPosition();

        if (Misc::GetSettings().enableAntiAim || Misc::GetSettings().thirdPersonView) {
            camRot = Global::OverwrittenValues::g_CameraRotation;
            camPos = Global::OverwrittenValues::g_CameraPosition;
        }

        float fov = mainCamera->GetFOV();

        Team localTeam = localPlayer->m_Team;

        std::vector<PlayerController*> players = playerManager->GetPlayers();
        std::vector<EnemyData> frameEnemies;
        frameEnemies.reserve(players.size());

        for (PlayerController* player : players) {
            if (!player || !player->IsAlive()) continue;
            Team playerTeam = player->m_Team;
            if (playerTeam == localTeam) continue;

            Transform* transform = player->m_Transform;
            if (!transform) continue;

            Vector3 pos = transform->GetPosition();
            Vector3 screenPos = WorldToScreen_Internal(pos, camPos, camRot, fov);
            if (screenPos.z <= 0.01f) continue;

            PlayerCharacterView* charView = player->m_PlayerCharacterView;
            if (!charView) continue;

            BipedMap biped = charView->GetBipedMap();
            BipedMapToSend mapped = {};

#define MAP_BONE(dst, src) \
    dst = (src) ? WorldToScreen_Internal((src)->GetPosition(), camPos, camRot, fov) : Vector3(0, 0, -1)

            MAP_BONE(mapped.Head, biped.Head);
            MAP_BONE(mapped.Neck, biped.Neck);
            MAP_BONE(mapped.Spine, biped.Spine);
            MAP_BONE(mapped.Spine1, biped.Spine1);
            MAP_BONE(mapped.Spine2, biped.Spine2);
            MAP_BONE(mapped.LeftShoulder, biped.LeftShoulder);
            MAP_BONE(mapped.LeftUpperarm, biped.LeftUpperarm);
            MAP_BONE(mapped.LeftForearm, biped.LeftForearm);
            MAP_BONE(mapped.LeftHand, biped.LeftHand);
            MAP_BONE(mapped.RightShoulder, biped.RightShoulder);
            MAP_BONE(mapped.RightUpperarm, biped.RightUpperarm);
            MAP_BONE(mapped.RightForearm, biped.RightForearm);
            MAP_BONE(mapped.RightHand, biped.RightHand);
            MAP_BONE(mapped.Hip, biped.Hip);
            MAP_BONE(mapped.LeftUpLeg, biped.LeftUpLeg);
            MAP_BONE(mapped.LeftLeg, biped.LeftLeg);
            MAP_BONE(mapped.LeftFoot, biped.LeftFoot);
            MAP_BONE(mapped.LeftToeBase, biped.LeftToeBase);
            MAP_BONE(mapped.RightUpLeg, biped.RightUpLeg);
            MAP_BONE(mapped.RightLeg, biped.RightLeg);
            MAP_BONE(mapped.RightFoot, biped.RightFoot);
            MAP_BONE(mapped.RightToeBase, biped.RightToeBase);

#undef MAP_BONE

            mapped.ControllerPos = screenPos;

            PhotonPlayer* photonPlayer = player->m_PhotonPlayer;
            if (!photonPlayer) continue;

            Il2CppString* nickname = photonPlayer->GetNickName();
            if (!nickname || !nickname->c_str()) continue;

            WeaponryController* weaponryController = player->m_WeaponryController;
            if (!weaponryController) continue;

            WeaponController* currentWeaponController = weaponryController->m_CurrentWeapon;
            if (!currentWeaponController) continue;

            WeaponParameters* currentWeaponParameters = currentWeaponController->m_WeaponParameters;
            if (!currentWeaponParameters) continue;

            WeaponId currentWeaponId = currentWeaponParameters->_id;

            EnemyData data{};
            data.androidWidth = Screen::GetWidth();
            data.androidHeight = Screen::GetHeight();
            data.bipedMap = mapped;
            data.currentWeaponId = currentWeaponId;
            data.team = playerTeam;
            data.health = player->m_PhotonPlayer->GetHealth();
            data.maxHealth = playerTeam == Team::Tr ? PlayerControllerStaticFields::GetInstance()->MaxTrHealth : PlayerControllerStaticFields::GetInstance()->MaxCtHealth;

            if (weaponryController->m_DefuseKit || weaponryController->HasWeapon(WeaponId::Bomb)) {
                data.hasExplosiveRelatedItem = true;
                if (weaponryController->m_DefuseKit->m_State == DefuseKitState::Defusing ||
                    (currentWeaponId == WeaponId::Bomb && ((BombController*)(currentWeaponController))->m_State->curState == BombState::Planting))
                    data.isPlantingOrDefusing = true;
            }

            if (currentWeaponParameters->IsGun()) {
                GunController* gunController = (GunController*)(currentWeaponController);
                if (gunController && gunController->gunParameters && gunController->gunParameters->_ammunition) {
                    Ammunition* ammo = gunController->gunParameters->_ammunition;
                    if (ammo->m_MagazineCapacitySafe.hasValue)
                        data.ammo = ammo->m_MagazineCapacitySafe.value.GetValue();
                    data.maxAmmo = static_cast<int>(ammo->m_MagazineCapacity);
                }

                if (gunController->gunParameters->_sightType != SightType::Default) {
                    const StateSimple<AimingMode>* aimingMode = gunController->m_AimingMode;
                    const StateSimple<GunState>* gunState = gunController->m_GunState;
                    if (aimingMode && gunState) {
                        bool isAiming = (aimingMode->curState == AimingMode::Aiming || aimingMode->curState == AimingMode::StartingAiming);
                        bool isReloading = (gunState->curState == GunState::Reloading || gunState->curState == GunState::PreReload);
                        data.isScoped = isAiming && !isReloading;
                        data.isReloading = isReloading;
                    }
                }
            }
            data.hasHelmet = photonPlayer->HasHelmet();
            data.armor = photonPlayer->GetArmor();

            data.distanceToLocalPlayer = Vector3::Distance(localPlayer->m_Transform->GetPosition(), pos);

            strncpy(data.nickName, nickname->c_str(), sizeof(data.nickName) - 1);

            data.nickName[sizeof(data.nickName) - 1] = '\0';

            Compute3DBoxCorners(pos, camPos, camRot, fov, data.bipedMap.boxCorners);

            frameEnemies.push_back(data);
        }

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            enemies.swap(frameEnemies);
        }
    }

    void Init() override {
        // public interface [A-Z]{15} // TypeDefIndex: .*\n\{\n\t// Methods\n\n\t// RVA: -1 Offset: -1 Slot: 0\n\tpublic abstract void [A-Z]{15}\(ObjectOccludee
        /*
        to find OnOcclusionBecameInvisible name, search for this signature: "F0 48 2D E9 ? ? ? E2 ? ? ? E5 00 40 A0 E1 01 50 A0 E1 06 60 8F E0 ? ? ? E5 ? ? ? E3 ? ? ? 1A ? ? ? E5 ? ? ? E7 ? ? ? EB ? ? ? E5 ? ? ? E7 ? ? ? EB ? ? ? E5 ? ? ? E7 ? ? ? EB ? ? ? E3 ? ? ? E5 ? ? ? E3 ? ? ? 1A ? ? ? EB ? ? ? E5 ? ? ? E5"
        */
       
        constexpr uint8_t offset = 2 + 1; // slot + 1
        constexpr uint8_t playerControllerOnBecameInvisibleSlot = 61 + offset;
        constexpr uint8_t aimControllerOnBecameInvisibleSlot = 82 + offset;
        constexpr uint8_t mecanimControllerOnBecameInvisibleSlot = 76 + offset;
        constexpr uint8_t characterPlayerOnBecameInvisibleSlot = 5 + offset;

        // 0.35.3: absolute 64, relative 3
        // 0.35.2: absolute 62, relative 1

        Il2CppUtils::ReplaceVirtualMethod(oxorany("Axlebolt.Standoff.Player"), oxorany("PlayerController"),
            playerControllerOnBecameInvisibleSlot, playerControllerOnBecameInvisibleSlot - 2);
        Il2CppUtils::ReplaceVirtualMethod(oxorany("Axlebolt.Standoff.Player.Aim"), oxorany("AimController"),
            aimControllerOnBecameInvisibleSlot, aimControllerOnBecameInvisibleSlot - 2);
        Il2CppUtils::ReplaceVirtualMethod(oxorany("Axlebolt.Standoff.Player.Mecanim"), oxorany("MecanimController"),
            mecanimControllerOnBecameInvisibleSlot, mecanimControllerOnBecameInvisibleSlot - 2);
        Il2CppUtils::ReplaceVirtualMethod(oxorany("Axlebolt.Standoff.Player"), oxorany("CharacterPlayer"),
            characterPlayerOnBecameInvisibleSlot, characterPlayerOnBecameInvisibleSlot - 2);
    }

    void ApplySettings(const void* data) override {
        memcpy(&settings, data, sizeof(ESPSettings));
    }

    void GetCurrentSettings(void* out) override {
        memcpy(out, &settings, sizeof(ESPSettings));
    }

    static const ESPSettings& GetCurrentSettings() {
        return settings;
    }

    void GetData(std::vector<uint8_t>& out) override {
        std::vector<EnemyData> temp;

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            temp.swap(enemies); // Swap instead of clearing within the lock to avoid race conditions
        }

        size_t count = temp.size();

        out.resize(sizeof(uint32_t) + count * sizeof(EnemyData));

        if (!out.empty()) {
            memcpy(out.data(), &count, sizeof(uint32_t));

            if (count > 0) {
                memcpy(out.data() + sizeof(uint32_t), temp.data(), count * sizeof(EnemyData));
            }
        }
    }
private:
    static inline ESPSettings settings;
    static inline std::vector<EnemyData> enemies;
    static inline std::mutex dataMutex;
};