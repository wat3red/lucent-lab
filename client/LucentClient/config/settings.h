#pragma once

#include <oxorany/oxorany_include.h>
#include <imgui/imgui.h>

#include "config_var.h"
#include "config_manager.h"

#include <windows.h>

class Settings {
public:
	struct SkinChanger {
		ConfigVar<bool> enabled{ oxorany("skinChanger"), oxorany("enabled"), false };
		ConfigVar<bool> autoApplyAfterLoading{ oxorany("skinChanger"), oxorany("autoApplyAfterLoading"), true };

		struct SkinEntry {
			int definitionId = 0;
			int flags = 0;
			int inventoryId = -1; // -1 для новых скинов

			// Методы для сериализации/десериализации для одного скина
			void Save(const std::string& section) {
				ConfigManager::SetValue(section, oxorany("definitionId"), definitionId);
				ConfigManager::SetValue(section, oxorany("flags"), flags);
				ConfigManager::SetValue(section, oxorany("inventoryId"), inventoryId);
			}

			void Load(const std::string& section) {
				definitionId = ConfigManager::GetValue<int>(section, oxorany("definitionId"), 0);
				flags = ConfigManager::GetValue<int>(section, oxorany("flags"), 0);
				inventoryId = ConfigManager::GetValue<int>(section, oxorany("inventoryId"), -1);
			}
		};

		std::vector<SkinEntry> skins;

		void ClearSkins() {
			int skinsCount = ConfigManager::GetValue<int>(oxorany("skinChanger"), oxorany("skinsCount"), 0);
			for (int i = 0; i < skinsCount; ++i) {
				ConfigManager::RemoveSection(oxorany("skinChanger.skin_") + std::to_string(i));
			}

			ConfigManager::SetValue<int>(oxorany("skinChanger"), oxorany("skinsCount"), 0);
		}

		void Save() {
			ClearSkins();
			// Сохраняем количество скинов
			ConfigManager::SetValue(oxorany("skinChanger"), oxorany("skinsCount"), static_cast<int>(skins.size()));
			// Сохраняем каждый скин в отдельной секции
			for (size_t i = 0; i < skins.size(); ++i) {
				skins[i].Save(oxorany("skinChanger.skin_") + std::to_string(i));
			}
		}

		void Load() {
			// Загружаем количество скинов
			int skinsCount = ConfigManager::GetValue<int>(oxorany("skinChanger"), oxorany("skinsCount"), 0);
			skins.resize(skinsCount);
			// Загружаем каждый скин
			for (int i = 0; i < skinsCount; ++i) {
				skins[i].Load(oxorany("skinChanger.skin_") + std::to_string(i));
			}
		}
	} skinChanger;

	struct Hits {
		ConfigVar<bool> hitlogs{ oxorany("hits"), oxorany("hitlogs"), false };
		ConfigVar<bool> damageLabel{ oxorany("hits"), oxorany("hitlogs"), false };
		ConfigVar<float> damageLabelSize{ oxorany("hits"), oxorany("damageLabelSize"), 12 };
		ConfigVar<ImColor> damageLabelColor{ oxorany("hits"), oxorany("damageLabelColor"), ImColor(255, 255, 255, 255) };
	} hits;

	struct Aim {
		ConfigVar<bool> enabled{ oxorany("aim"), oxorany("enabled"), false };
		ConfigVar<bool> silentAim{ oxorany("aim"), oxorany("silentAim"), false };
		ConfigVar<bool> disableVisibilityCheck{ oxorany("aim"), oxorany("disableVisibilityCheck"), false };

		ConfigVar<bool> autoFire{ oxorany("aim"), oxorany("autoFire"), false };

		ConfigVar<bool> drawMarker{ oxorany("aim"), oxorany("drawMarker"), false };
		ConfigVar<ImColor> markerColor{ oxorany("aim"), oxorany("markerColor"), ImColor(255, 255, 255, 255) };
		ConfigVar<float> markerSize{ oxorany("aim"), oxorany("markerSize"), 20 };

		ConfigVar<int> preferredBone{ oxorany("aim"), oxorany("preferredBone"), 0 };

		ConfigVar<int> targetSelectionMode{ oxorany("aim"), oxorany("targetSelectionMode"), 0 };
		ConfigVar<int> hitboxSelectionMode{ oxorany("aim"), oxorany("hitboxSelectionMode"), 0 };
		ConfigVar<int> visibilityCheckMode{ oxorany("aim"), oxorany("visibilityCheckMode"), 0 };

		ConfigVar<float> fov{ oxorany("aim"), oxorany("fov"), 90.f };
		ConfigVar<ImColor> fovLimiterColor{ oxorany("aim"), oxorany("fovLimiterColor"), ImColor(255, 255, 255, 255) };
		ConfigVar<float> limiterThickness{ oxorany("aim"), oxorany("limiterThickness"), 2.0f };
	} aim;

	struct AFKKickBypass {
		ConfigVar<bool> enabled{ oxorany("afkKickBypass"), oxorany("enabled"), false };
	} afkKickBypass;

	struct Misc {
		ConfigVar<bool> disableKickForAFK{ oxorany("misc"), oxorany("disableKickForAFK"), false };

		ConfigVar<bool> enableFov{ oxorany("misc"), oxorany("enableFov"), false };
		ConfigVar<float> fovValue{ oxorany("misc"), oxorany("fovValue"), 60.f };

		ConfigVar<bool> enableArmsPos{ oxorany("misc"), oxorany("enableArmsPos"), false };
		ConfigVar<Vector3> armsOffset{ oxorany("misc"), oxorany("armsOffset"), { 0.f, 0.f, 0.f } };

		ConfigVar<bool> thirdPersonView{ oxorany("misc"), oxorany("thirdPersonView"), false };
		ConfigVar<float> verticalOffset{ oxorany("misc"), oxorany("verticalOffset"), 3.f };
		ConfigVar<float> value{ oxorany("misc"), oxorany("value"), 3.f };

		ConfigVar<bool> enableAirJump{ oxorany("misc"), oxorany("enableAirJump"), false };

		ConfigVar<bool> enableStrafes{ oxorany("misc"), oxorany("enableStrafes"), false };
		ConfigVar<float> jumpMoveSpeed{ oxorany("misc"), oxorany("jumpMoveSpeed"), 6.f };

		ConfigVar<bool> moveBeforeTime{ oxorany("misc"), oxorany("moveBeforeTime"), false };

		ConfigVar<bool> noclip{ oxorany("misc"), oxorany("noclip"), false };
		ConfigVar<float> horizontalSpeed{ oxorany("misc"), oxorany("horizontalSpeed"), 6.f };
		ConfigVar<float> verticalSpeed{ oxorany("misc"), oxorany("verticalSpeed"), 0.7f };

		ConfigVar<bool> enableAntiAim{ oxorany("misc"), oxorany("enableAntiAim"), false };
		ConfigVar<int> antiAimType{ oxorany("misc"), oxorany("antiAimType"), 0 };
		ConfigVar<Vector2> antiAimAngles{ oxorany("misc"), oxorany("antiAimAngles"), { 0.f, 0.f } };
		ConfigVar<float> jitterRange{ oxorany("misc"), oxorany("jitterRange"), 0.0f };
		ConfigVar<float> spinSpeed{ oxorany("misc"), oxorany("spinSpeed"), 0.0f };

		ConfigVar<bool> incognitoMode{ oxorany("misc"), oxorany("incognitoMode"), false };
		ConfigVar<bool> ghostMode{ oxorany("misc"), oxorany("ghostMode"), false };
		ConfigVar<int> clumsyBind{ oxorany("misc"), oxorany("clumsyBind"), 'G' };
	} misc;

	struct GunControls {
		ConfigVar<bool> enable{ oxorany("gunControls"), oxorany("enable"), false };

		ConfigVar<bool> rapidFire{ oxorany("gunControls"), oxorany("rapidFire"), false };
		ConfigVar<bool> infinityAmmo{ oxorany("gunControls"), oxorany("infinityAmmo"), false };
		ConfigVar<bool> disableRecoil{ oxorany("gunControls"), oxorany("disableRecoil"), false };
		ConfigVar<bool> accurateShooting{ oxorany("gunControls"), oxorany("accurateShooting"), false };
		ConfigVar<bool> damageHack{ oxorany("gunControls"), oxorany("damageHack"), false };
		ConfigVar<bool> wallShot{ oxorany("gunControls"), oxorany("wallShot"), false };
	} gunControls;

	struct WorldController {
		ConfigVar<bool> enable{ oxorany("worldController"), oxorany("enable"), false };

		ConfigVar<bool> enableFog{ oxorany("worldController"), oxorany("enableFog"), false };
		ConfigVar<float> fogEndDistance{ oxorany("worldController"), oxorany("fogEndDistance"), 0.f };
		ConfigVar<float> fogStartDistance{ oxorany("worldController"), oxorany("fogStartDistance"), 30.f };
		ConfigVar<ImVec4> fogColor{ oxorany("worldController"), oxorany("fogColor"), ImColor(255, 255, 255, 255) };

		ConfigVar<bool> enableWorldColor{ oxorany("worldController"), oxorany("enableWorldColor"), false };
		ConfigVar<ImVec4> worldColor{ oxorany("worldController"), oxorany("worldColor"), ImColor(255, 255, 255, 255) };

		ConfigVar<bool> enableSkyColor{ oxorany("worldController"), oxorany("enableSkyColor"), false };
		ConfigVar<ImVec4> skyColor{ oxorany("worldController"), oxorany("skyColor"), ImColor(255, 255, 255, 255) };
	} worldController;

	struct ChamsModelConfig {
		ConfigVar<bool> enable;
		ConfigVar<ImVec4> invisibleEmissionColor;
		ConfigVar<ImVec4> visibleEmissionColor;
		ConfigVar<ImVec4> outlineColor;
		ConfigVar<float> outlineWidth;
		ConfigVar<int> outlineVisibilityMode;

		ChamsModelConfig(const std::string& prefix)
			: enable(oxorany("chams"), prefix + oxorany(".enable"), false),
			invisibleEmissionColor(oxorany("chams"), prefix + oxorany(".invisibleEmissionColor"), { 0.f, 0.f, 0.f, 1.f }),
			visibleEmissionColor(oxorany("chams"), prefix + oxorany(".visibleEmissionColor"), { 0.f, 0.f, 0.f, 1.f }),
			outlineColor(oxorany("chams"), prefix + oxorany(".outlineColor"), { 0.f, 0.f, 0.f, 1.f }),
			outlineWidth(oxorany("chams"), prefix + oxorany(".outlineWidth"), 0.05f),
			outlineVisibilityMode(oxorany("chams"), prefix + oxorany(".outlineVisibilityMode"), 0)
		{}
	};

	struct Chams {
		ConfigVar<bool> enable{ oxorany("chams"), oxorany("enable"), false };

		ChamsModelConfig playerArms{ oxorany("playerArms") };
		ChamsModelConfig playerGloves{ oxorany("playerGloves") };
		ChamsModelConfig enemy{ oxorany("enemy") };
		ChamsModelConfig ally{ oxorany("ally") };
	} chams;

	struct GrenadeIndicatorConfig {
		ConfigVar<bool> enable;
		ConfigVar<bool> showTimer;
		ConfigVar<ImVec4> timerColor;
		ConfigVar<int> identificatorType; // 0 for none, 1 for grenade icon, 2 for grenade name
		ConfigVar<ImVec4> identificatorColor;

		GrenadeIndicatorConfig(const std::string& prefix) :
			enable(oxorany("grenadeController"), prefix + oxorany(".enable"), false),
			showTimer(oxorany("grenadeController"), prefix + oxorany(".showTimer"), false),
			identificatorType(oxorany("grenadeController"), prefix + oxorany(".identificatorType"), 0.f),
			timerColor(oxorany("grenadeController"), prefix + oxorany(".timerColor"), { 1.f, 1.f, 1.f, 1.f }),
			identificatorColor(oxorany("grenadeController"), prefix + oxorany(".identificatorColor"), { 1.f, 1.f, 1.f, 1.f })
		{}
	};

	struct GrenadeController {
		ConfigVar<bool> enable{ oxorany("grenadeController"), oxorany("enable"), false };
		ConfigVar<bool> enableGrenadePractice{ oxorany("grenadeController"), oxorany("enableGrenadePractice"), true };

		ConfigVar<int> heDamage{ oxorany("grenadeController"), oxorany("heDamage"), 0 };
		GrenadeIndicatorConfig heIndicator{ oxorany("heIndicator") };

		ConfigVar<bool> smokeDisableEffects{ oxorany("grenadeController"), oxorany("smokeDisableEffects"), false };
		GrenadeIndicatorConfig smokeIndicator{ oxorany("smokeIndicator") };

		ConfigVar<bool> flashbangDisableEffects{ oxorany("grenadeController"), oxorany("flashbangDisableEffects"), false };
		GrenadeIndicatorConfig flashbangIndicator{ oxorany("flashbangIndicator") };

		ConfigVar<int> incendiaryDamage{ oxorany("grenadeController"), oxorany("incendiaryDamage"), 0 };
		GrenadeIndicatorConfig incendiaryIndicator{ oxorany("incendiaryIndicator") };
	} grenadeController;

	struct ESP {
		ConfigVar<bool> enabled{ oxorany("esp"), oxorany("enabled"), false };
		ConfigVar<bool> preciseBones{ oxorany("esp"), oxorany("preciseBones"), false };
		struct Visual {
			ConfigVar<bool> flags{ oxorany("esp"), oxorany("flags"), false };
			ConfigVar<ImColor> flagsColor{ oxorany("esp"), oxorany("flagsColor"), ImColor(255, 255, 255, 255) };
			ConfigVar<float> flagsSize{ oxorany("esp"), oxorany("flagsSize"), 14.0f };
			ConfigVar<int> flagsPosition{ oxorany("esp"), oxorany("flagsPosition"), 1 }; // 0 = Left, 1 = Right, 2 = Top, 3 = Bottom

			ConfigVar<bool> flagHelmet{ oxorany("esp"), oxorany("flagHelmet"), true };
			ConfigVar<bool> flagArmor{ oxorany("esp"), oxorany("flagArmor"), true };
			ConfigVar<bool> flagScoped{ oxorany("esp"), oxorany("flagScoped"), true };
			ConfigVar<bool> flagReloading{ oxorany("esp"), oxorany("flagReloading"), true };
			ConfigVar<bool> flagPlantDefuse{ oxorany("esp"), oxorany("flagPlantDefuse"), true };
			ConfigVar<bool> flagBombKit{ oxorany("esp"), oxorany("flagBombKit"), true };

			// Health bar
			ConfigVar<bool> healthBar{ oxorany("esp"), oxorany("healthBar"), false };
			ConfigVar<ImColor> healthLowColor{ oxorany("esp"), oxorany("healthLowColor"), ImColor(255, 0, 0, 255) };
			ConfigVar<ImColor> healthHighColor{ oxorany("esp"), oxorany("healthHighColor"), ImColor(0, 255, 0, 255) };
			ConfigVar<int> healthBarStyle{ oxorany("esp"), oxorany("healthBarStyle"), 0 }; // 0 = left, 1 = right, 2 = bottom
			ConfigVar<float> healthBarThickness{ oxorany("esp"), oxorany("healthBarThickness"), 3.0f };

			// Ammo bar
			ConfigVar<bool> ammoBar{ oxorany("esp"), oxorany("ammoBar"), false };
			ConfigVar<ImColor> ammoLowColor{ oxorany("esp"), oxorany("ammoLowColor"), ImColor(255, 255, 0, 255) };
			ConfigVar<ImColor> ammoHighColor{ oxorany("esp"), oxorany("ammoHighColor"), ImColor(0, 255, 255, 255) };
			ConfigVar<int> ammoBarStyle{ oxorany("esp"), oxorany("ammoBarStyle"), 2 }; // 0 = left, 1 = right, 2 = bottom
			ConfigVar<float> ammoBarThickness{ oxorany("esp"), oxorany("ammoBarThickness"), 2.0f };

			ConfigVar<bool> box2D{ oxorany("esp"), oxorany("box2d"), false };
			ConfigVar<ImColor> box2DColor{ oxorany("esp"), oxorany("box2DColor"), ImColor(255, 255, 255, 255) };

			ConfigVar<bool> box3D{ oxorany("esp"), oxorany("box3d"), false };
			ConfigVar<ImColor> box3DColor{ oxorany("esp"), oxorany("box3DColor"), ImColor(255, 255, 255, 255) };

			ConfigVar<bool> skeleton{ oxorany("esp"), oxorany("skeleton"), false };
			ConfigVar<ImColor> skeletonColor{ oxorany("esp"), oxorany("skeletonColor"), ImColor(255, 255, 255, 255) };

			ConfigVar<bool> nicknames{ oxorany("esp"), oxorany("nicknames"), false };
			ConfigVar<ImColor> nameColor{ oxorany("esp"), oxorany("nameColor"), ImColor(255, 255, 255, 255) };
			ConfigVar<float> nameSize{ oxorany("esp"), oxorany("nameSize"), 16.f };

			ConfigVar<bool> weaponIcon{ oxorany("esp"), oxorany("weaponIcon"), false };
			ConfigVar<ImColor> weaponIconColor{ oxorany("esp"), oxorany("weaponIconColor"), ImColor(255, 255, 255, 255) };
			ConfigVar<float> weaponIconSize{ oxorany("esp"), oxorany("weaponIconSize"), 18.f };
			ConfigVar<int> weaponIconDirection{ oxorany("esp"), oxorany("weaponIconDirection"), 0 };

			ConfigVar<bool> line{ oxorany("esp"), oxorany("line"), false };
			ConfigVar<ImColor> lineColor{ oxorany("esp"), oxorany("lineColor"), ImColor(255, 255, 255, 255) };
			ConfigVar<int> lineBase{ oxorany("esp"), oxorany("lineBase"), 0 };
			ConfigVar<int> lineTarget{ oxorany("esp"), oxorany("lineTarget"), 0 };
			ConfigVar<float> lineThickness{ oxorany("esp"), oxorany("lineThickness"), 2.f };
		} visual;
	} esp;

	struct SettingsUI {
		ConfigVar<int> theme{ oxorany("settingsUI"), oxorany("theme"), 0 };
		ConfigVar<bool> enableBlur{ oxorany("settingsUI"), oxorany("enableBlur"), true };
		ConfigVar<float> menuScale{ oxorany("settingsUI"), oxorany("menuScale"), 1.f };

		struct Hotkeys {
			ConfigVar<int> openMenu{ oxorany("settingsUI.hotkeys"), oxorany("openMenu"), VK_INSERT };
			ConfigVar<int> aim{ oxorany("settingsUI.hotkeys"), oxorany("aim"), 0 };
			ConfigVar<int> esp{ oxorany("settingsUI.hotkeys"), oxorany("esp"), 0 };
			ConfigVar<int> chams{ oxorany("settingsUI.hotkeys"), oxorany("chams"), 0 };
			ConfigVar<int> gunControls{ oxorany("settingsUI.hotkeys"), oxorany("gunControls"), 0 };
			ConfigVar<int> fov{ oxorany("settingsUI.hotkeys"), oxorany("fov"), 0 };
			ConfigVar<int> armsPos{ oxorany("settingsUI.hotkeys"), oxorany("armsPos"), 0 };
			ConfigVar<int> thirdPersonView{ oxorany("settingsUI.hotkeys"), oxorany("thirdPersonView"), 0 };
			ConfigVar<int> airJump{ oxorany("settingsUI.hotkeys"), oxorany("airJump"), 0 };
			ConfigVar<int> strafes{ oxorany("settingsUI.hotkeys"), oxorany("strafes"), 0 };
			ConfigVar<int> moveBeforeTime{ oxorany("settingsUI.hotkeys"), oxorany("moveBeforeTime"), 0 };
			ConfigVar<int> noclip{ oxorany("settingsUI.hotkeys"), oxorany("noclip"), 0 };
			ConfigVar<int> antiaim{ oxorany("settingsUI.hotkeys"), oxorany("antiaim"), 0 };
			ConfigVar<int> ghostMode{ oxorany("settingsUI.hotkeys"), oxorany("ghostMode"), 0 };
		} hotkeys;
	} settingsUI;
};

extern Settings settings;
