#pragma once

#include <vector>
#include <mutex>
#include <array>
#include <algorithm>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include "function.h"
#include "imgui/imgui.h"
#include "vector3.h"

#include "config/settings.h"
#include "config/imgui_config.h"

enum class WeaponId : uint8_t {
	None = 0,
	G22 = 11,
	USP = 12,
	P350 = 13,
	Deagle = 15,
	Tec9 = 16,
	FiveSeven = 17,
	Berettas = 18,
	UMP45 = 32,
	AkimboUzi = 33,
	MP7 = 34,
	P90 = 35,
	MP5 = 36,
	MAC10 = 37,
	VAL = 42,
	M4A1 = 43,
	AKR = 44,
	AKR12 = 45,
	M4 = 46,
	M16 = 47,
	FAMAS = 48,
	FnFal = 49,
	AWM = 51,
	M40 = 52,
	M110 = 53,
	SM1014 = 62,
	FabM = 63,
	M60 = 64,
	SPAS = 65,
	Knife = 70,
	KnifeBayonet = 71,
	KnifeKarambit = 72,
	jKommando = 73,
	KnifeButterfly = 75,
	FlipKnife = 77,
	KunaiKnife = 78,
	ScorpionKnife = 79,
	KnifeTanto = 80,
	DaggerKnife = 81,
	KnifeKukri = 82,
	KnifeStilet = 83,
	KnifeMantis = 85,
	KnifeFang = 86,
	KnifeSting = 88,
	Hands = 89,
	GrenadeHE = 91,
	GrenadeSmoke = 92,
	GrenadeFlash = 93,
	GrenadeMolotov = 94,
	GrenadeIncendiary = 95,
	Bomb = 100,
	Defuser = 101,
	DefuseKit = 102,
	Vest = 110,
	VestAndHelmet = 111,
	Watergun = 19,
	ScifiRailgun = 66,
	ScifiShotgun = 68,
	ScifiP90 = 69,
	CandyCane = 84,
	Firecracker = 96,
	EventGrenadeMolotov = 97,
	GrenadeSnowball = 99,
};

#pragma pack(push, 4) 
struct BipedMap { // points in 2d that are being sent from emulator
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

enum class Team : uint8_t {
	None = 0,
	Tr = 1,
	Ct = 2,
	Spec = 3,
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
	BipedMap bipedMap = {};
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

extern ImFont* weaponFontLeft;
extern ImFont* weaponFontRight;
extern ImFont* cyrillicFont;

const std::unordered_map<WeaponId, char> weaponFontMappings = {
	{ WeaponId::G22, 'A' },
	{ WeaponId::USP, 'B' },
	{ WeaponId::P350, 'C' },
	{ WeaponId::Deagle, 'D' },
	{ WeaponId::Tec9, 'E' },
	{ WeaponId::FiveSeven, 'F' },
	{ WeaponId::Berettas, 'G' },
	{ WeaponId::UMP45, 'H' },
	{ WeaponId::AkimboUzi, 'I' },
	{ WeaponId::MP7, 'J' },
	{ WeaponId::P90, 'K' },
	{ WeaponId::MP5, 'L' },
	{ WeaponId::MAC10, 'M' },
	{ WeaponId::VAL, 'N' },
	{ WeaponId::M4A1, 'O' },
	{ WeaponId::AKR, 'P' },
	{ WeaponId::AKR12, 'Q' },
	{ WeaponId::M4, 'R' },
	{ WeaponId::M16, 'S' },
	{ WeaponId::FAMAS, 'T' },
	{ WeaponId::FnFal, 'U' },
	{ WeaponId::AWM, 'V' },
	{ WeaponId::M40, 'W' },
	{ WeaponId::M110, 'X' },
	{ WeaponId::SM1014, 'Y' },
	{ WeaponId::FabM, 'Z' },
	{ WeaponId::M60, 'a' },
	{ WeaponId::SPAS, 'b' },
	{ WeaponId::Knife, '0' },
	{ WeaponId::KnifeBayonet, '1' },
	{ WeaponId::KnifeKarambit, '2' },
	{ WeaponId::jKommando, '3' },
	{ WeaponId::KnifeButterfly, '4' },
	{ WeaponId::FlipKnife, '5' },
	{ WeaponId::KunaiKnife, '6' },
	{ WeaponId::ScorpionKnife, '7' },
	{ WeaponId::KnifeTanto, '8' },
	{ WeaponId::DaggerKnife, '9' },
	{ WeaponId::KnifeKukri, '[' },
	{ WeaponId::KnifeStilet, ']' },
	{ WeaponId::KnifeMantis, '{' },
	{ WeaponId::KnifeFang, '}' },
	{ WeaponId::KnifeSting, '(' },
	{ WeaponId::GrenadeHE, 'c' },
	{ WeaponId::GrenadeSmoke, 'd' },
	{ WeaponId::GrenadeFlash, 'e' },
	{ WeaponId::GrenadeMolotov, 'f' },
	{ WeaponId::GrenadeIncendiary, 'g' },
	{ WeaponId::Bomb, 'h' },
	{ WeaponId::Defuser, 'i' }
};

// Example client implementation
class ESP : public Function {
public:
	std::string GetName() {
		return oxorany("Visuals/ESP");
	}

	std::chrono::steady_clock::time_point lastUpdateTime;
	uint32_t GetId() const override { return 2; }
	size_t GetSettingsSize() const override { return sizeof(ESPSettings); }

	enum LineTarget {
		Head, Hip, Feets
	};
	enum LineBase {
		Bottom, Center, Top
	};

	void RenderUI() override {
		ImGuiEx::Checkbox(oxorany("Enable ESP"), settings.esp.enabled);

		if (settings.esp.enabled) {
			ImGui::Indent();

			ImGuiEx::Checkbox(oxorany("2D Box"), settings.esp.visual.box2D);
			ImGuiEx::ColorEdit4(oxorany("2D Box color"), settings.esp.visual.box2DColor);

			ImGuiEx::Checkbox(oxorany("3D Box"), settings.esp.visual.box3D);
			ImGuiEx::ColorEdit4(oxorany("3D Box color"), settings.esp.visual.box3DColor);

			ImGuiEx::Checkbox(oxorany("Skeleton"), settings.esp.visual.skeleton);
			ImGuiEx::ColorEdit4(oxorany("Skeleton color"), settings.esp.visual.skeletonColor);

			ImGuiEx::Checkbox(oxorany("Nicknames"), settings.esp.visual.nicknames);
			ImGuiEx::ColorEdit4(oxorany("Name color"), settings.esp.visual.nameColor);
			if (settings.esp.visual.nicknames) {
				ImGui::Indent();
				ImGuiEx::SliderFloat(oxorany("Name size"), settings.esp.visual.nameSize, 8.0f, 24.0f);
				ImGui::Unindent();
			}

			ImGuiEx::Checkbox(oxorany("Weapon icon"), settings.esp.visual.weaponIcon);
			ImGuiEx::ColorEdit4(oxorany("Weapon icon color"), settings.esp.visual.weaponIconColor);
			if (settings.esp.visual.weaponIcon) {
				ImGui::Indent();
				const char* directions[] = { oxorany("Left"), oxorany("Right") };
				ImGuiEx::Combo(oxorany("Icon direction"), settings.esp.visual.weaponIconDirection, directions, IM_ARRAYSIZE(directions));
				ImGuiEx::SliderFloat(oxorany("Icon size"), settings.esp.visual.weaponIconSize, 8.0f, 24.0f);
				ImGui::Unindent();
			}

			ImGuiEx::Checkbox(oxorany("Line"), settings.esp.visual.line);
			ImGuiEx::ColorEdit4(oxorany("Line color"), settings.esp.visual.lineColor);
			if (settings.esp.visual.line) {
				ImGui::Indent();

				const char* bases[] = { oxorany("Bottom"), oxorany("Center"), oxorany("Top") };
				ImGuiEx::Combo(oxorany("Line base"), settings.esp.visual.lineBase, bases, IM_ARRAYSIZE(bases));
				const char* targets[] = { oxorany("Head"), oxorany("Hip"), oxorany("Feets") };
				ImGuiEx::Combo(oxorany("Line target"), settings.esp.visual.lineTarget, targets, IM_ARRAYSIZE(targets));

				ImGuiEx::SliderFloat(oxorany("Line thickness"), settings.esp.visual.lineThickness, 1.0f, 5.0f);
				ImGui::Unindent();
			}

			ImGuiEx::Checkbox(oxorany("Flags"), settings.esp.visual.flags);
			ImGuiEx::ColorEdit4(oxorany("Flags color"), settings.esp.visual.flagsColor);

			if (settings.esp.visual.flags) {
				ImGui::Indent();
				const char* flagPositions[] = { oxorany("Left"), oxorany("Right"), oxorany("Bottom") };
				ImGuiEx::Combo(oxorany("Flags position"), settings.esp.visual.flagsPosition, flagPositions, IM_ARRAYSIZE(flagPositions));
				ImGuiEx::SliderFloat(oxorany("Flags size"), settings.esp.visual.flagsSize, 8.0f, 24.0f);

				ImGuiEx::Checkbox(oxorany("Helmet"), settings.esp.visual.flagHelmet);
				ImGuiEx::Checkbox(oxorany("Armor"), settings.esp.visual.flagArmor);
				ImGuiEx::Checkbox(oxorany("Scoped"), settings.esp.visual.flagScoped);
				ImGuiEx::Checkbox(oxorany("Reloading"), settings.esp.visual.flagReloading);
				ImGuiEx::Checkbox(oxorany("Plant/Defuse"), settings.esp.visual.flagPlantDefuse);
				ImGuiEx::Checkbox(oxorany("Bomb/Kit"), settings.esp.visual.flagBombKit);

				ImGui::Unindent();
			}


			ImGuiEx::Checkbox(oxorany("Health bar"), settings.esp.visual.healthBar);
			if (settings.esp.visual.healthBar) {
				ImGui::Indent();
				const char* styles[] = { oxorany("Left"), oxorany("Right"), oxorany("Bottom") };
				ImGuiEx::Combo(oxorany("HP bar side"), settings.esp.visual.healthBarStyle, styles, IM_ARRAYSIZE(styles));
				ImGuiEx::SliderFloat(oxorany("HP bar thickness"), settings.esp.visual.healthBarThickness, 1.0f, 6.0f);
				ImGuiEx::ColorEdit4(oxorany("Low HP color"), settings.esp.visual.healthLowColor, true);
				ImGuiEx::ColorEdit4(oxorany("High HP color"), settings.esp.visual.healthHighColor, true);
				ImGui::Unindent();
			}

			ImGuiEx::Checkbox(oxorany("Ammo bar"), settings.esp.visual.ammoBar);
			if (settings.esp.visual.ammoBar) {
				ImGui::Indent();
				const char* styles[] = { oxorany("Left"), oxorany("Right"), oxorany("Bottom") };
				ImGuiEx::Combo(oxorany("Ammo bar side"), settings.esp.visual.ammoBarStyle, styles, IM_ARRAYSIZE(styles));
				ImGuiEx::SliderFloat(oxorany("Ammo bar thickness"), settings.esp.visual.ammoBarThickness, 1.0f, 6.0f);
				ImGuiEx::ColorEdit4(oxorany("Low Ammo color"), settings.esp.visual.ammoLowColor, true);
				ImGuiEx::ColorEdit4(oxorany("High Ammo color"), settings.esp.visual.ammoHighColor, true);
				ImGui::Unindent();
			}

			ImGui::Unindent();
		}
	}

	void DrawLine(ImDrawList* drawList, const EnemyData& enemy, float scaleX, float scaleY) {
		Vector3 target;
		switch ((LineTarget)settings.esp.visual.lineTarget.GetValue()) {
		case Head:
			target = enemy.bipedMap.Head;
			break;
		case Hip:
			target = enemy.bipedMap.Hip;
			break;
		case Feets:
			target = enemy.bipedMap.ControllerPos;
			break;
		}

		ImVec2 base;
		ImVec2 screenSize = ImGui::GetMainViewport()->Size;
		switch ((LineBase)settings.esp.visual.lineBase.GetValue()) {
		case Bottom:
			base = ImVec2(screenSize.x / 2, screenSize.y);
			break;
		case Center:
			base = ImVec2(screenSize.x / 2, screenSize.y / 2);
			break;
		case Top:
			base = ImVec2(screenSize.x / 2, 0);
			break;
		}

		drawList->AddLine(
			base, ImVec2(target.x * scaleX, target.y * scaleY), settings.esp.visual.lineColor.GetValue(), settings.esp.visual.lineThickness);
	}

	float GetScaleFromDistance(float distance, float minSize, float maxSize) {
		// Define distance range
		constexpr float minDistance = 5.0f;   // closest enemy (larger font)
		constexpr float maxDistance = 100.0f; // farthest enemy (smaller font)

		// Normalize and invert distance (closer = bigger)
		float t = 1.0f - std::clamp((distance - minDistance) / (maxDistance - minDistance), 0.0f, 1.0f);
		return minSize + t * (maxSize - minSize);
	}

	void DrawFlags(ImDrawList* drawList, const EnemyData& enemy, float scaleX, float scaleY) {
		if (!settings.esp.visual.flags) return;

		// масштабируем бокс
		BipedMap scaledBiped;
		scaledBiped.Head = Vector3{ enemy.bipedMap.Head.x * scaleX, enemy.bipedMap.Head.y * scaleY, enemy.bipedMap.Head.z };
		scaledBiped.ControllerPos = Vector3{ enemy.bipedMap.ControllerPos.x * scaleX, enemy.bipedMap.ControllerPos.y * scaleY, enemy.bipedMap.ControllerPos.z };

		ScreenBox box = Calculate2DBox(scaledBiped);

		float fontSize = GetScaleFromDistance(enemy.distanceToLocalPlayer, 8.0f, settings.esp.visual.flagsSize);

		// Собираем активные флаги
		std::vector<std::string> activeFlags;
		std::string armorFlags;

		if (settings.esp.visual.flagHelmet && enemy.hasHelmet)
			armorFlags.push_back(oxorany('H'));

		if (settings.esp.visual.flagArmor && enemy.armor > 15) {
			if (armorFlags.empty()) armorFlags += oxorany("V");
			else armorFlags += oxorany("+V");
		}
		if (!armorFlags.empty()) activeFlags.push_back(armorFlags);

		if (settings.esp.visual.flagScoped && enemy.isScoped)
			activeFlags.emplace_back(oxorany("Scoped"));

		if (settings.esp.visual.flagReloading && enemy.isReloading)
			activeFlags.emplace_back(oxorany("Reloading"));

		if (settings.esp.visual.flagPlantDefuse && enemy.isPlantingOrDefusing)
			activeFlags.emplace_back(enemy.team == Team::Tr ? oxorany("Planting") : oxorany("Defusing"));

		if (settings.esp.visual.flagBombKit && enemy.hasExplosiveRelatedItem)
			activeFlags.emplace_back(enemy.team == Team::Tr ? oxorany("Bomb") : oxorany("Kit"));

		// начальная позиция
		ImVec2 pos;
		switch (settings.esp.visual.flagsPosition.GetValue()) {
		case 0: // Left
			pos = ImVec2(box.top_left.x - 3.0f, box.top_left.y);
			break;
		case 1: // Right
			pos = ImVec2(box.bottom_right.x + 3.0f, box.top_left.y);
			break;
		case 2: // Bottom
		default:
			pos = ImVec2((box.top_left.x + box.bottom_right.x) * 0.5f, box.bottom_right.y + 3.0f + (settings.esp.visual.weaponIcon ? weaponnIconSizeY : 0.f));
			break;
		}

		// Рисуем
		for (const auto& flag : activeFlags) {
			ImVec2 textSize = cyrillicFont->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, flag.c_str());
			ImVec2 drawPos = pos;

			switch (settings.esp.visual.flagsPosition.GetValue()) {
			case 0: // Left
				drawPos.x -= textSize.x; // выравниваем влево
				break;
			case 1: // Right
				break;
			case 2: // Bottom
				drawPos.x -= textSize.x / 2.0f; // по центру
				break;
			}

			drawList->AddText(cyrillicFont, fontSize, drawPos, settings.esp.visual.flagsColor.GetValue(), flag.c_str());

			if (settings.esp.visual.flagsPosition.GetValue() == 0 || settings.esp.visual.flagsPosition.GetValue() == 1)
				pos.y += textSize.y + 2.0f;
			else
				pos.y += textSize.y + 2.0f;
		}
	}

	struct ScreenBox {
		ImVec2 top_left;
		ImVec2 bottom_right;

		ImVec2 top_right() const {
			return ImVec2(bottom_right.x, top_left.y);
		}
	};

	ScreenBox Calculate2DBox(const BipedMap& biped) {
		ScreenBox box;

		Vector3 delta = biped.ControllerPos - biped.Head;
		float height = sqrtf(delta.x * delta.x + delta.y * delta.y);
		float width = height * 0.5f; // or tweak based on testing

		float centerX = (biped.Head.x + biped.ControllerPos.x) / 2.0f;

		box.top_left = ImVec2(centerX - width / 2.0f, biped.Head.y);
		box.bottom_right = ImVec2(centerX + width / 2.0f, biped.ControllerPos.y);

		return box;
	}

	void Draw2DBox(ImDrawList* drawList, const EnemyData& enemy, float scaleX, float scaleY) {
		BipedMap scaledBipedMap;

		if (enemy.bipedMap.ControllerPos.z <= 0.01f) return;

		scaledBipedMap.Head = Vector3{ enemy.bipedMap.Head.x * scaleX, enemy.bipedMap.Head.y * scaleY, enemy.bipedMap.Head.z };
		scaledBipedMap.ControllerPos = Vector3{ enemy.bipedMap.ControllerPos.x * scaleX, enemy.bipedMap.ControllerPos.y * scaleY, enemy.bipedMap.ControllerPos.z };

		// Calculate the bounding box
		ScreenBox rect = Calculate2DBox(scaledBipedMap);

		// Draw the box
		drawList->AddRect(
			rect.top_left,  // Top-left corner
			rect.bottom_right,  // Bottom-right corner
			settings.esp.visual.box2DColor.GetValue(),  // Red color
			0.0f,  // Rounding
			ImDrawFlags_None,
			2.0f  // Thickness
		);
	}

	void Draw3DBox(ImDrawList* drawList, const BipedMap& biped, float scaleX, float scaleY) {
		const ImVec2 points[8] = {
			ImVec2(biped.boxCorners[0].x * scaleX, biped.boxCorners[0].y * scaleY),
			ImVec2(biped.boxCorners[1].x * scaleX, biped.boxCorners[1].y * scaleY),
			ImVec2(biped.boxCorners[2].x * scaleX, biped.boxCorners[2].y * scaleY),
			ImVec2(biped.boxCorners[3].x * scaleX, biped.boxCorners[3].y * scaleY),
			ImVec2(biped.boxCorners[4].x * scaleX, biped.boxCorners[4].y * scaleY),
			ImVec2(biped.boxCorners[5].x * scaleX, biped.boxCorners[5].y * scaleY),
			ImVec2(biped.boxCorners[6].x * scaleX, biped.boxCorners[6].y * scaleY),
			ImVec2(biped.boxCorners[7].x * scaleX, biped.boxCorners[7].y * scaleY)
		};

		// Check if any corner is behind the camera (z < 0)
		for (const auto& corner : biped.boxCorners) {
			if (corner.z <= 0.01f) return;
		}

		// Front face
		drawList->AddLine(points[0], points[1], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[1], points[2], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[2], points[3], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[3], points[0], settings.esp.visual.box3DColor.GetValue());

		// Back face							
		drawList->AddLine(points[4], points[5], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[5], points[6], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[6], points[7], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[7], points[4], settings.esp.visual.box3DColor.GetValue());

		// Connect front and back faces			
		drawList->AddLine(points[0], points[4], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[1], points[5], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[2], points[6], settings.esp.visual.box3DColor.GetValue());
		drawList->AddLine(points[3], points[7], settings.esp.visual.box3DColor.GetValue());
	}

	void DrawSkeleton(ImDrawList* drawList, const EnemyData& enemy, float scaleX, float scaleY) {
		auto drawLine = [&](Vector3 a, Vector3 b, ImColor color) {
			if (a.z <= 0.01f || b.z <= 0.01f) return;
			drawList->AddLine(ImVec2(a.x * scaleX, a.y * scaleY), ImVec2(b.x * scaleX, b.y * scaleY), color, 2.0f);
			};

		// Головa и шея
		drawLine(enemy.bipedMap.Head, enemy.bipedMap.Neck, settings.esp.visual.skeletonColor);

		// Позвоночник
		drawLine(enemy.bipedMap.Neck, enemy.bipedMap.Spine, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.Spine, enemy.bipedMap.Spine2, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.Spine2, enemy.bipedMap.Hip, settings.esp.visual.skeletonColor);

		// Руки
		drawLine(enemy.bipedMap.Neck, enemy.bipedMap.LeftShoulder, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.LeftShoulder, enemy.bipedMap.LeftUpperarm, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.LeftUpperarm, enemy.bipedMap.LeftForearm, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.LeftForearm, enemy.bipedMap.LeftHand, settings.esp.visual.skeletonColor);

		drawLine(enemy.bipedMap.Neck, enemy.bipedMap.RightShoulder, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.RightShoulder, enemy.bipedMap.RightUpperarm, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.RightUpperarm, enemy.bipedMap.RightForearm, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.RightForearm, enemy.bipedMap.RightHand, settings.esp.visual.skeletonColor);

		// Ноги	 
		drawLine(enemy.bipedMap.Hip, enemy.bipedMap.LeftUpLeg, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.LeftUpLeg, enemy.bipedMap.LeftLeg, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.LeftLeg, enemy.bipedMap.LeftFoot, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.LeftFoot, enemy.bipedMap.LeftToeBase, settings.esp.visual.skeletonColor);

		drawLine(enemy.bipedMap.Hip, enemy.bipedMap.RightUpLeg, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.RightUpLeg, enemy.bipedMap.RightLeg, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.RightLeg, enemy.bipedMap.RightFoot, settings.esp.visual.skeletonColor);
		drawLine(enemy.bipedMap.RightFoot, enemy.bipedMap.RightToeBase, settings.esp.visual.skeletonColor);
	}

	float weaponnIconSizeY = 0.0f;
	void DrawWeaponIcon(ImDrawList* drawList, const EnemyData& enemy, float scaleX, float scaleY) {
		const auto& characterSearch = weaponFontMappings.find(enemy.currentWeaponId);
		if (characterSearch == weaponFontMappings.end()) return;

		float size = GetScaleFromDistance(enemy.distanceToLocalPlayer, 8.0f, settings.esp.visual.weaponIconSize);

		ImVec2 textPos(
			enemy.bipedMap.ControllerPos.x * scaleX,
			enemy.bipedMap.ControllerPos.y * scaleY + 8.f
		);

		ImFont* font = settings.esp.visual.weaponIconDirection ? weaponFontRight : weaponFontLeft;
		ImGui::PushFont(font);

		char iconChar[] = { characterSearch->second, '\0' };
		ImVec2 iconSize = font->CalcTextSizeA(size, FLT_MAX, 0.0f, iconChar);
		weaponnIconSizeY = iconSize.y;

		textPos.x -= iconSize.x / 2.0f;  // center horizontally
		textPos.y -= iconSize.y / 2.0f;  // optional: center vertically

		drawList->AddText(font, size, textPos, settings.esp.visual.weaponIconColor.GetValue(), iconChar);

		ImGui::PopFont();
	}

	void DrawNickName(ImDrawList* drawList, const EnemyData& enemy, float scaleX, float scaleY) {
		float size = GetScaleFromDistance(enemy.distanceToLocalPlayer, 8.0f, settings.esp.visual.nameSize);

		// Calculate position above head
		ImVec2 textPos(
			enemy.bipedMap.Head.x * scaleX,
			enemy.bipedMap.Head.y * scaleY
		);

		// Calculate text size for centering
		ImVec2 textSize = cyrillicFont->CalcTextSizeA(size, FLT_MAX, 0.0f, enemy.nickName);

		textPos.x -= textSize.x / 2.0f;  // Center text horizontally 
		textPos.y -= textSize.y;

		// Draw main text
		drawList->AddText(cyrillicFont, size, textPos, settings.esp.visual.nameColor.GetValue(), enemy.nickName);
	}

	void RenderBackgroundUI() override {
		if (!settings.esp.enabled) return;

		if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - lastUpdateTime).count() > 1)
			return;

		std::lock_guard<std::mutex> lock(dataMutex);
		for (auto& enemy : enemies) {
			ImVec2 screenScale = GetScreenScale(enemy);

			if (settings.esp.visual.line)
				DrawLine(ImGui::GetBackgroundDrawList(), enemy, screenScale.x, screenScale.y);

			if (settings.esp.visual.box2D)
				Draw2DBox(ImGui::GetBackgroundDrawList(), enemy, screenScale.x, screenScale.y);

			if (settings.esp.visual.box3D)
				Draw3DBox(ImGui::GetBackgroundDrawList(), enemy.bipedMap, screenScale.x, screenScale.y);

			if (settings.esp.visual.skeleton)
				DrawSkeleton(ImGui::GetBackgroundDrawList(), enemy, screenScale.x, screenScale.y);

			if (settings.esp.visual.nicknames)
				DrawNickName(ImGui::GetBackgroundDrawList(), enemy, screenScale.x, screenScale.y);

			if (settings.esp.visual.weaponIcon)
				DrawWeaponIcon(ImGui::GetBackgroundDrawList(), enemy, screenScale.x, screenScale.y);

			if (settings.esp.visual.flags)
				DrawFlags(ImGui::GetBackgroundDrawList(), enemy, screenScale.x, screenScale.y);

			if (settings.esp.visual.healthBar)
				DrawHealthBar(ImGui::GetBackgroundDrawList(), enemy, screenScale.x, screenScale.y);

			if (settings.esp.visual.ammoBar)
				DrawAmmoBar(ImGui::GetBackgroundDrawList(), enemy, screenScale.x, screenScale.y);
		}
	}

	void DrawHealthBar(ImDrawList* drawList, const EnemyData& enemy, float scaleX, float scaleY) {
		if (enemy.maxHealth <= 0) return;
		if (enemy.bipedMap.ControllerPos.z <= 0.01f) return;

		BipedMap scaledBipedMap;
		scaledBipedMap.Head = Vector3{
		  enemy.bipedMap.Head.x * scaleX, enemy.bipedMap.Head.y * scaleY, enemy.bipedMap.Head.z
		};
		scaledBipedMap.ControllerPos = Vector3{
		  enemy.bipedMap.ControllerPos.x * scaleX, enemy.bipedMap.ControllerPos.y * scaleY, enemy.bipedMap.ControllerPos.z
		};

		const auto& box = Calculate2DBox(scaledBipedMap);
		float healthFrac = std::clamp(static_cast <float> (enemy.health) / enemy.maxHealth, 0.0f, 1.0f);
		float thickness = settings.esp.visual.healthBarThickness;

		ImVec2 p1, p2;
		switch (settings.esp.visual.healthBarStyle) {
		case 0: // left
			p1 = ImVec2(box.top_left.x - thickness - 1, box.top_left.y + (1.0f - healthFrac) * (box.bottom_right.y - box.top_left.y));
			p2 = ImVec2(box.top_left.x - 1, box.bottom_right.y);
			break;
		case 1: // right
			p1 = ImVec2(box.bottom_right.x + 1, box.top_left.y + (1.0f - healthFrac) * (box.bottom_right.y - box.top_left.y));
			p2 = ImVec2(box.bottom_right.x + thickness + 1, box.bottom_right.y);
			break;
		case 2: // bottom 
		default:
			p1 = ImVec2(box.top_left.x, box.bottom_right.y + 1);
			p2 = ImVec2(box.top_left.x + healthFrac * (box.bottom_right.x - box.top_left.x), box.bottom_right.y + thickness + 1);
			break;
		}

		drawList->AddRectFilledMultiColor(p1, p2, settings.esp.visual.healthHighColor.GetValue(), settings.esp.visual.healthHighColor.GetValue(), settings.esp.visual.healthLowColor.GetValue(), settings.esp.visual.healthLowColor.GetValue());
	}

	void DrawAmmoBar(ImDrawList* drawList, const EnemyData& enemy, float scaleX, float scaleY) {
		if (enemy.maxAmmo <= 0 || enemy.bipedMap.ControllerPos.z <= 0.f) return;

		BipedMap scaledBipedMap;
		scaledBipedMap.Head = Vector3{ enemy.bipedMap.Head.x * scaleX, enemy.bipedMap.Head.y * scaleY, enemy.bipedMap.Head.z };
		scaledBipedMap.ControllerPos = Vector3{ enemy.bipedMap.ControllerPos.x * scaleX, enemy.bipedMap.ControllerPos.y * scaleY, enemy.bipedMap.ControllerPos.z };

		const auto& box = Calculate2DBox(scaledBipedMap);
		float ammoFrac = std::clamp(static_cast<float>(enemy.ammo) / enemy.maxAmmo, 0.0f, 1.0f);

		ImVec2 p1, p2;
		switch (settings.esp.visual.ammoBarStyle) {
		case 0: // left (stacked beside HP)
			p1 = ImVec2(box.top_left.x - 2 * settings.esp.visual.ammoBarThickness - 3, box.top_left.y + (1.0f - ammoFrac) * (box.bottom_right.y - box.top_left.y));
			p2 = ImVec2(box.top_left.x - settings.esp.visual.ammoBarThickness - 3, box.bottom_right.y);
			break;
		case 1: // right
			p1 = ImVec2(box.bottom_right.x + settings.esp.visual.ammoBarThickness + 3, box.top_left.y + (1.0f - ammoFrac) * (box.bottom_right.y - box.top_left.y));
			p2 = ImVec2(box.bottom_right.x + 2 * settings.esp.visual.ammoBarThickness + 3, box.bottom_right.y);
			break;
		case 2: // bottom
		default:
			p1 = ImVec2(box.top_left.x, box.bottom_right.y + settings.esp.visual.ammoBarThickness + 3);
			p2 = ImVec2(box.top_left.x + ammoFrac * (box.bottom_right.x - box.top_left.x), box.bottom_right.y + 2 * settings.esp.visual.ammoBarThickness + 3);
			break;
		}

		drawList->AddRectFilledMultiColor(p1, p2, settings.esp.visual.ammoHighColor.GetValue(), settings.esp.visual.ammoHighColor.GetValue(), settings.esp.visual.ammoLowColor.GetValue(), settings.esp.visual.ammoLowColor.GetValue());
	}

	void ApplyData(const void* data, size_t size) override {
		if (size < sizeof(uint32_t)) return;

		const uint8_t* ptr = static_cast<const uint8_t*>(data);
		uint32_t count;
		memcpy(&count, ptr, sizeof(uint32_t));
		ptr += sizeof(uint32_t);

		std::lock_guard<std::mutex> lock(dataMutex);
		enemies.resize(count);

		memcpy(enemies.data(), ptr, count * sizeof(EnemyData));

		lastUpdateTime = std::chrono::steady_clock::now();
	}

	bool UpdateSettings() override {
		settingsForSending.enabled = settings.esp.enabled;

		return UpdateSettingsBase(settingsForSending, previousSettings);
	}

	void ApplySettings(const void* data) override {
		memcpy(&settingsForSending, data, sizeof(ESPSettings));
		previousSettings = settingsForSending;
	}

	void GetCurrentSettings(void* out) override {
		memcpy(out, &settingsForSending, sizeof(ESPSettings));
	}

private:
	// Android->PC
	ImVec2 GetScreenScale(const EnemyData& enemy) {
		ImVec2 screenSize = ImGui::GetMainViewport()->Size;
		return ImVec2(screenSize.x / enemy.androidWidth, screenSize.y / enemy.androidHeight);
	}

	ESPSettings settingsForSending;
	ESPSettings previousSettings;
	std::mutex dataMutex;
	std::vector<EnemyData> enemies;
};