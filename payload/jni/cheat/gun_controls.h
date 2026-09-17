#pragma once

#pragma pack(push, 1)
struct GunControlsSettings {
    bool enable;

    bool rapidFire;
    bool infinityAmmo;
    bool disableRecoil;
    bool accurateShooting;
    bool damageHack;
    bool wallShot;
};
#pragma pack(pop)

class GunControls : public Function {
public:
    uint32_t GetId() const override { return 1; }
    size_t GetSettingsSize() const override { return sizeof(GunControlsSettings); }
    static GunControlsSettings GetSettings() {
        return settings;
    }
    void Init() override {
        Global::Hooks::AddPlayerUpdateCallback(GunControls::PlayerUpdateCallback);
    }

    static void PlayerUpdateCallback(PlayerController* _this) {
        // Timer timer(oxorany("GunControls::PlayerUpdateCallback"));
        PlayerManager* playerManager = SingletonManager::Instance().GetPlayerManager();
        if (!playerManager || (_this != playerManager->GetLocalPlayer())) return;

        WeaponController* weapon = _this->m_WeaponryController->m_CurrentWeapon;
        if (!weapon) return;

        WeaponType weaponType = weapon->m_WeaponParameters->GetType();

        switch (weaponType) {
        case WeaponType::None:
        case WeaponType::Grenade:
        case WeaponType::Knife:
            break;
        case WeaponType::Pistol:
        case WeaponType::Smg:
        case WeaponType::Rifle:
        case WeaponType::Shotgun:
        case WeaponType::Heavy:
        case WeaponType::Sniper:
            if (settings.enable) {
                auto gunController = (GunController*)weapon;

                GunParameters* params = gunController->gunParameters;
                if (params) {
                    if (settings.rapidFire) {
                        gunController->m_LastTimeFiredSafe.SetValue(gunController->m_LastTimeFiredSafe.GetValue() -
                            gunController->m_FireIntervalSafe.GetValue());
                    }

                    if (settings.infinityAmmo) {
                        gunController->_magazineCapacitySafe.SetValue(params->_ammunition->m_MagazineCapacity);
                    }

                    if (settings.accurateShooting) {
                        params->_accuracyMultOnJumpSafe.value.SetValue(0.f);
                        params->_accuracyMultOnJump = 0.f;

                        params->_accuracyMultOnCrouchSafe.value.SetValue(0.f);
                        params->_accuracyMultOnCrouch = 0.f;

                        params->_accuracyAimMultSafe.value.SetValue(0.f);
                        params->_accuracyAimMult = 0.f;

                        params->_accuracyAimMultOnCrouchSafe.value.SetValue(0.f);
                        params->_accuracyAimMultOnCrouch = 0.f;

                        AnimationCurve* accuracyAdditiveCurve = params->_accuracyAdditiveCurve;
                        if (accuracyAdditiveCurve) {
                            accuracyAdditiveCurve->ClearKeys();
                        }

                        gunController->accuracyMult = 0.f;
                        gunController->recoilMult = 0.f;
                    }

                    if (settings.damageHack) {
                        params->_damage->_headDamageSafe.value.SetValue(500);
                        params->_damage->_chestAndArmsDamageSafe.value.SetValue(500);
                        params->_damage->_legsDamageSafe.value.SetValue(500);
                        params->_damage->_stomachDamageSafe.value.SetValue(500);

                        params->_armorPenetrationSafe.value.SetValue(500.f);
                    }

                    if (settings.disableRecoil) {
                        gunController->recoilControl->SetLastShotTime(0.f);
                        gunController->recoilControl->SetCurrentActualPoint({ 0.f, 0.f });
                        gunController->recoilControl->SetCurrentRelativeDispersion({ 0.f, 0.f });
                        gunController->recoilControl->SetPreviousActualPoint({ 0.f, 0.f });
                    }

                    if (settings.wallShot) {
                        params->_penetrationPowerSafe.value.SetValue(999999999);
                    }
                }
            }
            break;
        }
    }

    void ApplySettings(const void* data) override {
        memcpy(&settings, data, sizeof(GunControlsSettings));
    }
    void GetCurrentSettings(void* out) override {
        memcpy(out, &settings, sizeof(GunControlsSettings));
    }
private:
    static inline GunControlsSettings settings;
};