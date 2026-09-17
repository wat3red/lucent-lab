#pragma once
#include "gun_controls.h"

#define NOISE_MARKER_LAYER 20
#define INVIS_CUBE_LAYER 28
#define INVIS_CUBE_LAYER2 7

enum class PreferredBone : int {
    Head = 0,
    Body = 1,
    Limbs = 2
};

enum class TargetSelectionMode : int {
    FOV = 0,
    Distance = 1,
    LowestHealth = 2
};

enum class HitboxSelectionMode : int {
    Priority = 0,   // Приоритет по группам (голова, тело, конечности)
    Closest = 1,    // Ближайшая к прицелу кость
    HighestDamage = 2 // Максимальный урон (Голова > Тело)
};

enum class VisibilityCheckMode : int {
    Multipoint = 0,
    CenterOnly = 1
};

static const int MAX_DEBUG_POINTS = 128;
#pragma pack(push, 1)
struct AimSettings {
    bool enabled;
    bool disableVisibilityCheck;
    bool autoFire;
    bool drawMarker;
    float fov;

    // Modes
    TargetSelectionMode targetSelectionMode;
    HitboxSelectionMode hitboxSelectionMode;
    VisibilityCheckMode visibilityCheckMode;

    // Using this in Priority mode
    PreferredBone preferredBone;
};
struct EmulatorData {
    int screenWidth;
    int screenHeight;
    int debugPointCount;
    Vector2 currentTarget;
    Vector2 debugPoints[MAX_DEBUG_POINTS];
};
#pragma pack(pop)


struct TargetInfo {
    PlayerController* controller = nullptr;
    float fovDistance = FLT_MAX;
    float distance3D = FLT_MAX;
    float health = 100.0f;
};

class Aim : public Function {
public:
    uint32_t GetId() const override { return 3; }
    size_t GetSettingsSize() const override { return sizeof(AimSettings); }
private:
    // Функция для классификации костей по группам
    static PreferredBone GetBoneGroup(BoneID boneId) {
        switch (boneId) {
        case BoneID::Head:
            return PreferredBone::Head;
        case BoneID::Spine1:
        case BoneID::Spine2:
        case BoneID::Spine:
        case BoneID::Hip:
        case BoneID::Neck:
            return PreferredBone::Body;
        default:
            return PreferredBone::Limbs;
        }
    }

    // Список костей для сканирования в порядке приоритета (от более важных к менее)
    static inline const std::vector<BoneID> TargetableBones = {
        BoneID::Head, BoneID::Neck, BoneID::Spine1, BoneID::Spine2, BoneID::Spine, BoneID::Hip,
        BoneID::LeftUpperarm, BoneID::LeftForearm, BoneID::LeftHand, BoneID::LeftShoulder, BoneID::RightShoulder,
        BoneID::RightUpperarm, BoneID::RightForearm, BoneID::RightHand, BoneID::LeftUpLeg, BoneID::LeftLeg, BoneID::LeftFoot,
        BoneID::RightUpLeg, BoneID::RightLeg, BoneID::RightFoot, BoneID::LeftToeBase, BoneID::RightToeBase
    };

public:
    static inline bool (*RayCast_orig)(void*& scene, Ray* ray, float maxDistance, RaycastHit* hit, int32_t layer, int32_t trigger);
    static bool IsVisible(Vector3 start, Vector3 end) noexcept {
        if (RayCast_orig == nullptr) {
            RayCast_orig = (decltype(RayCast_orig))il2cpp_resolve_icall(
                oxorany("UnityEngine.PhysicsScene::Internal_Raycast_Injected(UnityEngine.PhysicsScene&,UnityEngine.Ray&,"
                "System.Single,UnityEngine.RaycastHit&,System.Int32,UnityEngine.QueryTriggerInteraction)"));
        }

        void* scene = nullptr;
        Ray ray;
        ray.m_VecOrigin = start;
        Vector3 direction = end - start;
        float distance = Vector3::Magnitude(direction);
        if (distance == 0.f) return true;
        ray.m_VecDirection = direction / distance;

        RaycastHit hit;
        // Маска для проверки препятствий: 0 = Default (стены), 11 = RemotePlayer (другие игроки)
        int layerMask = (1 << 0) | (1 << 11);

        bool result = RayCast_orig(scene, &ray, distance * 0.99f, &hit, layerMask, 0);

        return !result;
    };

    // Ваш хук RayCast_hook (без изменений)
    static bool RayCast_hook(void*& scene, Ray* ray, float maxDistance, RaycastHit* hit, int layer, int trigger) {
        if (RayCast_orig == nullptr) {
            RayCast_orig = (decltype(RayCast_orig))il2cpp_resolve_icall(
                oxorany("UnityEngine.PhysicsScene::Internal_Raycast_Injected(UnityEngine.PhysicsScene&,UnityEngine.Ray&,"
                "System.Single,UnityEngine.RaycastHit&,System.Int32,UnityEngine.QueryTriggerInteraction)"));
        }

        if (ray->m_VecDirection == Vector3(0, -1, 0) || maxDistance != 1000.f) {
            return RayCast_orig(scene, ray, maxDistance, hit, layer, trigger);
        }

        if (!settings.autoFire && !GunControls::GetSettings().rapidFire && !settings.drawMarker)
            ProcessPlayers();

        if (!settings.enabled) {
            return RayCast_orig(scene, ray, maxDistance, hit, layer, trigger);
        }

        if (currentTarget != Vector3(-1, -1, -1) && Camera::GetMain() != nullptr) {
            Vector3 origin = Camera::GetMain()->GetTransform()->GetPosition();
            if (Misc::GetSettings().enableAntiAim || Misc::GetSettings().thirdPersonView) {
                origin = Global::OverwrittenValues::g_CameraPosition;
            }

            ray->m_VecOrigin = origin;
            ray->m_VecDirection = Vector3::Normalize(currentTarget - origin);
        }

        int filteredLayer = layer & ~((1 << NOISE_MARKER_LAYER) | (1 << INVIS_CUBE_LAYER) | (1 << INVIS_CUBE_LAYER2));
        return RayCast_orig(scene, ray, maxDistance, hit, filteredLayer, trigger);
    }

    static inline std::vector<Vector3> debugMultipoints;
    static std::vector<Vector3> GenerateMultipoints(PlayerHitbox* hitbox) {
        std::vector<Vector3> points;
        if (!hitbox || !hitbox->_hitboxConfig) {
            return points;
        }

        PlayerHitboxConfig_HitboxConfig* config = hitbox->_hitboxConfig;
        Transform* hitboxTransform = hitbox->GetTransform();
        if (!hitboxTransform) {
            return points;
        }

        if (config->hitboxType == HitboxType::Capsule) {
            std::vector<Vector3> localPoints;
            Vector3 center = config->center;
            float radius = config->radius;
            // Убедимся, что высота капсулы не меньше ее диаметра
            float height = std::max(radius * 2.0f, config->height);
            float halfHeight = (height / 2.0f) - radius;

            Vector3 upVector;

            // 1. Определяем локальный вектор oxorany("вверх") для капсулы
            if (config->direction == 0) {      // Вытянута по оси X
                upVector = Vector3(1, 0, 0);
            }
            else if (config->direction == 2) { // Вытянута по оси Z
                upVector = Vector3(0, 0, 1);
            }
            else {                           // Стандарт, вытянута по оси Y
                upVector = Vector3(0, 1, 0);
            }

            // 2. Вычисляем центры верхней и нижней сфер в ЛОКАЛЬНЫХ координатах
            Vector3 p1 = center + upVector * halfHeight;
            Vector3 p2 = center - upVector * halfHeight;

            // 3. Генерируем все точки в ЛОКАЛЬНОМ пространстве
            localPoints.push_back(center);                   // Центр всей капсулы
            localPoints.push_back(p1);                       // Центр верхней сферы
            localPoints.push_back(p2);                       // Центр нижней сферы
            localPoints.push_back(p1 + upVector * radius);   // Самая верхняя точка капсулы (макушка)
            localPoints.push_back(p2 - upVector * radius);   // Самая нижняя точка капсулы

            // 4. Трансформируем ВСЕ локальные точки в мировые
            for (const auto& local_p : localPoints) {
                points.push_back(hitboxTransform->TransformPoint(local_p));
            }

        }
        else if (config->hitboxType == HitboxType::Box) {
            // Логика для бокса у вас уже была правильной, так как она изначально
            // вычисляла все в локальных координатах. Оставляем ее без изменений.
            Vector3 center = config->center;
            Vector3 size = config->size / 2.0f; // Полу-размеры

            std::vector<Vector3> localPoints;
            localPoints.push_back(center);
            localPoints.push_back(center + Vector3(size.x, size.y, size.z));
            localPoints.push_back(center + Vector3(size.x, size.y, -size.z));
            localPoints.push_back(center + Vector3(size.x, -size.y, size.z));
            localPoints.push_back(center + Vector3(size.x, -size.y, -size.z));
            localPoints.push_back(center + Vector3(-size.x, size.y, size.z));
            localPoints.push_back(center + Vector3(-size.x, size.y, -size.z));
            localPoints.push_back(center + Vector3(-size.x, -size.y, size.z));
            localPoints.push_back(center + Vector3(-size.x, -size.y, -size.z));

            for (const auto& p : localPoints) {
                points.push_back(hitboxTransform->TransformPoint(p));
            }
        }

        return points;
    }

    static void ProcessPlayers() {
        PlayerManager* playerManager = SingletonManager::Instance().GetPlayerManager();
        if (!playerManager) return;
        PlayerController* localPlayerController = playerManager->GetLocalPlayer();
        if (!localPlayerController) return;

        if (!settings.enabled) {
            targetValid = false;
            currentTarget = Vector3(-1, -1, -1);
            emulatorData.currentTarget = Vector2(-1.f, -1.f);
            return;
        }

        screenResolution = iVector2(Screen::GetWidth(), Screen::GetHeight());

        Camera* mainCamera = Camera::GetMain();
        if (!mainCamera) return;
        Transform* camTransform = mainCamera->GetTransform();
        if (!camTransform) return;

        Vector3 camPos = camTransform->GetPosition();
        Quaternion camRot = camTransform->GetRotation();

        if (Misc::GetSettings().enableAntiAim || Misc::GetSettings().thirdPersonView) {
            camRot = Global::OverwrittenValues::g_CameraRotation;
            camPos = Global::OverwrittenValues::g_CameraPosition;
        }

        float camFOV = mainCamera->GetFOV();
        Vector2 screenCenter = { (float)Screen::GetWidth() / 2, (float)Screen::GetHeight() / 2 };
        debugMultipoints.clear();

        // --- ЭТАП 1: Сбор всех потенциальных целей ---
        std::vector<TargetInfo> potentialTargets;
        std::vector<PlayerController*> players = playerManager->GetPlayers();
        for (PlayerController* enemy : players) {
            if (!enemy) continue;
            PhotonPlayer* photon = enemy->m_PhotonPlayer;

            if (!enemy || !photon || enemy == localPlayerController ||
                !enemy->IsAlive() /*|| photon->IsUntouchable() */ || enemy->m_Team == localPlayerController->m_Team) {
                continue;
            }

            // Используем позицию таза для базовых расчетов
            Transform* hipTransform = enemy->m_PlayerCharacterView->GetBipedMap().Hip;
            if (!hipTransform) continue;

            Vector3 enemyPos = hipTransform->GetPosition();
            Vector3 screenPos = WorldToScreen_Internal(enemyPos, camPos, camRot, camFOV);
            if (screenPos.z < 0) continue;

            float fovDist = Vector2::Distance(screenPos, screenCenter);
            // if (fovDist > settings.fov) continue;

            TargetInfo info;
            info.controller = enemy;
            info.fovDistance = fovDist;
            info.distance3D = Vector3::Distance(camPos, enemyPos);
            info.health = photon->GetHealth();
            potentialTargets.push_back(info);
        }

        if (potentialTargets.empty()) {
            targetValid = false;
            currentTarget = Vector3(-1, -1, -1);
            return;
        }

        // --- ЭТАП 2: Сортировка целей согласно настройкам ---
        std::sort(potentialTargets.begin(), potentialTargets.end(), [&](const TargetInfo& a, const TargetInfo& b) {
            switch (settings.targetSelectionMode) {
            case TargetSelectionMode::Distance:
                return a.distance3D < b.distance3D;
            case TargetSelectionMode::LowestHealth:
                return a.health < b.health;
            case TargetSelectionMode::FOV:
            default:
                return a.fovDistance < b.fovDistance;
            }
            });

        // --- ЭТАП 3: Поиск валидной точки для выстрела ---
        Vector3 finalTargetPoint = Vector3(-1, -1, -1);
        for (const auto& target : potentialTargets) {
            PlayerHitController* hitController = target.controller->m_PlayerHitController;
            if (!hitController) continue;
            Dictionary<BoneID, PlayerHitbox*>* hitboxDict = hitController->_playerHitboxes;
            if (!hitboxDict) continue;

            Vector3 bestBonePoint = Vector3(-1, -1, -1);
            float bestBoneFovDist = FLT_MAX; // Для режима oxorany("Closest")

            // --- Цикл по костям для выбранного игрока ---
            for (BoneID boneId : TargetableBones) {
                PlayerHitbox* hitbox = hitboxDict->GetValue((BoneID)boneId);
                if (!hitbox) continue;

                std::vector<Vector3> multipoints = GenerateMultipoints(hitbox);
                if (multipoints.empty()) continue;

                // if (settings.visibilityCheckMode == VisibilityCheckMode::CenterOnly) {
                multipoints = { multipoints[0] }; // Оставляем только центральную точку
                // }

                for (const Vector3& point : multipoints) {
                    debugMultipoints.push_back(point);
                    Vector3 screenPos = WorldToScreen_Internal(point, camPos, camRot, camFOV);
                    float distToCenter = Vector2::Distance(screenPos, screenCenter);
                    if ((settings.disableVisibilityCheck ? true : IsVisible(camPos, point)) && settings.fov >= distToCenter) {
                        switch (settings.hitboxSelectionMode) {
                        case HitboxSelectionMode::Closest: {
                            if (distToCenter < bestBoneFovDist) {
                                bestBoneFovDist = distToCenter;
                                bestBonePoint = point;
                            }
                            break;
                        }
                        case HitboxSelectionMode::HighestDamage: {
                            if (GetBoneGroup(boneId) == PreferredBone::Head) {
                                finalTargetPoint = point;
                                goto found_target;
                            }
                            if (GetBoneGroup(boneId) == PreferredBone::Body && bestBonePoint.x == -1.f) {
                                bestBonePoint = point;
                            }
                            break;
                        }
                        case HitboxSelectionMode::Priority:
                        default: {
                            if (GetBoneGroup(boneId) == settings.preferredBone) {
                                finalTargetPoint = point;
                                goto found_target;
                            }
                            if (bestBonePoint.x == -1.f) {
                                bestBonePoint = point;
                            }
                            break;
                        }
                        }
                    }
                }
            }

            // Если основной цикл не нашел точку (для режимов Priority/HighestDamage), используем запасную
            if (bestBonePoint.x != -1.f) {
                finalTargetPoint = bestBonePoint;
                goto found_target;
            }
        }

    found_target:
        // --- ЭТАП 4: Обновление состояния ---
        if (finalTargetPoint.x != -1.f) {
            currentTarget = finalTargetPoint;
            targetValid = true;
        }
        else {
            currentTarget = Vector3(-1, -1, -1);
            targetValid = false;
        }

        // Обновляем данные для эмулятора
        if (targetValid) {
            emulatorData.currentTarget = (Vector2)WorldToScreen_Internal(currentTarget, camPos, camRot, camFOV);
        }
        else {
            emulatorData.currentTarget = Vector2(-1.f, -1.f);
        }
    }

    static void CreateMoveCallback(Il2CppObject* obj, PlayerInputs* cmd) {
        if (!settings.enabled || !settings.autoFire)
            return;

        cmd->m_bFire |= (targetValid && currentTarget != Vector3(-1, -1, -1));
    }

    static void ProcessPlayersGameUpdate(GameController*) {
        if (settings.autoFire || GunControls::GetSettings().rapidFire || settings.drawMarker)
            ProcessPlayers();
    }

    void Init() override {
        *(void**)(g_LibUnityBase + RAYCAST_HOOK_ADDRESS) = (void*)&RayCast_hook;

        Global::Hooks::AddGameUpdateCallback(ProcessPlayersGameUpdate);
        Global::Hooks::AddCreateMoveCallback(CreateMoveCallback);
    }

    void GetData(std::vector<uint8_t>& out) override {
        std::lock_guard<std::mutex> lock(dataMutex);

        // Camera* mainCamera = Camera::GetMain();
        // if (!mainCamera) return;
        // Transform* camTransform = mainCamera->GetTransform();
        // if (!camTransform) return;

        // Vector3 camPos = camTransform->GetPosition();
        // Quaternion camRot = camTransform->GetRotation();

        // if (Misc::GetSettings().enableAntiAim || Misc::GetSettings().thirdPersonView) {
        //     camRot = Global::OverwrittenValues::g_CameraRotation;
        //     camPos = Global::OverwrittenValues::g_CameraPosition;
        // }

        // float camFOV = mainCamera->GetFOV();

        // emulatorData.debugPointCount = std::min((int)debugMultipoints.size(), MAX_DEBUG_POINTS);
        // for (int i = 0; i < emulatorData.debugPointCount; ++i) {
        //     emulatorData.debugPoints[i] = (Vector2)WorldToScreen_Internal(debugMultipoints[i], camPos, camRot, camFOV);
        // }

        emulatorData.screenWidth = screenResolution.x;
        emulatorData.screenHeight = screenResolution.y;

        out.assign((uint8_t*)&emulatorData, (uint8_t*)&emulatorData + sizeof(EmulatorData));
    }

    void ApplySettings(const void* data) override {
        memcpy(&settings, data, sizeof(AimSettings));
    }

    void GetCurrentSettings(void* out) override {
        memcpy(out, &settings, sizeof(AimSettings));
    }

private:
    static inline Vector3 currentTarget = Vector3(-1, -1, -1);
    static inline bool targetValid = false;
    static inline iVector2 screenResolution;

    static inline EmulatorData emulatorData;
    static inline AimSettings settings;
    std::mutex dataMutex;
};