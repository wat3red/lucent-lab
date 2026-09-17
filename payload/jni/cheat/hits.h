#pragma once

#pragma pack(push, 1)
struct HitsSettings {
    bool hitlogs;
};

struct HitLog {
    int androidHeight;
    int androidWidth;
    float distanceToLocalPlayer;
    Vector3 positionOnScreen;
    int32_t damage;
    char nickname[32];
    char boneName[32];
};
#pragma pack(pop)

class Hits : public Function {
public:
    uint32_t GetId() const override { return 10; }
    size_t GetSettingsSize() const override { return sizeof(HitsSettings); }
    static HitsSettings GetSettings() {
        return settings;
    }

    void GetData(std::vector<uint8_t>& out) override {
        std::lock_guard<std::mutex> lock(dataMutex);

        size_t count = hits.size();
        out.resize(sizeof(uint32_t) + count * sizeof(HitLog));

        if (!out.empty()) {
            memcpy(out.data(), &count, sizeof(uint32_t));
            if (count > 0) {
                memcpy(out.data() + sizeof(uint32_t), hits.data(), count * sizeof(HitLog));
            }
        }

        hits.clear();
    }

    static void LocalPlayerHitCallback(PlayerHitController* hitController, PlayerController* player, HitData* hitData) {
        if (!settings.hitlogs || !player || !hitController || !hitData || !hitData->m_Hits) return;

        PhotonPlayer* photonPlayer = player->m_PhotonPlayer;
        if (!photonPlayer) return;

        Il2CppString* nickStr = photonPlayer->GetNickName();
        if (!nickStr) return;

        PlayerManager* playerManager = SingletonManager::Instance().GetPlayerManager();
        if (!playerManager) return;

        PlayerController* localPlayer = playerManager->GetLocalPlayer();
        if (!localPlayer || !localPlayer->IsAlive()) return;

        Camera* cam = Camera::GetMain();
        if (!cam) return;

        std::vector<HitLog> tempHits;
        for (int i = 0; i < hitData->m_Hits->size; ++i) {
            BulletHitData* hit = hitData->m_Hits->items[i];
            if (!hit) continue;

            // LOGD("name: %s, hit %d: Point: (%f, %f, %f), Damage: %d, Impulse: %f, Bone: %d, Penetrated: %s",
            //     nickStr->c_str(), i, hit->m_Point.x, hit->m_Point.y, hit->m_Point.z,
            //     hit->m_Damage, hit->m_Impulse, hit->m_Bone,
            //     hit->m_Penetrated ? oxorany("true") : oxorany("false"));

            HitLog log{};
            log.androidHeight = Screen::GetHeight();
            log.androidWidth = Screen::GetWidth();
            log.distanceToLocalPlayer = Vector3::Magnitude(hit->m_Point - localPlayer->m_Transform->GetPosition());
            log.positionOnScreen = cam->WorldToScreen(hit->m_Point);
            log.damage = hit->m_Damage;

            const char* boneName = GetBoneName(hit->m_Bone);
            if (boneName)
                strncpy(log.boneName, boneName, sizeof(log.boneName));

            strncpy(log.nickname, nickStr->c_str(), sizeof(log.nickname));

            // Ensure null termination
            log.boneName[sizeof(log.boneName) - 1] = '\0';
            log.nickname[sizeof(log.nickname) - 1] = '\0';

            tempHits.push_back(log);
        }

        {
            std::lock_guard<std::mutex> lock(dataMutex);
            hits.insert(hits.end(), tempHits.begin(), tempHits.end());
        }
    }

    void Init() override {
        Global::Hooks::AddLocalPlayerHitCallback(Hits::LocalPlayerHitCallback);
    }

    void ApplySettings(const void* data) override {
        memcpy(&settings, data, sizeof(HitsSettings));
    }
    void GetCurrentSettings(void* out) override {
        memcpy(out, &settings, sizeof(HitsSettings));
    }
private:
    static inline HitsSettings settings;
    static inline std::mutex dataMutex;
    static inline std::vector<HitLog> hits;
};