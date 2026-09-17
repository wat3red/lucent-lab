#pragma once

#pragma pack(push, 1)
enum class GrenadeType : uint8_t {
    HE = 91,
    Smoke = 92,
    Flashbang = 93,
    Molotov = 94,
    Incendiary = 95
};

struct GrenadeControllerSettings {
    bool enabled;
    bool enableGrenadePractice;

    int heDamage;
    bool heShowIndicator;

    int incendiaryDamage;
    bool incendiaryShowIndicator;

    bool disableFlashbang;
    bool flashbangShowIndicator;
    
    bool disableSmoke;
    bool smokeShowIndicator;
};

struct GrenadeData {
    Vector3 screenPosition;
    char name[64];
    GrenadeType type;

    int androidWidth;
    int androidHeight;

    float distanceToLocalPlayer;

    float remainingTime; // осталось до взрыва/окончания действия
    float totalTime;     // общее время действия гранаты
};
#pragma pack(pop)

class GrenadeController : public Function {
public:
    uint32_t GetId() const override { return 8; }
    size_t GetSettingsSize() const override { return sizeof(GrenadeControllerSettings); }

    void Init() override {
        Global::Hooks::AddGameUpdateCallback(GameUpdateCallback);

        Il2CppClass* flashbangEffectControllerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Inventory.Grenade"), oxorany("FlashbangEffectController"));
        Il2CppUtils::MethodHook(flashbangEffectControllerKlass, oxorany("Update"), FlashbangEffectController_Update_hook, &FlashbangEffectController_Update_orig);

        Il2CppClass* smokeEffectControllerKlass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Inventory.Grenade"), oxorany("SmokeEffectController"));
        Il2CppUtils::MethodHook(smokeEffectControllerKlass, oxorany("Update"), SmokeEffectController_Update_hook, &SmokeEffectController_Update_orig);
    }

    static inline void (*FlashbangEffectController_Update_orig)(void* _this);
    static void FlashbangEffectController_Update_hook(void* _this) {
        FlashbangEffectController_Update_orig(_this);
        if (settings.disableFlashbang && settings.enabled)
            FlashbangEffectController_ResetEffect(_this);
    }

    static inline void (*SmokeEffectController_Update_orig)(void* _this);
    static void SmokeEffectController_Update_hook(void* _this) {
        if (!settings.disableSmoke || !settings.enabled) {
            SmokeEffectController_Update_orig(_this);
            return;
        }

        Il2CppArray<GameObject*>* gameObjects = GameObject::GetAllGameObjects();
        if (!gameObjects) return;

        for (int i = 0; i < gameObjects->size; ++i) {
            GameObject* gameObject = gameObjects->items[i];
            if (!gameObject) continue;

            std::string name = gameObject->GetName()->c_str();
            for (auto& c : name)
                c = tolower(c);

            if (name.find(oxorany("smoke")) != std::string::npos)
            {
                gameObject->SetActive(false);
            }
        }
    }

    void ApplySettings(const void* data) override {
        memcpy(&settings, data, sizeof(GrenadeControllerSettings));
    }

    void GetCurrentSettings(void* out) override {
        memcpy(out, &settings, sizeof(GrenadeControllerSettings));
    }

    struct PersistentGrenade {
        int id;                    // уникальный ID гранаты
        Vector3 lastKnownPosition; // последняя известная позиция
        GrenadeType type;
        char name[64];
        float spawnTime;           // время создания гранаты
        float detonationTime;      // время детонации (для HE и Flash)
        float detonationMoment; // фактическое время детонации

        float totalEffectTime;     // общее время действия эффекта
        bool hasDetonated;         // взорвалась ли граната
        bool isFromDroppedList;    // есть ли еще в droppedGrenadeControllers
    };

    static inline std::vector<PersistentGrenade> persistentGrenades;
    static inline int nextGrenadeId = 1;

    static void GameUpdateCallback(GameController* _this) {
        if (_this->m_Settings) {
            _this->m_Settings->m_GrenadePracticeType = settings.enableGrenadePractice;
        }
    }

    // Получить время полного действия гранаты (включая эффекты после взрыва)
    float GetTotalGrenadeEffectTime(WeaponId grenadeId) {
        switch (grenadeId) {
        case WeaponId::GrenadeSmoke:
            return 20.f;
        case WeaponId::GrenadeMolotov:
        case WeaponId::GrenadeIncendiary:
            return 8.f;
        default:
            return 0.f;
        }
    }

    void UpdatePersistentGrenadesList() {
        GrenadeManager* grenadeManager = SingletonManager::Instance().GetGrenadeManager();
        if (!grenadeManager) return;

        Dictionary<int, DroppedGrenadeController*>* droppedGrenadeControllers = grenadeManager->GetDroppedGrenades();
        if (!droppedGrenadeControllers) return;

        float currentTime = Time::GetTime();

        // Помечаем все гранаты как не найденные в droppedList
        for (auto& persistentGrenade : persistentGrenades) {
            persistentGrenade.isFromDroppedList = false;
        }

        // Обрабатываем гранаты из droppedGrenadeControllers
        for (int i = 0; i < droppedGrenadeControllers->count; i++) {
            auto entry = droppedGrenadeControllers->entries->items[i];
            DroppedGrenadeController* droppedGrenadeController = entry.value;

            if (!droppedGrenadeController) continue;

            GrenadeParameters* grenadeParameters = droppedGrenadeController->m_GrenadeParameters;
            if (!grenadeParameters) continue;

            grenadeParameters->_damageSafe.value.SetValue(settings.heDamage);

            Transform* grenadeTransform = droppedGrenadeController->m_GrenadeTransform;
            if (!grenadeTransform) continue;

            WeaponId grenadeId = (WeaponId)grenadeParameters->_id;
            int droppedId = entry.key;

            // Ищем эту гранату в нашем списке
            auto it = std::find_if(persistentGrenades.begin(), persistentGrenades.end(),
                [droppedId](const PersistentGrenade& g) { return g.id == droppedId; });

            if (it != persistentGrenades.end()) {
                // Граната уже есть в списке - обновляем позицию
                it->lastKnownPosition = grenadeTransform->GetPosition();
                it->isFromDroppedList = true;
            }
            else {
                // Новая граната - добавляем в список
                PersistentGrenade newGrenade;
                newGrenade.id = droppedId;
                newGrenade.lastKnownPosition = grenadeTransform->GetPosition();
                newGrenade.type = (GrenadeType)grenadeId;
                newGrenade.spawnTime = droppedGrenadeController->m_SpawnTime;
                newGrenade.detonationTime = grenadeParameters->_detonationDuration;
                newGrenade.totalEffectTime = newGrenade.type == GrenadeType::Flashbang || newGrenade.type == GrenadeType::HE ?
                    grenadeParameters->_detonationDuration : GetTotalGrenadeEffectTime(grenadeId);
                newGrenade.hasDetonated = false;
                newGrenade.isFromDroppedList = true;

                Il2CppString* grenadeName = grenadeParameters->_displayName;
                if (grenadeName && grenadeName->c_str()) {
                    strncpy(newGrenade.name, grenadeName->c_str(), sizeof(newGrenade.name) - 1);
                    newGrenade.name[sizeof(newGrenade.name) - 1] = '\0';
                }
                else {
                    strcpy(newGrenade.name, oxorany("Unknown Grenade"));
                }

                persistentGrenades.push_back(newGrenade);
            }
        }

        // Помечаем гранаты как взорвавшиеся, если их больше нет в droppedList
        for (auto& persistentGrenade : persistentGrenades) {
            if (!persistentGrenade.isFromDroppedList && !persistentGrenade.hasDetonated) {
                if (persistentGrenade.type == GrenadeType::Flashbang || persistentGrenade.type == GrenadeType::HE) {
                    float elapsed = currentTime - persistentGrenade.spawnTime;
                    if (elapsed >= persistentGrenade.detonationTime) {
                        persistentGrenade.hasDetonated = true;
                        persistentGrenade.detonationMoment = currentTime; // фиксируем момент взрыва
                        // LOGD("Grenade %d detonated after %f seconds", persistentGrenade.id, elapsed);
                        continue;
                    }
                }
                else {
                    persistentGrenade.hasDetonated = true;
                    persistentGrenade.detonationMoment = currentTime; // фиксируем момент взрыва
                }
            }
        }

        persistentGrenades.erase(
            std::remove_if(persistentGrenades.begin(), persistentGrenades.end(),
                [currentTime](const PersistentGrenade& grenade) {
                    if (!grenade.hasDetonated)
                        return false; // не удаляем, пока не взорвалась

                    float elapsed;
                    if (grenade.type == GrenadeType::Smoke ||
                        grenade.type == GrenadeType::Molotov ||
                        grenade.type == GrenadeType::Incendiary) {
                        elapsed = currentTime - grenade.detonationMoment; // считаем от момента детонации
                    }
                    else {
                        elapsed = currentTime - grenade.spawnTime; // для HE/Flash — от спавна
                    }
                    return elapsed >= grenade.totalEffectTime;
                }),
            persistentGrenades.end()
        );

    }

    void ProcessGrenadesIndicators() {
        UpdatePersistentGrenadesList();

        std::vector<GrenadeData> frameGrenades;
        float currentTime = Time::GetTime();

        Camera* mainCamera = Camera::GetMain();
        if (!mainCamera) return;

        Transform* mainCameraTransform = mainCamera->GetTransform();
        if (!mainCameraTransform) return;

        float fov = mainCamera->GetFOV();
        Quaternion camRot = mainCameraTransform->GetRotation();
        Vector3 camPos = mainCameraTransform->GetPosition();

        for (const auto& persistentGrenade : persistentGrenades) {
            // Проверяем, нужно ли показывать индикатор для этого типа гранаты
            bool shouldShow = false;
            switch ((WeaponId)persistentGrenade.type) {
            case WeaponId::GrenadeFlash:
                shouldShow = settings.flashbangShowIndicator;
                break;
            case WeaponId::GrenadeHE:
                shouldShow = settings.heShowIndicator;
                break;
            case WeaponId::GrenadeSmoke:
                shouldShow = settings.smokeShowIndicator;
                break;
            case WeaponId::GrenadeMolotov:
            case WeaponId::GrenadeIncendiary:
                shouldShow = settings.incendiaryShowIndicator;
                break;
            default: shouldShow = false;
            }

            if (!shouldShow) continue;

            float remaining;
            if (persistentGrenade.type == GrenadeType::Smoke ||
                persistentGrenade.type == GrenadeType::Molotov ||
                persistentGrenade.type == GrenadeType::Incendiary)
            {
                if (!persistentGrenade.hasDetonated) {
                    remaining = 0.f; // пока не взорвалась – ждем
                }
                else {
                    float effectElapsed = currentTime - persistentGrenade.detonationMoment;
                    remaining = persistentGrenade.totalEffectTime - effectElapsed;
                }
            }
            else {
                float totalElapsed = currentTime - persistentGrenade.spawnTime;
                remaining = persistentGrenade.totalEffectTime - totalElapsed;
            }

            if (remaining < 0) remaining = 0;

            GrenadeData grenadeData;
            grenadeData.androidWidth = Screen::GetWidth();
            grenadeData.androidHeight = Screen::GetHeight();
            grenadeData.remainingTime = remaining;
            grenadeData.totalTime = persistentGrenade.totalEffectTime;
            grenadeData.type = persistentGrenade.type;
            grenadeData.distanceToLocalPlayer = Vector3::Magnitude(persistentGrenade.lastKnownPosition - camPos);
            // grenadeData.isActive = !persistentGrenade.hasDetonated || (persistentGrenade.hasDetonated && remaining > 0);

            strncpy(grenadeData.name, persistentGrenade.name, sizeof(grenadeData.name) - 1);
            grenadeData.name[sizeof(grenadeData.name) - 1] = '\0';

            grenadeData.screenPosition = WorldToScreen_Internal(
                persistentGrenade.lastKnownPosition, camPos, camRot, fov);

            frameGrenades.push_back(grenadeData);

            // LOGD("Persistent Grenade %d (%s): remaining=%f, total=%f, detonated=%d",
                // persistentGrenade.id, grenadeData.name, remaining,
                // persistentGrenade.totalEffectTime, persistentGrenade.hasDetonated);
        }

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            grenades.swap(frameGrenades);
        }
    }

    void ProcessIncendiaryGrenades() {
        GrenadeEffectEmitter* grenadeEffectEmitter = SingletonManager::Instance().GetGrenadeEffectEmitter();
        if (!grenadeEffectEmitter) return;
        Dictionary<WeaponId, GrenadeEffectParameters*>* grenadeEffectParametersMappings = grenadeEffectEmitter->GetGrenadeEffectParameters();
        if (!grenadeEffectParametersMappings) return;

        for (int i = 0; i < grenadeEffectParametersMappings->count; i++) {
            auto entry = grenadeEffectParametersMappings->entries->items[i];
            GrenadeEffectParameters* grenadeEffectParameters = entry.value;

            if (!grenadeEffectParameters) continue;

            switch (entry.key) {
            case WeaponId::GrenadeMolotov:
                grenadeEffectParameters->_mollyEffect->_MaxDamage = settings.incendiaryDamage;
                break;
            case WeaponId::GrenadeIncendiary:
                grenadeEffectParameters->_mollyEffect->_MaxDamage = settings.incendiaryDamage;
                break;
            default:
                break;
            };
        }
    }

    void Update() override {
        if (!settings.enabled) return;

        ProcessIncendiaryGrenades();
        ProcessGrenadesIndicators();
    }

    void GetData(std::vector<uint8_t>& out) override {
        std::vector<GrenadeData> temp;

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            temp.swap(grenades);
        }

        size_t count = temp.size();
        out.resize(sizeof(uint32_t) + count * sizeof(GrenadeData));

        if (!out.empty()) {
            memcpy(out.data(), &count, sizeof(uint32_t));

            if (count > 0) {
                memcpy(out.data() + sizeof(uint32_t), temp.data(), count * sizeof(GrenadeData));
            }
        }
    }

private:
    static inline GrenadeControllerSettings settings;
    static inline std::vector<GrenadeData> grenades;
    static inline std::mutex dataMutex;
};