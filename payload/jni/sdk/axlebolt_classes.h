#pragma once

#include "../il2cpp/il2cpp_structs.h"
#include "unity_classes.h"
#include "functions.h"
#include "utils.h"
#include "logger.h"
#include "enums.h"

#include <unordered_map>
#include <random>
#include <mutex>

struct Bone {
    const char* name;
    Vector3 position;
};

struct BipedMap : UnityEngine::MonoBehaviour {
    Transform* Head;
    Transform* Neck;
    Transform* Spine;
    Transform* Spine1;
    Transform* Spine2;
    Transform* LeftShoulder;
    Transform* LeftUpperarm;
    Transform* LeftForearm;
    Transform* LeftHand;
    Transform* RightShoulder;
    Transform* RightUpperarm;
    Transform* RightForearm;
    Transform* RightHand;
    Transform* Hip;
    Transform* LeftUpLeg;
    Transform* LeftLeg;
    Transform* LeftFoot;
    Transform* LeftToeBase;
    Transform* RightUpLeg;
    Transform* RightLeg;
    Transform* RightFoot;
    Transform* RightToeBase;
    Transform* LeftInHandIndex;
    Transform* LeftHandIndex1;
    Transform* LeftHandIndex2;
    Transform* LeftHandIndex3;
    Transform* LeftHandIndex4;
    Transform* LeftInHandMiddle;
    Transform* LeftHandMiddle1;
    Transform* LeftHandMiddle2;
    Transform* LeftHandMiddle3;
    Transform* LeftHandMiddle4;
    Transform* LeftInHandPinky;
    Transform* LeftHandPinky1;
    Transform* LeftHandPinky2;
    Transform* LeftHandPinky3;
    Transform* LeftHandPinky4;
    Transform* LeftInHandRing;
    Transform* LeftHandRing1;
    Transform* LeftHandRing2;
    Transform* LeftHandRing3;
    Transform* LeftHandRing4;
    Transform* LeftInHandThumb;
    Transform* LeftHandThumb1;
    Transform* LeftHandThumb2;
    Transform* LeftHandThumb3;
    Transform* RightInHandIndex;
    Transform* RightHandIndex1;
    Transform* RightHandIndex2;
    Transform* RightHandIndex3;
    Transform* RightHandIndex4;
    Transform* RightInHandMiddle;
    Transform* RightHandMiddle1;
    Transform* RightHandMiddle2;
    Transform* RightHandMiddle3;
    Transform* RightHandMiddle4;
    Transform* RightInHandPinky;
    Transform* RightHandPinky1;
    Transform* RightHandPinky2;
    Transform* RightHandPinky3;
    Transform* RightHandPinky4;
    Transform* RightInHandRing;
    Transform* RightHandRing1;
    Transform* RightHandRing2;
    Transform* RightHandRing3;
    Transform* RightHandRing4;
    Transform* RightInHandThumb;
    Transform* RightHandThumb1;
    Transform* RightHandThumb2;
    Transform* RightHandThumb3;
    Transform* WeaponContainer;



    Transform* GetBoneByName(const char* boneName) {
        struct BoneMapping {
            Transform* transform;
            const char* name;
        };

        BoneMapping mappings[] = {
            {Head, oxorany("Head")},
            {Neck, oxorany("Neck")},
            {Spine, oxorany("Spine")},
            {Spine1, oxorany("Spine1")},
            {Spine2, oxorany("Spine2")},
            {LeftShoulder, oxorany("LeftShoulder")},
            {LeftUpperarm, oxorany("LeftUpperarm")},
            {LeftForearm, oxorany("LeftForearm")},
            {LeftHand, oxorany("LeftHand")},
            {RightShoulder, oxorany("RightShoulder")},
            {RightUpperarm, oxorany("RightUpperarm")},
            {RightForearm, oxorany("RightForearm")},
            {RightHand, oxorany("RightHand")},
            {Hip, oxorany("Hip")},
            {LeftUpLeg, oxorany("LeftUpLeg")},
            {LeftLeg, oxorany("LeftLeg")},
            {LeftFoot, oxorany("LeftFoot")},
            {LeftToeBase, oxorany("LeftToeBase")},
            {RightUpLeg, oxorany("RightUpLeg")},
            {RightLeg, oxorany("RightLeg")},
            {RightFoot, oxorany("RightFoot")},
            {RightToeBase, oxorany("RightToeBase")}
        };

        for (const auto& mapping : mappings) {
            if (mapping.transform && std::strcmp(mapping.name, boneName) == 0) {
                return mapping.transform;
            }
        }

        return nullptr;
    }

    std::vector<Bone> GetBones() {
        std::vector<Bone> bones;

        // Define all bone mappings
        struct BoneMapping {
            Transform* transform;
            const char* name;
        };

        BoneMapping mappings[] = {
            {Head, oxorany("Head")},
            {Neck, oxorany("Neck")},
            {Spine, oxorany("Spine")},
            {Spine1, oxorany("Spine1")},
            {Spine2, oxorany("Spine2")},
            {LeftShoulder, oxorany("LeftShoulder")},
            {LeftUpperarm, oxorany("LeftUpperarm")},
            {LeftForearm, oxorany("LeftForearm")},
            {LeftHand, oxorany("LeftHand")},
            {RightShoulder, oxorany("RightShoulder")},
            {RightUpperarm, oxorany("RightUpperarm")},
            {RightForearm, oxorany("RightForearm")},
            {RightHand, oxorany("RightHand")},
            {Hip, oxorany("Hip")},
            {LeftUpLeg, oxorany("LeftUpLeg")},
            {LeftLeg, oxorany("LeftLeg")},
            {LeftFoot, oxorany("LeftFoot")},
            {LeftToeBase, oxorany("LeftToeBase")},
            {RightUpLeg, oxorany("RightUpLeg")},
            {RightLeg, oxorany("RightLeg")},
            {RightFoot, oxorany("RightFoot")},
            {RightToeBase, oxorany("RightToeBase")}
        };

        // Add all valid bones to the vector
        for (const auto& mapping : mappings) {
            if (mapping.transform) {
                bones.push_back(Bone{
                    mapping.name,
                    mapping.transform->GetPosition()
                    });
            }
        }

        return bones;
    }
};

template<typename T>
struct StateSimple : Il2CppObject {
    T curState;
    T prevState;
    float timeSwitched;
    bool isJustSwitched;
    int stateNo;
};

struct PhotonPlayer : Il2CppObject {
    Il2CppObject* m_Logger;
    int m_ActorID;
    Il2CppString* m_NameField;
    Il2CppString* m_UserId;
    bool m_IsLocal;
    bool m_IsInactive;
    Hashtable* m_CustomProperties;
    Il2CppObject* m_TagObject;

    int GetHealth() {
        if (m_CustomProperties)
            return m_CustomProperties->GetValue<int>(oxorany("health"));
        return 0;
    }

    int GetArmor() {
        if (m_CustomProperties)
            return m_CustomProperties->GetValue<int>(oxorany("armor"));
        return 0;
    }

    bool IsUntouchable() {
        if (m_CustomProperties)
            return m_CustomProperties->GetValue<bool>(oxorany("untouchable"));
        return false;
    }

    bool HasHelmet() {
        if (m_CustomProperties)
            return m_CustomProperties->GetValue<bool>(oxorany("helmet"));
        return false;
    }

    Il2CppString* GetNickName() {
        return PhotonPlayer_get_NickName(this);
    }

    void SetNickName(Il2CppString* value) {
        PhotonPlayer_set_NickName(this, value);
    }
};
static_assert(offsetof(PhotonPlayer, m_TagObject) == 0x20, "CHECK PhotonPlayer FIELD OFFSETS!");

struct SafeFloat {
    int salt;
    int encrypted;

    static float Decrypt(int32_t salt, int32_t encrypted) {
        if (salt & 1) {
            int32_t raw = salt ^ encrypted;
            return *reinterpret_cast<float*>(&raw);
        }

        int32_t raw = (encrypted & 0xFF00FF00) |
            ((uint8_t)(encrypted >> 16)) |
            ((encrypted & 0xFF) << 16);
        return *reinterpret_cast<float*>(&raw);
    }

    static int32_t Encrypt(float value, int32_t salt) {
        int32_t raw = *reinterpret_cast<int32_t*>(&value);
        if (salt & 1)
            return raw ^ salt;
        return (raw & 0xFF00FF00) | ((uint8_t)raw << 16) | ((raw >> 16) & 0xFF);
    }

    static int32_t GenerateSalt() {
        static thread_local std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<int32_t> dist(1, 0x7FFFFFFF); // avoid 0
        return dist(rng);
    }

    float GetValue() const {
        return Decrypt(salt, encrypted);
    }

    void SetValue(float value) {
        salt = GenerateSalt();
        encrypted = Encrypt(value, salt);
    }
};

struct SafeBool {
    int salt;
    int encrypted;

    bool GetValue() const {
        unsigned int result;
        if (salt & 1)
            result = encrypted & 0xFF00FF00 |
            ((uint8_t)encrypted << 16) |
            ((encrypted >> 16) & 0xFF);
        else
            result = salt ^ encrypted;

        return result == 1;
    }

    void SetValue(bool value) {
        if (salt & 1) {
            uint8_t low = (1 >> 16) & 0xFF;
            uint8_t mid = 1 & 0xFF;
            encrypted = (1 & 0xFF00FF00) | (mid << 16) | low;
        }
        else {
            encrypted = salt ^ 1;
        }

        if (!value) {
            if (salt & 1) {
                encrypted = 0;
            }
            else {
                encrypted = salt;
            }
        }
    }
};

struct SafeInt {
    int salt;
    int encrypted;

    int GetValue() const {
        int result;
        if (salt & 1)
            result = (encrypted & 0xFF00FF00) |
            ((uint8_t)encrypted << 16) |
            ((encrypted >> 16) & 0xFF);
        else
            result = salt ^ encrypted;

        return result;
    }

    void SetValue(int newValue) {
        if (salt & 1) {
            encrypted =
                (newValue & 0xFF00FF00) |
                ((newValue >> 16) & 0xFF) |
                ((newValue & 0xFF) << 16);
        }
        else {
            encrypted = salt ^ newValue;
        }
    }
};

template<typename T>
struct SafeEnum {
    int32_t key;
    int32_t obfuscated;

    T Get() const {
        T value = static_cast<T>(key ^ obfuscated);
        return value;
    }

    void Set(T value) {
        key = GenerateKey();
        obfuscated = key ^ static_cast<int32_t>(value);
    }

    SafeEnum() : key(0), obfuscated(0) {}

    SafeEnum(T value) {
        Set(value);
    }

private:
    static int32_t GenerateKey() {
        return 0x5A4B161C; // fixed key or replace with rand()
    }
};


namespace Photon {
    struct MonoBehaviour : UnityEngine::MonoBehaviour {
        struct PhotonView* pvCache;
    };

    struct PunBehaviour : MonoBehaviour {};
}

namespace Standoff::Common {
    struct PhotonBehavior : Photon::PunBehaviour {};
}

namespace Standoff::Player {
    struct Controller : Standoff::Common::PhotonBehavior
    {
        int32_t ACAFHCBAHEEDCEG;
        uint8_t FGHBGEFHFAEFBDB;
        BipedMap* DBHGHFBACHDFECC;
        Transform* transform;
        PhotonPlayer* BFDCEGCGEAACEFA;
        int32_t GFCCHBGABCBACEC;
        int32_t EGHBGHBHHDBBBBF;
        bool HGHDBCABEFBDEAE;
        bool _GDGGCGEAGFDGEAA_k__BackingField;
        bool _EEDGBHHBGHECCHF_k__BackingField;
    };
    static_assert(offsetof(Controller, _EEDGBHHBGHECCHF_k__BackingField) == 0x32, "CHECK Standoff::Player::Controller FIELD OFFSETS!");
}

template<typename T>
struct Nullable {
public:
    bool hasValue;
    T value;
};

struct CurvedValue : Il2CppObject {
    AnimationCurve* curve;
    float _multiplier;
    float duration;
    int id;
    float StartTime;
    Nullable<SafeFloat> _multiplierSafe;
};

template<typename T>
struct BlendedValue : Il2CppObject {
    T m_actual;
    T m_actualPrevFrame;
    T m_blended;
    T m_previous;
    float m_fTimeFix;
    float m_fBlendDuration;
};

struct RankedConfirmationDialog : Il2CppObject {
};

struct PlayerInputs : Il2CppObject {
    float m_fSideMove;
    float m_fForwardMove;
    bool m_bCrouch;
    bool m_bReload;
    int32_t m_iSwitchToWeapon;
    bool m_bAim;
    bool m_bFire;
    bool m_bJump;
    bool m_bDrop;
    bool m_bAction;
    bool m_bInspect;
    bool m_bPickup;
    bool m_bUnknown;
    Vector2 m_vecDeltaAimAngles;
};

struct PlayerTranslationParameters_CrouchParameters : Il2CppObject {
    float m_fCrouchSpeedMultiplier;
    float m_fCrouchSpeed;
    void* m_standToCrouchCurve;
    Nullable<SafeFloat> m_fCrouchSpeedMultiplierSafe;
};

struct PlayerTranslationParameters_WalkParameters : Il2CppObject {
    float m_WalkSpeedMultiplier;
    float m_fDefaultAcceleration;
    Nullable<SafeFloat> m_WalkSpeedMultiplierSafe;
};

struct PlayerTranslationParameters_JumpParameters : Il2CppObject
{
    float upwardSpeedDefualt;
    CurvedValue* jumpCurve;
    CurvedValue* landCurve;
    CurvedValue* minSpeedCurve;
    AnimationCurve* LandDelayCurve;
    AnimationCurve* LandDelayDurationCurve;
    AnimationCurve* LandDelayMultCurve;
    CurvedValue* MovementStopCurve;
    AnimationCurve* YRotationSpeedCurve;
    AnimationCurve* YRotationSpeedMultCurve;
    float JumpMoveSpeed;
};

struct PlayerTranslationParameters : Il2CppObject
{
    float speedDefault;
    Nullable<SafeFloat> _speedDefaultSafe;
    Nullable<SafeFloat> _speedMultiplierSafe;
    float moveDirectionChangeSpeed;
    float mecanimDirectionChangeSpeed;
    PlayerTranslationParameters_WalkParameters* walkParameters;
    struct PlayerTranslationParameters_IdleParameters* idleParameters;
    PlayerTranslationParameters_CrouchParameters* crouchParameters;
    PlayerTranslationParameters_JumpParameters* jumpParameters;
    struct PlayerTranslationParameters_CharacterColliderParameters* characterColliderParameters;
    struct PlayerTranslationParameters_GeneralCurveTypes* generalCurveTypes;
};
static_assert(offsetof(PlayerTranslationParameters, generalCurveTypes) == 0x40, "CHECK PlayerTranslationParameters FIELD OFFSETS!");

struct BulletHitData : Il2CppObject {
    Vector3 m_ShotPoint;
    Vector3 m_Point;
    float m_Impulse;
    int m_Damage;
    float m_ArmorPenetration;
    BoneID m_Bone;
    bool m_Penetrated;
};

struct TransformTR : Il2CppObject {
    Vector3 pos;
    Vector3 rot;
    Quaternion quaternionRot;
};
static_assert(offsetof(TransformTR, quaternionRot) == 0x20, "CHECK TransformTR FIELD OFFSETS!");

struct TransformTRS : Il2CppObject {
    Vector3 pos;
    Vector3 rot;
    Vector3 scale;
};

struct HitData : Il2CppObject {
    Vector3 m_VictimPosition;
    Vector3 m_Direction;
    WeaponId m_WeaponId;
    int32_t m_OwnerId;
    int32_t m_SkinId;
    Il2CppArray<BulletHitData*>* m_Hits;
    int16_t m_int1;
    int32_t m_int2;
};

struct WeaponMaterialController : Il2CppObject {
    void SetSkin(int skin) {
        //private AEDFBCBADDCGFAH <ACEFCHEBGAFDCEH>k__BackingField; // 0x18
        *(int*)((uintptr_t)this + 0x18) = skin;
    }

    // void UpdateMaterial() {
    //     //std::cout << oxorany("trying to call WeaponMaterialController_UpdateMaterial") << std::endl;

    //     WeaponMaterialController_UpdateMaterial(this);

    //     //std::cout << oxorany("called successfuly WeaponMaterialController_UpdateMaterial") << std::endl;

    // }
};

struct InventoryItemDefinition : Il2CppObject {
    int GetId() {
        // private AEDFBCBADDCGFAH _id; // 0xC
        return *(int*)((uintptr_t)this + 0xC);
    }

    void SetId(int newId) {
        // private AEDFBCBADDCGFAH _id; // 0xC
        *(int*)((uintptr_t)this + 0xC) = newId;
    }
};

struct BoltInventoryItem : Il2CppObject { // CEFHBFACHHBDHAB 0.32.3
    struct Properties;

    int definitionId; // 
    int quantity;//
    int flags;
    // void* unk;
    // Properties* props;
    Properties* props;
    void* unk;
    int id;//

    struct Properties : Il2CppObject
    {
        Dictionary<Il2CppString*, Il2CppObject*>* properties;
        Dictionary<Il2CppString*, Il2CppObject*>* properties2;
    };
};

struct tempName1 : Il2CppObject {
    // Fields
    int int1; // 0x8
    Il2CppString* str1; // 0xC
    Dictionary<int, float> dict1; // 0x10
    Dictionary<int, float> dict2; // 0x14
    Dictionary<Il2CppString*, Il2CppString*> dict3; // 0x18
    bool bool1; // 0x1C
    bool bool2; // 0x1D
    int int2; // 0x20
    bool bool3; // 0x24
};

struct InventoryService : Il2CppObject {
    Dictionary<int, tempName1*>* GetSomeThings1() {
        // private Dictionary<int, CEHFAGEHCFDCBCB> AAAAAEDAGBFCCEB; // 0x70
        return *(Dictionary<int, tempName1*>**)((uintptr_t)this + 0x70);
    }

    Dictionary<int, BoltInventoryItem*>* GetBoltInventoryItems() {
        // private Dictionary<int, DHGGHHACEBGGEGB> BCCFAGEFGBCHDFB; // 0x78
        return *(Dictionary<int, BoltInventoryItem*>**)((uintptr_t)this + 0x78);
    }
};

struct InventoryManager : Il2CppObject {
    // std::vector<InventoryItemDefinition*> GetItemDefinitions() {
        //std::cout << oxorany("this: ") << this << std::endl;

        // private Dictionary<int, InventoryItemDefinition> CGFDBEHABHGFABG; // 0x1C
        // uintptr_t itemsPtr = *(uintptr_t*)((uintptr_t)this + 0x1C);
        //std::cout << oxorany("itemsPtr: ") << itemsPtr << std::endl;

        // if (!itemsPtr) return std::vector<InventoryItemDefinition*>();
        // Dictionary<int, InventoryItemDefinition*> itemsDict(itemsPtr);

        // return itemsDict.GetValues();
    // }
};

struct InventoryActionPopupMenu : Il2CppObject {
    uintptr_t GetBoltInventoryItem() {
        // 	private CEFHBFACHHBDHAB ECHCCFDHCCHCAHG; // 0x50
        return *(uintptr_t*)((uintptr_t)this + 0x50);
    }
};

struct InventoryTabController : Il2CppObject {
    InventoryActionPopupMenu* GetInventoryActionPopupMenu() {
        // private InventoryActionPopupMenu _popupMenu; // 0x44
        return *(InventoryActionPopupMenu**)((uintptr_t)this + 0x44);
    }
};

struct InventoryController : Il2CppObject {
    InventoryTabController* GetInventoryTabController() {
        // private InventoryTabController _inventoryTabController; // 0x40
        return *(InventoryTabController**)((uintptr_t)this + 0x40);
    }
};

struct DamageEffect : Il2CppObject {
    bool _enabled;
    void* _texture;
    void* _distanceCurve;
    float _duration;
};

struct FlashbangEffect : Il2CppObject {
    bool _enabled;
    void* _texture;
    void* _curvesByAngle;
    void* _distanceMultiplierCurve;
    float _screenShotMultiplier;
};
static_assert(offsetof(FlashbangEffect, _screenShotMultiplier) == 0x18, "a");

struct SmokeEffect : Il2CppObject {
    bool _enabled;
    void* _texture;
    void* _curve;
    void* _verticalRadiusMultiplier;
    void* _radiusMultiplier;
    float _duration;
    float _blindnessRadius;
    void* _obstructionLimits;
};

struct MollyEffect : Il2CppObject {
    bool _Enabled;
    int32_t _FlamesCount;
    float _Radius;
    void* _ProgressCurve;
    void* _DamageCurve;
    int32_t _ChildCount;
    int32_t _BaseAngle;
    float _PositionOffset;
    int32_t _RandomRange;
    float _FireTime;
    float _YDiffBetweenFlames;
    float _YDamageOffset;
    Vector3 _BoxCastSize;
    float _DamageTick;
    int32_t _MaxDamage;
    int32_t _MaxFriendlyDamage;
    float _EffectYOffset;
    GameObject* _Prefab;
    GameObject* _HiResPrefab;
    float _ParticlesStopTimeOffset;
};
static_assert(offsetof(MollyEffect, _MaxDamage) == 0x48, "a");

struct GrenadeEffectParameters : Object {
    void* _particles;
    void* _alternativeParticles;
    void* _localExplosionParticles;
    float _particleDuration;
    bool _particleAdaptiveRotation;
    int32_t _particlesPoolSize;
    void* _decals;
    Vector2 _decalSize;
    float _decalCastDistance;
    float _radius;
    int32_t _effectPoolSize;
    bool _isRubberPool;
    bool _enableOnlyInRadius;
    DamageEffect* _damageEffect;
    FlashbangEffect* _flashbangEffect;
    SmokeEffect* _smokeEffect;
    Vector2 _padding;
    MollyEffect* _mollyEffect;
};

struct AntiCheatDetector : Il2CppObject {
    struct System_Action_BAFGABFFCEGEEAB__string__o* ACGEBBFHBDEDGGE;
    bool undetected;
    Transform* CECFHGAGAHGHDBC;
    struct UnityEngine_YieldInstruction_o* _ACHGEFECEHBGGBH_k__BackingField;
    struct UnityEngine_YieldInstruction_o* _EHHFADGFCHGBAGB_k__BackingField;
    struct UnityEngine_YieldInstruction_o* _HHCCHGGDDGBBDBC_k__BackingField;
};

struct AntiCheatManager : Il2CppObject {
    List<AntiCheatDetector*>* GetDetectors1() {
        // private readonly List<BAFGABFFCEGEEAB> GHGDCEECFEHHCAH; // 0x1C
        return *(List<AntiCheatDetector*>**)((uintptr_t)this + 0x1C);
    }

    List<AntiCheatDetector*>* GetDetectors2() {
        // private readonly List<BAFGABFFCEGEEAB> BGGABHGEGHFBDEE; // 0x20
        return *(List<AntiCheatDetector*>**)((uintptr_t)this + 0x20);
    }
};

struct GrenadeEffectEmitter {
    Dictionary<WeaponId, GrenadeEffectParameters*>* GetGrenadeEffectParameters() {
        // private Dictionary<CFAEEBDHGBAHFCF, GrenadeEffectParameters> GEDAEDGGDGGGDGF; // 0x14
        return *(Dictionary<WeaponId, GrenadeEffectParameters*>**)((uintptr_t)this + 0x14);
    }
};

struct MainController : Il2CppObject {
    InventoryController* GetInventoryController() {
        // private InventoryController CEHAEEAFDEBHHAC; // 0x54
        return *(InventoryController**)((uintptr_t)this + 0x54);
    }
};

struct InventoryParameters : Object {
    WeaponId _id;
    Il2CppString* _displayName;
    int32_t _cost;
    int32_t _movementRate;
    void* _sprites;
    Team _weaponTeam;
    bool _isAvailableCreateBuyRequest;
    Nullable<SafeEnum<WeaponId>> _idSafe;
    Nullable<SafeInt> _costSafe;
    Nullable<SafeInt> _movementRateSafe;

    WeaponType GetType() {
        WeaponId id = (WeaponId)_id;

        if (id <= WeaponId::Berettas)
            return WeaponType::Pistol;

        if (id >= WeaponId::UMP45 && id <= WeaponId::MAC10)
            return WeaponType::Smg;

        if (id >= WeaponId::VAL && id <= WeaponId::FnFal)
            return WeaponType::Rifle;

        if (id == WeaponId::FabM || id == WeaponId::SM1014 || id == WeaponId::SPAS)
            return WeaponType::Shotgun;

        if (id == WeaponId::M60)
            return WeaponType::Heavy;

        if (id >= WeaponId::AWM && id <= WeaponId::M110)
            return WeaponType::Sniper;

        if (id >= WeaponId::GrenadeHE && id <= WeaponId::GrenadeIncendiary)
            return WeaponType::Grenade;

        if (id >= WeaponId::Knife && id <= WeaponId::KnifeSting)
            return WeaponType::Knife;

        return WeaponType::None;
    }

    bool IsGun() {
        WeaponId id = (WeaponId)_id;
        return id > WeaponId::None && id < WeaponId::Knife;
    }
};

struct WeaponParameters : InventoryParameters {
    float _fixScale;
    bool _dropBoxCollider;
    void* _throwParameters;
    bool _inspectable;
    bool _forceTakeAfterPickup;
    bool _inspectableBonus;
    void* _hitDelayParameters;
    Il2CppString* _customAnimationName;
    uint8_t _customAnimationWeaponId;
    bool _isDroppable;
    void* _customShootButtonSprite;
    uint8_t _shootInShiftFireBehaviourType;
    uint32_t _shootInShiftAdditionalBehaviourType;
    float _shootInShiftSpeedMult;
    bool _drawStrecherOnGettingReadyToFire;
    bool _hasCancelAimButton;
    uint8_t _cancelAimBehaviourType;
    uint8_t _weaponAmmoIconType;
    bool _showHitMarker;
    int32_t _killType;
    Nullable<SafeFloat> _fixScaleSafe;
    Nullable<SafeBool> _dropBoxColliderSafe;
    Nullable<SafeBool> _inspectableSafe;
    Nullable<SafeBool> _forceTakeAfterPickupSafe;
    Nullable<SafeBool> _inspectableBonusSafe;
    Nullable<SafeBool> _isDroppableSafe;
    Nullable<SafeBool> _hasCancelAimButtonSafe;
    Nullable<SafeBool> _showHitMarkerSafe;
};

struct GrenadeParameters : WeaponParameters {
    float _takeDuration;
    float _throwDuration;
    float _nearThrowDuration;
    bool _throwByTap;
    void* _nearThrowParameters;
    void* _farThrowParameters;
    void* _ballisticsParameters;
    void* _ballisticsRotationParameters;
    Vector3 _scaleOnThrow;
    DetonationType m_DetonationType;
    float _detonationDuration;
    float _damageRadius;
    int32_t _damage;
    int32_t _penetrationPower;
    float _armorPenetration;
    int32_t _impulse;
    void* _damageCurve;
    float _backToPoolDelay;
    bool _unlimited;
    GameObject* _effectEmetter1;
    GameObject* _effectEmetter2;
    GameObject* _effectEmetterThirdView1;
    GameObject* _effectEmetterThirdView2;
    Il2CppString* _descriptionKeyLocalization;
    bool _isCountKillToStats;
    uint8_t _grenadeStatsFormatKeyType;
    Nullable<SafeFloat> _takeDurationSafe;
    Nullable<SafeFloat> _throwDurationSafe;
    Nullable<SafeFloat> _nearThrowDurationSafe;
    Nullable<SafeBool> _throwByTapSafe;
    Nullable<SafeFloat> _detonationDurationSafe;
    Nullable<SafeFloat> _damageRadiusSafe;
    Nullable<SafeInt> _damageSafe;
    Nullable<SafeInt> _penetrationPowerSafe;
    Nullable<SafeFloat> _armorPenetrationSafe;
    Nullable<SafeInt> _impulseSafe;
    Nullable<SafeFloat> _backToPoolDelaySafe;
    Nullable<SafeBool> _unlimitedSafe;
    Nullable<SafeBool> _isCountKillToStatsSafe;
    void* _statsFormatKeySafe;
};
static_assert(offsetof(GrenadeParameters, _statsFormatKeySafe) == 0x1E8, "CHECK GrenadeParameters FIELD OFFSETS!");

struct PhysicsCollisionDataValue {
    Collider* m_Collider;
    Vector3 m_RelativeVelocity;
    Vector3 m_HitPoint;
    Vector3 m_HitNormal;
};

struct DroppedGrenadeController : UnityEngine::MonoBehaviour {
    struct System_Collections_Generic_List_FEDDDDGGFHFGCFH__o* HHDHBBFFGECBGGB;
    struct System_Collections_Generic_List_IDetonationableGrenadeListener__o* FFGBDEGEGDCGACG;
    GrenadeParameters* m_GrenadeParameters;
    struct Axlebolt_Standoff_Inventory_Grenade_GrenadeSoundParameters_o* CGGFHGDCHFAGACA;
    Transform* m_GrenadeTransform;
    struct Axlebolt_Standoff_Inventory_WeaponMap_o* HHCGHDECFCEEHCD;
    struct Axlebolt_Standoff_Inventory_WeaponLodGroup_o* EDEBCGAHCCHFCFA;
    struct Axlebolt_Standoff_Inventory_WeaponMaterialController_o* BCBGEHABFHHDBAG;
    struct GDGDAHCECHDFDBB_o* CGCCCHDGBBHHDDC;
    float m_SpawnTime;
    uint8_t BEADFBGDBGBGDEA;
    float DBEDCDEDFADHCEE;
    PhotonPlayer* GGCFCDGFGFAHFFC;
    struct CFAHBGBCDFCBGGF_o* AEHEGAEBFHGFGBH;
    struct DGCDEFBDCFDAEFB_o* GFBHAGHBEHGABCE;
    Nullable<PhysicsCollisionDataValue> FAHGDCDGEEEBBEC;
    uint8_t FACFBCHAGBEDAGA;
    GameObject* GDGGEHBABDFDEHH;
    GameObject* AEHCEFCFGADEGCE;
    struct Axlebolt_Ballistics_Simulation_Abstractions_IRaycastPhysicsController_o* AAHDHFGECCCHHEG;
    Vector3 DEAEAAADCBDCEHG;
    Vector3 GEHDADABDBDBGEE;
    uint8_t _AEEFFHCFFGDEDCD_k__BackingField;
    Vector3 _CHAEFGACGABBAAD_k__BackingField;
};

// struct DroppedGrenadeController : Il2CppObject {
//     GrenadeParameters* GetGrenadeParameters() {
//         // private GrenadeParameters GEDAEDGGDGGGDGF; // 0x18
//         return *(GrenadeParameters**)((uintptr_t)this + 0x18);
//     }

//     Transform* GetTransform() {
//         // private Transform HHFHCBDHHCAHECB; // 0x20
//         return *(Transform**)((uintptr_t)this + 0x20);
//     }
// };

struct GrenadeManager : Il2CppObject {
    Dictionary<int, DroppedGrenadeController*>* GetDroppedGrenades() {
        // private readonly Dictionary<int, DroppedGrenadeController> GDFCDHCCHAGHBDD; // 0x18
        return *(Dictionary<int, DroppedGrenadeController*>**)((uintptr_t)this + 0x18);
    }
};

struct DCAEABEHEGHHFHH {
    bool GFHBEGABBFGEHDB;
    bool CFCFCFGGEFHAFEC;
    bool CFBGBCFCGDEGHBC;
    bool FAGEHCAAGHGACEE;
    bool AEDEEAFECCCEDHB;
    bool HECHAHEGGACCGBE;
};

struct WeaponController : UnityEngine::MonoBehaviour {
    PlayerController* GAEAHCBHDEHHBDA;
    struct Axlebolt_Standoff_Player_Mecanim_MecanimController_o* CEFDFFBBHFFGEGH;
    struct Axlebolt_Standoff_Inventory_Animation_WeaponAnimationController_o* BADGFADGGCBGCHC;
    struct Axlebolt_Standoff_Inventory_Animation_WeaponAnimationParameters_o* GDGBAHBCDHGBDFG;
    uint8_t ABFFDABGFAEFGGE;
    int32_t DFEFDCDGGFBGFDE;
    Action* HDHEGBGGDHGGCCB;
    Action* CDBGCHDHCGCHDCG;
    Transform* AGFDAHHEEHAHFGG;
    struct Axlebolt_Standoff_Inventory_Controller_array* HFCAFEGGCCGABBB;
    struct BCCEECFHHFABBBA_o* HFCHFDCDHFFGAAF;
    struct BCCEECFHHFABBBA_o* DDDABDAGFAGHCCE;
    float _DAFFABGECGFDDCC_k__BackingField;
    float _FEACBFFDFFAAGCH_k__BackingField;
    struct Axlebolt_Standoff_Inventory_WeaponLodGroup_o* _GCDCABGCBCECAFE_k__BackingField;
    int32_t _CEDGAAAEHDFCGBD_k__BackingField;
    uint8_t _ECHEADACCBFGAGC_k__BackingField;
    Il2CppString* m_sAnimationWeaponName;
    int32_t m_iOwnerID;
    int32_t m_int1;
    WeaponParameters* m_WeaponParameters;
    struct BHGAEADDAFEDDDC_o* m_ptr3;
    struct Axlebolt_Standoff_Inventory_WeaponMap_o* m_map;
    uint8_t m_iSlotIndex;
    bool m_bIsActive;
    struct Axlebolt_Standoff_Inventory_WeaponStatTrackController_o* m_statrackController;
    struct Axlebolt_Standoff_Inventory_WeaponMaterialController_o* m_material;
    int32_t skinId;
    struct DCBADEBCEBAFBBF_o* AAAGADGGCBDFDGA;
    struct FACBCAGADBHDEAG_o* GGBEDDGBGDCFHAG;
    DCAEABEHEGHHFHH DEAFCBGAFEHBBBF;
};

struct Damage : Il2CppObject {
    int32_t _headDamage;
    int32_t _chestAndArmsDamage;
    int32_t _stomachDamage;
    int32_t _legsDamage;
    AnimationCurve* _distanceDamageMult;
    Nullable<SafeInt> _headDamageSafe;
    Nullable<SafeInt> _chestAndArmsDamageSafe;
    Nullable<SafeInt> _legsDamageSafe;
    Nullable<SafeInt> _stomachDamageSafe;
};

struct Ammunition : Il2CppObject {
    int16_t m_MagazineCapacity;
    int16_t m_iCapacity;
    Nullable<SafeInt> m_MagazineCapacitySafe;
    Nullable<SafeInt> m_iCapacitySafe;
};

struct GunParameters : WeaponParameters {
    SightType _sightType;
    bool _drawDefaultRecoilLinesNotInAiming;
    uint8_t _rifleType;
    uint8_t _bulletTraceType;
    uint8_t _reloadType;
    uint8_t _ammunitionType;
    struct Axlebolt_Standoff_Inventory_Gun_ShotgunShellParameters_o* _shotgunShellParameters;
    uint8_t _muzzleType;
    bool _combineMeshInIdle;
    float _scopeAimSensitivityMult;
    Ammunition* _ammunition;
    bool _doubledAmmoConsumption;
    Damage* _damage;
    float _hitRange;
    int32_t _fireRate;
    int32_t _recoilControl;
    struct Axlebolt_Standoff_Inventory_Gun_RecoilParameters_o* _recoilParameters;
    AnimationCurve* _accuracyAdditiveCurve;
    float _accuracyMultOnJump;
    AnimationCurve* _additiveNoAimDispertionCurve;
    float _recoilMultOnCrouch;
    float _accuracyMultOnCrouch;
    float _recoilAimMult;
    float _accuracyAimMult;
    float _recoilAimMultOnCrouch;
    float _accuracyAimMultOnCrouch;
    int32_t _accurateRange;
    struct Axlebolt_Standoff_Inventory_Weapon_GunSightViewParameters_o* _gunSightViewParameters;
    float _armorPenetration;
    int32_t _penetrationPower;
    float _reloadDuration;
    float _tacticalReloadDuration;
    float _roundInsertDuration;
    float _reloadStartOffset;
    float _roundInsertOffset;
    int32_t _visibleCartridgeCount;
    float _magazineInsertTime;
    float _takeDuration;
    float _preReloadTime;
    int32_t _impulse;
    int32_t _muzzleFlashType;

    // === Safe поля ===
    Nullable<SafeInt>   _fireRateSafe;
    Nullable<SafeFloat> _scopeAimSensitivityMultSafe;
    Nullable<SafeFloat> _accuracyMultOnJumpSafe;
    Nullable<SafeFloat> _recoilMultOnCrouchSafe;
    Nullable<SafeFloat> _accuracyMultOnCrouchSafe;
    Nullable<SafeFloat> _recoilAimMultSafe;
    Nullable<SafeFloat> _accuracyAimMultSafe;
    Nullable<SafeFloat> _recoilAimMultOnCrouchSafe;
    Nullable<SafeFloat> _accuracyAimMultOnCrouchSafe;
    Nullable<SafeInt>   _recoilControlSafe;
    Nullable<SafeInt>   _accurateRangeSafe;
    Nullable<SafeFloat> _armorPenetrationSafe;
    Nullable<SafeInt>   _penetrationPowerSafe;
    Nullable<SafeFloat> _reloadDurationSafe;
    Nullable<SafeFloat> _tacticalReloadDurationSafe;
    Nullable<SafeFloat> _reloadStartOffsetSafe;
    Nullable<SafeFloat> _roundInsertOffsetSafe;
    Nullable<SafeFloat> _roundInsertDurationSafe;
    Nullable<SafeFloat> _magazineInsertTimeSafe;
    Nullable<SafeFloat> _takeDurationSafe;
    Nullable<SafeFloat> _preReloadTimeSafe;
    Nullable<SafeInt>   _impulseSafe;
    Nullable<SafeFloat> _hitRangeSafe;
    Nullable<SafeBool>  _combineMeshInIdleSafe;
    Nullable<SafeInt>   _visibleCartridgeCountSafe;
    Nullable<SafeBool>  _drawDefaultRecoilLinesNotInAimingSafe;
    Nullable<SafeBool>  _doubledAmmoConsumptionSafe;
};

static_assert(offsetof(GunParameters, _doubledAmmoConsumptionSafe) == 0x2AC, "GunParameters");
static_assert(offsetof(GunParameters, _accuracyAdditiveCurve) == 0x114, "GunParameters");

struct RecoilControl : Il2CppObject {
    void SetLastShotTime(float value) {
        // private float DEFABFDAEADDCFA; // 0x8
        *(float*)((uintptr_t)this + 0x8) = value;
    }

    void SetPreviousActualPoint(Vector2 value) {
        // private Vector2 HDFHCCCGBDHCCCD; // 0x14
        *(Vector2*)((uintptr_t)this + 0x14) = value;
    }

    void SetCurrentActualPoint(Vector2 value) {
        // private Vector2 GEDEAAFHAFABEFC; // 0x1C
        *(Vector2*)((uintptr_t)this + 0x1C) = value;
    }

    void SetCurrentRelativeDispersion(Vector2 value) {
        // 	private Vector2 AFGHHHEBBHDBFBG; // 0x28
        *(Vector2*)((uintptr_t)this + 0x28) = value;
    }
};

struct GunController : WeaponController {
    void* FFCEAGDEDHCCAGE;

    SafeFloat m_FireIntervalSafe;
    SafeFloat m_LastTimeFiredSafe;
    int32_t _DGBBCACBHGHCDGH_k__BackingField;
    int32_t BGBBCHGEGCDCDDE;
    SafeInt GABGEGCBBAEFGHE;
    SafeInt _magazineCapacitySafe;
    SafeBool _CGDEFGEBDGCBDHD_k__BackingField;
    SafeBool _GADHBHABADAADDC_k__BackingField;
    struct Axlebolt_Standoff_Inventory_Gun_GunSoundController_o* m_SoundController;
    uint8_t m_ShootingState;
    struct CAHABBFCCHGFFEF_o* DAHDEHHBGGHABCC;
    Action* m_Action1;
    RecoilControl* recoilControl;
    GunParameters* gunParameters;
    struct HFCDGHBCFFEEECG_o* _DCGAHHDHGEBECDH_k__BackingField;
    float AFDFFHFDEDDFFFD;
    float DGBECDHGHHHFBAE;
    struct BAAFBHDGCDECHGC_o* BEFEDCGDGBFFEEA;
    StateSimple<GunState>* m_GunState;
    StateSimple<AimingMode>* m_AimingMode;
    struct System_Collections_Generic_List_CDEFGHAGBFAHCHF__o* CDEEAFDCFCAGFCD;
	struct System_Collections_Generic_List_CDEFGHAGBFAHCHF__o* BGDEFHHBGBGDAAE;
	struct System_Collections_Generic_Queue_FHGBFFGGGDFHFDE__o* AEFEHCBBEEEAABF;
	struct System_Collections_Generic_List_FHGBFFGGGDFHFDE__o* ABBAFAGECBBBACH;
	struct System_Collections_Generic_List_ValueTuple_Vector3__Vector3___o* DDDEADABHBDFDAH;
    Transform* FEFHDHFFDEHBHCH;
    Transform* BGCHHHCBDBBFHFH;
    Transform* EAFHBAEGGHACABB;
    struct Axlebolt_Standoff_Inventory_Gun_GunSoundParameters_o* HHABBGBEEGGBGEB;
    float FHFFHEFBAFBAFEB;
    float EFHADGGEFCEGEEB;
    int32_t BEAGDFCHEBGEDFD;
    SafeFloat m_RadiusSafe; // not sure bout the name 
    SafeFloat m_MinRandomDegreeSafe; // not sure bout the name 
    SafeFloat m_MaxRandomDegreeSafe; // not sure bout the name 
    SafeFloat m_SafeFloat;
	struct GFCAGHDCDBAEBHG_o* DEDDCFBCEAFHDDC;
	struct System_Collections_Generic_List_CEEEFHDEHHBADEG__o* GBAFABHDCHEBCBE;
    bool AFGBEEEGHFBGHGA;
    float ABAAECGADHHHCGG;
    struct Axlebolt_Standoff_Inventory_AccuracyData_o* FDCGGBHCGADGGCE;
    struct System_Collections_Generic_Dictionary_AFFHDACGFCEBAHE__List_GBBBFGBHBFCBHGH___o* GGEFFCGFBEFBHAD;
    struct System_Collections_Generic_Dictionary_AFFHDACGFCEBAHE__DDBGFCFAEBHGDCF____o* EBHHFCGEBHEFDGA;
    float recoilMult;
    float accuracyMult;
    float AHFGABBEEEAHBAB;
    bool HHDCFGAAFABDEAF;
};
static_assert(offsetof(GunController, HHDCFGAAFABDEAF) == 0x170, "!!!");

struct BombParameters : WeaponParameters {
    float m_PlantDuration;
    float m_DetonationDuration;
    float m_DamageRadius;
    float m_Damage;
    float m_TakeDuration;
    void* m_DamageCurve;
    void* m_ArmorPenetrationCurve;
    float m_Impulse;
    TransformTR* m_Spine2Offset;
    bool m_DummyOnly;
    Nullable<SafeFloat> m_PlantDurationSafe;
    Nullable<SafeFloat> m_DetonationDurationSafe;
    Nullable<SafeFloat> m_DamageRadiusSafe;
    Nullable<SafeFloat> m_DamageSafe;
    Nullable<SafeFloat> m_TakeDurationSafe;
    Nullable<SafeFloat> m_ImpulseSafe;
};

struct BombController : WeaponController {
    StateSimple<BombState>* m_State;
    float m_Progress;
    void* m_Map;
    BombParameters* m_Parameters;
};

struct DefuseKitController : WeaponController {
    float m_Progress;
    DefuseKitState m_State;
    DefuseKitState m_OldState;
    void* m_SoundController;
};

struct WeaponryController : Standoff::Player::Controller {
    Dictionary<uint8_t, WeaponController*>* m_WeaponSlots;
    List<WeaponController*>* m_Weapons;
    List<void*>* m_List;
    List<void*>* m_List2;
    PlayerController* m_Player;
    void* m_Mecanim;
    uint8_t m_CurrentSlot;
    bool m_FPSView;
    void* m_PickUpController;
    DefuseKitController* m_DefuseKit;
    WeaponController* m_CurrentWeapon;
    bool m_Bool1;
    bool m_Bool2;
    float m_LocalTime;
    float m_DeltaTime;
    void* m_WeaponManager;

    bool HasWeapon(const WeaponId& id) const noexcept {
        const auto& weapons = m_Weapons->GetValues();

        for (const auto& weapon : weapons) {
            if (weapon && weapon->m_WeaponParameters && weapon->m_WeaponParameters->_id == id)
                return true;
        }
        return false;
    }

    // WeaponController* GetCurrentWeapon() {
    //     // private WeaponController <CEDAHGEEHGDFBHA>k__BackingField; // 0x58
    //     return memory->read<WeaponController*>((uintptr_t)this + 0x58);
    // }

    // std::vector<WeaponController*> GetWeapons() {
    //     // private readonly Dictionary<byte, WeaponController> CBEGCFGFGGFEDHH; // 0x34
    //     auto weaponSlots = memory->read<Dictionary<int, WeaponController*>*>((uintptr_t)this + 0x34);
    //     return weaponSlots->GetValues();
    // }
};

static_assert(offsetof(WeaponryController, m_WeaponManager) == 0x68, "WeaponryController fields!!!");
static_assert(offsetof(WeaponryController, m_CurrentWeapon) == 0x58, "WeaponryController fields!!!");


struct GameManager : Il2CppObject {
    // static inline bool GetInGame() {
    //     return GameManager_get_InGame();
    // }
};

struct GameModeTeamSettings {
    uint8_t m_MaxTeamSize;
    float m_SpeedMultiplier;
    bool m_HighJump;
    bool m_InfinityWeaponBuyTime;
    bool m_CanWeaponBuyEverywhere;
    bool m_InfinityAmmoAbsolutely;
    bool m_InfinityMagazines;
    bool m_InfinityGrenades;
    bool m_VisibleEnemyGrenadesTrail;
    bool m_LongGrenadeTrail;
    bool m_FriendlyFire;
    bool m_HeadshotOnly;
    int32_t m_KillsWinCondition;
    int32_t m_StartMoney;
    int32_t m_MaxMoney;
    int32_t m_Health;
    int32_t m_Armor;
    struct Axlebolt_Standoff_Game_Settings_GameModeWeaponSettings_o* m_WeaponSettings;
};

struct GameModeSettings : Il2CppObject {
    bool m_TeamChangingRestricted;
    int32_t m_WarmUpTime;
    float m_RespawnTime;
    int32_t m_MatchTime;
    int32_t m_RoundStartingTime;
    int32_t m_RoundTime;
    struct Axlebolt_Standoff_Game_Settings_BombSettings_o* m_BombSettings;
    int32_t m_RoundCount;
    bool m_GrenadePracticeType;
    GameModeTeamSettings* m_CtSettings;
    GameModeTeamSettings* m_TrSettings;
};

struct PlayerControls : UnityEngine::MonoBehaviour {
    int32_t _sourceType;
    struct Axlebolt_Standoff_Controls_KeyboardMouseControl_o* _keyboardMouseControl;
    PlayerController* _playerController;
    struct Axlebolt_Standoff_Controls_TouchController_o* DBGGFDCDGBAGAGC;
    struct Axlebolt_Standoff_Controls_RandomInputsSimulator_o* EAAHBEBHFFBHCHE;
    struct Axlebolt_Standoff_Controls_GyroscopeController_o* CFFBHGHCCFFCFAD;
    struct Axlebolt_Standoff_Controls_FireCommandModificator_o* FCEHCGHBECACEHH;
    struct Axlebolt_Standoff_Settings_Controls_ControlsSettingsManager_o* AGGCEEDFBBAHBAG;
    struct Axlebolt_Standoff_Settings_Game_GameSettingsManager_o* EEBGEDGBCAGDFCE;
    HashSet<Il2CppObject>* _disableMovementRequests;
    HashSet<Il2CppObject>* _disableFiringRequests;
    HashSet<Il2CppObject>* _disableDropRequests;
    HashSet<Il2CppObject>* _disableUnk1;
    HashSet<Il2CppObject>* _disableUnk2;
    HashSet<Il2CppObject>* _disableUnk3;
    struct Axlebolt_Standoff_Settings_Controls_ControlsSettings_o* GECFABGFGFDAEED;
    struct GFBBAAADEGDHGDH_o* ADHBEECBGCGFDGA;
    bool HBHFDECHADDFGFA;
    struct Axlebolt_Standoff_Controls_HUDView_o* _hudView;
    Action* _playerInputsFilter;
};

struct GameController : Photon::PunBehaviour {
    Il2CppObject* CachedPlayersFullNameList;
    Il2CppObject* CachedPlayersNickNameList;
    struct GGDHAFDGCDAGBEE_GDGFDBDCDHAGDFG__o* GameInitEvent;
    struct GGDHAFDGCDAGBEE_HFGEBEHEBDEBCHA__o* GameExitEvent;
    struct GGDHAFDGCDAGBEE_GCBFDBDGFFBBGDD__o* GameFinishedEvent;
    struct GGDHAFDGCDAGBEE_CFHHCCAAGBBDGFF__o* GameStateChangedEvent;
    struct GGDHAFDGCDAGBEE_CBEDDACBADCGFHH__o* PlayerWasKilledEvent;
    struct GGDHAFDGCDAGBEE_CBEDDACBADCGFHH__o* PlayerKilledEvent;
    struct GGDHAFDGCDAGBEE_CBEDDACBADCGFHH__o* PlayerAssistEvent;
    struct GGDHAFDGCDAGBEE_CBEDDACBADCGFHH__o* LocalPlayerDeadEvent;
    struct GGDHAFDGCDAGBEE_CBEDDACBADCGFHH__o* LocalPlayerSpawnEvent;
    struct GGDHAFDGCDAGBEE_WeaponParameters__o* LocalPlayerDropWeaponEvent;
    Hashtable* DAEEAGCABBBAHHG;
    struct System_Collections_Generic_List_CFAEEBDHGBAHFCF__o* HHHAHBFHEHDDBAA;
    float m_WhoKillViewShowTime;
    float m_LevelUpXpMultiplier;
    float m_XpMultiplierPerMap;
    float m_LevelUpDuration;
    float m_BattlePassAdditionalLevelUpDuration;
    struct Axlebolt_Standoff_Controls_WheelIndicatorDefinition_array* m_WheelIndicators;
    GameModeSettings* m_Settings;
    Transform* m_Transform;
    SafeFloat m_SafeFloat1;
    SafeFloat m_SafeFloat2;
    struct FHFADCCGEBBCEBH_o* GHFECBGAEABHEDG;
    struct UnityEngine_Coroutine_o* BFCBCEHGCFAHFEG;
    struct AHCFDEEDBGHFACB_o* AHEHFGDECDFHFED;
    struct Axlebolt_Standoff_Level_LevelDefinition_o* DHHAHFAHAGHFABE;
    Camera* mainCamera;
    Camera* BFGBFEFECEDFFAF;
    struct Axlebolt_Standoff_Cam_CameraEffectsManager_o* CHEBBAGGGBHGAHD;
    struct CCEGCDFGFHEGGAH_o* GFAFCCHDBAGAECH;
    struct Axlebolt_Standoff_Inventory_WeaponParameters_array* GFDBFHEHFEFCEBH;
    struct Axlebolt_Standoff_Inventory_WeaponParameters_array* FFAEABBGCEHFADB;
    struct Axlebolt_Standoff_Inventory_WeaponParameters_array* FADAGBCCHECEHAD;
    struct System_Collections_Generic_Dictionary_CFAEEBDHGBAHFCF__WeaponParameters__o* HAAECHHBHACCGCC;
    struct Axlebolt_Standoff_Inventory_EquipmentParameters_array* DHCFBGBHCGDGCGF;
    struct Axlebolt_Standoff_Inventory_EquipmentParameters_array* CCHAGFBCDBAFECB;
    struct Axlebolt_Standoff_Inventory_EquipmentParameters_array* DGCAAGBHGABHADH;
    struct Axlebolt_Standoff_Game_UI_Spectator_SpectatorHud_o* AADHHGADGEBBBFB;
    struct UnityEngine_Coroutine_o* EBCCGDHCCFFCGCA;
    bool FEACGBFEDEDAGBC;
    struct Axlebolt_Standoff_Game_TeamSelect_TeamSelectHud_o* ABBAGHHHHBEEEFC;
    struct Axlebolt_Standoff_Game_UI_Statistics_StatisticsView_o* FGGHDDHGEGCDGDA;
    struct Axlebolt_Standoff_Game_UI_Statistics_StatisticsButton_o* DFGGBGAAHGDDDHG;
    struct Axlebolt_Standoff_Game_UI_KillFeedView_o* GHDHCGCGCCEDAGG;
    struct Axlebolt_Standoff_Game_UI_Pause_PauseButton_o* CEFFEEDBGCDCGCC;
    struct Axlebolt_Standoff_Game_UI_ScreenshotView_o* BGFHADDDCGDFADD;
    struct Axlebolt_Standoff_Game_UI_WhoKillView_o* HDDADAGGDEGABCF;
    struct Axlebolt_Standoff_Game_UI_Chat_ChatButton_o* GBGCFCGCCFFGHDA;
    struct Axlebolt_Standoff_Game_UI_Chat_GameChatHud_o* FGGEFDCGBEHEEBH;
    struct Axlebolt_Standoff_Game_UI_GameLogsView_o* FEBGEDEEDGGEAFC;
    struct Axlebolt_Standoff_Game_UI_VoiceChat_VoiceLogsView_o* DFEGEADEGADAHEF;
    struct Axlebolt_Standoff_Game_UI_VoiceChat_VoiceChatButton_o* AEAHEACGDFCACCD;
    struct EBFECEGDHGFDCFF_o* AGCCBHDBCAHFCCD;
    struct Axlebolt_Standoff_Missions_GameMissionView_o* CEDBBBFBBBEFBBG;
    struct Axlebolt_Standoff_Game_Radar_RadarView_o* CFFHGFBEEABEFAH;
    struct Axlebolt_Standoff_Game_UI_Spectator_SpectatorRadarView_o* FBBCGHDHAEBGDGE;
    struct Axlebolt_Standoff_Game_Radar_RadarButton_o* BEDCBHBEBHDBAAE;
    struct Axlebolt_Standoff_Game_Radar_RadarViewButton_o* EEEEBCEFCBBHDGG;
    struct Axlebolt_Standoff_Game_ZoneView_o* CEEGHGDCCAHHCGE;
    struct Axlebolt_Standoff_Game_Voting_VotingInGameView_o* FCBFCAHDHEABHDC;
    Il2CppString* HBGAGEDBAFEHDGE;
    struct Axlebolt_Standoff_Game_UI_AfkKick_AfkKickHud_o* EGDDGGGECGBCDHB;
    struct System_Collections_Generic_Dictionary_GUID__ECBAFCGFHBECFCD__o* BADCBDBHADAGBFH;
    struct System_Collections_Generic_List_EventInstance__o* ADAHDCGGBEDAGHH;
    struct System_Collections_Generic_List_BBFHDEADAABFHBF_MarkerView__MarkerTrigger___o* HBHAEAHFFAEHEGH;
    float AHEFDCCHAFDCCHC;
    struct UnityEngine_Coroutine_o* GFGGHHEFFHGFHEA;
    struct Axlebolt_Standoff_Settings_Controls_ControlsSettingsManager_o* AGGCEEDFBBAHBAG;
    struct HFFDABBEBGEAEEC_o* DHAEEECCBDEFDCB;
    bool _CDFGEDFCCAFGDFD_k__BackingField;
    bool _FGAAFCHECECBDFG_k__BackingField;
    SafeBool CFFCGFFDHFDEAAG;
    bool HBHFDECHADDFGFA;
    struct Axlebolt_Standoff_UI_CachedPlayers_CachedPlayerUISettings_o* _GCAHGFBGAAAHFFB_k__BackingField;
    struct BHDGAFGGBGEAGGA_o* _FBBDACFBFECAABF_k__BackingField;
    struct HFGBFEGBGBABDCD_o* _AAACCCFGGBCCBCC_k__BackingField;
    struct Axlebolt_Standoff_Game_UI_GameHud_o* _ACFABGDFBFDAAFA_k__BackingField;
    struct Axlebolt_Standoff_Game_UI_Notifications_NotificationsHud_o* _HAHBGEBGECECEDB_k__BackingField;
    struct Axlebolt_Standoff_Game_UI_GameButtons_o* _CAAHHCCFAGFGDBC_k__BackingField;
    struct Axlebolt_Standoff_Game_Pause_PauseHud_o* _CHGEGHBACBHEGDD_k__BackingField;
    PlayerControls* _playerControls;
    float _FDAHDHGABFFFEBE_k__BackingField;
    PlayerController* m_PlayerController;
};
static_assert(offsetof(GameController, m_PlayerController) == 0x16C, "GameController fields!!!");
static_assert(offsetof(GameController, m_Transform) == 0x68, "GameController fields!!!");

struct HitEventArgs : Il2CppObject {
    PhotonPlayer* Shooter; // 0x8
    PhotonPlayer* Victim; // 0xC
    int Damage; // 0x10
    WeaponParameters* Weapon; // 0x14
    int Skin; // 0x18
    int WeaponOwnerId; // 0x1C
    bool Headshot; // 0x20
    bool Penetrated; // 0x21
    double Time; // 0x28
    HitData* HitData; // 0x30
};
static_assert(offsetof(HitEventArgs, HitData) == 0x30, "HitEventArgs");


struct AimingData : Il2CppObject {
    float curWTCoeff;
    float curMTCoeff;
    Vector3 angPitch; // curAimAngle
    Vector3 angYaw; // curEulerAngles
    TransformTR* gunSubstitude;
    float currentSpineZAxisOffset;
    float standTypeCoeff;
};

struct Pose {
    Vector3 position; // 0x0
    Quaternion rotation; // 0xC
};

struct PlayerCharacterView : Il2CppObject {
    BipedMap GetBipedMap() {
        return **(BipedMap**)((uintptr_t)this + 0x24);
    }
};

struct SkinnedMeshRenderer : Renderer {};

struct CharacterLodGroup : Il2CppObject {
    SkinnedMeshRenderer* GetSkinnedMeshRenderer() {
        // private SkinnedMeshRenderer _meshRenderer; // 0x20
        return *(SkinnedMeshRenderer**)((uintptr_t)this + 0x20);
    }
};

struct ArmsLodGroup : Renderer {
    SkinnedMeshRenderer* GetGlovesMeshRenderer() {
        // private SkinnedMeshRenderer _glovesMeshRenderer; // 0x20
        return *(SkinnedMeshRenderer**)((uintptr_t)this + 0x20);
    }

    SkinnedMeshRenderer* GetArmsMeshRenderer() {
        // protected SkinnedMeshRenderer _armsMeshRenderer; // 0x24
        return *(SkinnedMeshRenderer**)((uintptr_t)this + 0x24);
    }
};


struct ArmsAnimationController_OffsetPoints : Il2CppObject {
    TransformTRS* centerTR;
    Il2CppArray<Transform*>* point;
    Il2CppArray<TransformTRS>* pointTR;
};

struct ArmsAnimationController_ArmsOffsetInfo : Il2CppObject {
    Vector3 curAngularSpeed;
    Vector3 prevAngles;
    struct UnityEngine_AnimationCurve_o* offsetCoeffCurve;
    float relativeSpeed;
    float curVer;
    float curHor;
    float curMov;
    TransformTR* curFPSCamTR;
    float timeJumpStarted;
    struct UnityEngine_AnimationCurve_o* jumpAnimCurve;
    float jumpDuration;
    float timeLandStarted;
    struct UnityEngine_AnimationCurve_o* landAnimCurve;
    float landDuration;
    struct UnityEngine_AnimationCurve_o* FallDurationMultCurve;
};

struct ObjectOccludee : UnityEngine::MonoBehaviour // sizeof=0x1C
{
    Il2CppArray<void*>* BEFEABCGFCFDEFD;
    Transform* HHFHCBDHHCAHECB;
    bool BBAGGGCGDAEGBEF;
    bool BCEBEHBHGCEBFEA;
    bool _BAFDDHGCGCCABFA_k__BackingField;
    int32_t _BCAFEHCDFCEDBFD_k__BackingField;
    int32_t _AACEEFEABBGBCCE_k__BackingField;
};
static_assert(offsetof(ObjectOccludee, BEFEABCGFCFDEFD) == 0x10, "CHECK ArmsAnimationController FIELD OFFSETS!");



struct View : UnityEngine::MonoBehaviour {
    struct System_Action_bool__o* VisibleChanged;
};

struct HudComponentView : View {
    struct UnityEngine_CanvasGroup_o* BBACDBFEDBCADGG;
    bool EHDCEEADBEEEFEE;
};

struct CanvasGroup : UnityEngine::Behaviour {};

struct AimView : HudComponentView {
    struct UnityEngine_RectTransform_o* _interactiveArea;
    struct Axlebolt_Standoff_Controls_Crosshair_o* _crosshair;
    CanvasGroup* _defaultSight;
    CanvasGroup* _sniperSight;
    CanvasGroup* _collimatorSight;
    struct Axlebolt_Standoff_Controls_CollimatorSightViewParams_o* _collimatorSightViewParams;
    struct System_Collections_Generic_List_Image__o* _collimatorBackgrounds;
    struct UnityEngine_UI_Image_o* _collimatorSightPoint;
    struct UnityEngine_UI_Image_o* _sniperCrossLineHor;
    struct UnityEngine_UI_Image_o* _sniperCrossLineVer;
    struct UnityEngine_RectTransform_o* _sniperCrossBlurHor;
    struct UnityEngine_RectTransform_o* _sniperCrossBlurVer;
    struct UnityEngine_AnimationCurve_o* _sniperCrossLineFadeCurve;
    struct UnityEngine_AnimationCurve_o* _sniperCrossBlupShrinkCurve;
    float _sniperCrossBlurWidth;
    struct System_Collections_Generic_List_RectTransform__o* _stretchInds;
    CanvasGroup* _strechCanvas;
    float _strechAlpha;
    float _strechSpeed;
    float _startStretchOffeset;
    float _minStretchLimit;
    float _maxStretchLimit;
    struct UnityEngine_Camera_o* HDCHCDGBGCEABCF;
    float BBBFEADEFCEBAGB;
};

struct HUDView : View {
    struct Axlebolt_Standoff_Controls_HUDVisibleContainer_o* HUDVisibleContainer;
    struct AimView* AimView;
    struct Axlebolt_Standoff_Controls_HitMarkerView_o* HitMarkerView;
    struct Axlebolt_Standoff_Controls_AimButton_o* AimButton;
    struct Axlebolt_Standoff_Controls_CancelAimButton_o* CancelAimButton;
    struct Axlebolt_Standoff_Controls_ItemSelectView_o* ItemSelectView;
    struct Axlebolt_Standoff_Main_Inventory_Graffiti_GameGraffitiSelectView_o* GraffitielectView;
    struct Axlebolt_Standoff_Controls_WeaponInfoView_o* WeaponInfoView;
    struct Axlebolt_Standoff_Controls_GrenadeInfoView_o* GrenadeInfoView;
    struct Axlebolt_Standoff_Controls_PlayerStatsView_o* PlayerStatsView;
    struct Axlebolt_Standoff_Controls_WheelIndicatorView_o* WheelIndicatorsView;
    struct Axlebolt_Standoff_Controls_ClingingButton_o* ClingingButton;
    struct Axlebolt_Standoff_Controls_ShootButton_o* ShootButton;
    struct Axlebolt_Standoff_Controls_ShootInShiftButton_o* ShootInShiftButton;
    struct Axlebolt_Standoff_Controls_ActionButton_o* ActionButton;
    struct Axlebolt_Standoff_Controls_PickupButton_o* PickupButton;
    struct Axlebolt_Standoff_Controls_DropButton_o* DropButton;
    struct Axlebolt_Standoff_Controls_UntouchableView_o* UntouchableView;
    struct Axlebolt_Standoff_Controls_FloatingJoystick_o* FloatingJoystick;
    struct Axlebolt_Standoff_Controls_JumpButton_o* JumpButton;
    struct Axlebolt_Standoff_Common_FpsCounter_o* FpsCounter;
    struct Axlebolt_Standoff_Effects_Graffiti_GraffitiCanPerformPreview_o* GraffitiCanPerformPreview;
    struct Axlebolt_Standoff_Controls_NetworkStatus_NetworkStatusView_o* NetworkStatusView;
    struct Axlebolt_Standoff_Settings_Controls_SafeAreaBlockTouch_o* _blockTouch;
    GameObject* _controlsHud;
    struct Axlebolt_Standoff_UI_SafeArea_o* _bottomSafeAreaContainer;
    struct System_Collections_Generic_List_CEBFCHBEHAGGECD__o* FHECDCGAEACBDCA;
    struct Axlebolt_Standoff_Player_PlayerController_o* FAGAHHEGBGBHBDG;
    struct Axlebolt_Standoff_Controls_TouchController_o* DBGGFDCDGBAGAGC;
};

struct PlayerTranslationData : Il2CppObject
{
    float m_fTargetSpeed;
    float m_fCurrentSpeed;
    float m_fCurrentSpeedMult;
    Vector3 m_vecTargetDelta;
    Vector3 m_vecPrevDelta;
    float m_fPrevDeltaTime;
    Vector3 m_vecPrev;
    Vector3 m_vecCurrent;
    Vector2 m_vecTargetRelativeDirectionMecanim;
    Vector2 m_vecCurrentRelativeDirectionMecanim;
    Vector2 m_vecFixedRelativeDirectionMecanim;
    Vector3 m_VecMoveDirection;
    float m_fCharacterEulerY;
    float m_float1;
    float m_float2;
    float m_float3;
    struct System_Func_float__float__o* m_inputFilter;
    AnimationCurve* m_delayCurve;
    float  m_fDelayDuration;
    float  m_fDelaySetTime;
    float  m_fDelayMult;
    Vector3 m_prevFrameRotation;
    bool m_bIsVelocitySynchronized;
    Vector3 m_vecVelocity;
    struct FBAGDHFEEECGDED_float__o* m_fStandTypeCoeff;
    struct FBAGDHFEEECGDED_float__o* m_fJumpTypeCoeff;
    Vector2 m_vecTargetRelativeDirection;
    float m_fTargetMecanimDirectionMagnitude;
    BlendedValue<Vector2>* m_vecDampedRelativeDirection;
};

struct PlayerOcclusionController : ObjectOccludee // sizeof=0x34
{
    struct System_Collections_Generic_List_Vector2__o* _occlusionPoints;
    struct System_Collections_Generic_List_BipedMap_BBCEDEHADEDCEGB__o* _obstructionPoints;
    CharacterController* FHAACAFGFFBHBEB;
    Il2CppArray<Vector3>* BFAEDDCBHDEFCEC;
    Il2CppArray<Vector3>* FEFBHDGACBHFHEC;
    PlayerController* _DGACABBBHHEBEBH_k__BackingField;
};

struct ArmsAnimationController_ArmsAnimationParametres : Il2CppObject // sizeof=0x34
{
    AnimationCurve* XAxisAnimtion; // 0x8
    AnimationCurve* YAxisAnimtion; // 0xC
    AnimationCurve* ZAxisAnimtion; // 0x10
    AnimationCurve* ArmsShakingMultiplier; // 0x14
};

struct MovementController;
struct ArmsAnimationController : Standoff::Player::Controller {
    ArmsAnimationController_OffsetPoints* offsetPoints;
    ArmsAnimationController_ArmsOffsetInfo* armsOffsetInfo;
    Transform* fpsDirective;
    Transform* fpCameraPlaceholder;
    int32_t targetPointNo;
    bool resetToCamPos;
    bool isTuningMode;
    struct Axlebolt_Standoff_Player_Arms_ArmsAnimationController_FootstepTraceParameters* _footstepTraceParameters;
    ArmsAnimationController_ArmsAnimationParametres* _armsAnimationParametres;
    struct Axlebolt_Standoff_Player_Mecanim_MecanimController* mecanimController;
    MovementController* movementController;
    struct Axlebolt_Standoff_Player_Occlusion_PlayerOcclusionController* playerOcclusionController;
    int32_t JLStates;
    float _fallDurationMult;
    struct ABFGHDAEBFHAFGF_array* _listeners;
    int32_t _footstepTraceState;
    int32_t animationState;
    bool enabledAmTr;
    TransformTR* _additiveCameraAnimation;
    float CCCGCBAHDGAADFG;
    float ABBEEDAABFFCDCE;
    float _curFootstepTraceProgress;
    float GGACDCDBCFAHHEF;
    float _curFootstepCycleProgress;
    float _halfOffsetFootstepCycleProgress;
    Vector3 _additiveArmsAnimation;
    float CACABACEFAAABGE;
    float EBCACHFCGDGFAHH;
    float HDGEFEECEGGFDCB;
};
static_assert(offsetof(ArmsAnimationController, _additiveArmsAnimation) == 0x94, "CHECK ArmsAnimationController FIELD OFFSETS!");

struct CameraMovementController : UnityEngine::MonoBehaviour {
    bool FAECEDFHCFHHDGB;
    bool FAFDGEBGFGEDHFE;
    bool DAECBABGDGFFCEA;
    float GDAGFCFACHDFFHG;
    float FAEAAFGCEHEFFFH;
    float BCDEEHAEHBAGABD;
    float GGHBEEGDDCCEBGC;
    bool CFHECFBGEBCGAGE;
    float CCBGGHCFHEDAHFG;
    float DEBGDHHCAGCCHDE;
    struct FDGDDAEGDCAAEDG_o* BAADAAHBBHHCGEA;
    Transform* GFCGHADFHEGHDAH;
    struct UnityEngine_AnimationCurve_o* EABDCDCADDDFBDC;
    Vector3 BFHHDCFFADCCFBE;
    Vector3 FHFCDBCFBEGDDAD;
    RaycastHit GEDHCEHCEACFHFD;
    Camera* GHGEFGDBGHGBHBF;
    float _HBHGHACAFECGCAG_k__BackingField;
    Transform* EAHGDDBHBDFHGGH;
};
static_assert(offsetof(CameraMovementController, GHGEFGDBGHGBHBF) == 0x80, "CHECK CameraMovementController FIELD OFFSETS!");

struct MainCamera : CameraMovementController {
};

struct PlayerMainCamera : Il2CppObject {
    Transform* GetTransform() {
        // private Transform ACHCCGGEFBBGAHD; // 0x1C
        return *(Transform**)((uintptr_t)this + 0x1C);
    }

    MainCamera* GetMainCamera() {
        // private MainCamera EHECBADECDCBACB; // 0x20
        return *(MainCamera**)((uintptr_t)this + 0x20);
    }
};

struct PlayerFPSCamera : Il2CppObject {
};

template<typename TEnumState>
struct EnumWrapper : Il2CppObject {
    float _GGABDGDHDCEGFEA_k__BackingField;
    int32_t _FHGAAABGCCEEGEC_k__BackingField;
    TEnumState value;
};

template<typename TEnumState>
struct StateMap : Il2CppObject {
    struct System_Collections_Generic_List_BHEDEGADGDFHHCB_TEnumState___o* HHDCGFGCAGBGGBC;
    EnumWrapper<TEnumState>* currentState;
    EnumWrapper<TEnumState>* previousState;
};

struct StateMapWrapper : StateMap<TranslationStates> {
    void* DBDDDBCBCCFBDDH;
    struct EDEBEAEAACFDADB_HDGDDEGDFFHGCAD_o* HAECFFCAFGFGHCB;
};

struct MovementController : Standoff::Player::Controller
{
    PlayerController* _player;
    struct Axlebolt_Standoff_Player_Occlusion_PlayerOcclusionController_o* _playerOcclusion;
    bool _neverIdle;
    Transform* _movementDirector;
    struct DBAAAEEHGEHEBCH_o* _CGFBFGDEADBDHFH_k__BackingField;
    float _deltaTime;
    float _localTime;
    CharacterController* _characterController;
    struct Axlebolt_Standoff_Player_Movement_Trigger_o* _CDEBEGCBAAEHBAH_k__BackingField;
    struct EHGBGADGHHGEGFH_array* BEFEABCGFCFDEFD;
    StateMapWrapper* stateMap;
    PlayerTranslationParameters* translationParameters;
    PlayerTranslationData* translationData;
    struct ECFCEHHHBFADBDC_o* CAFCFGDFBGFBHEA;
    Transform* characterTransform;
    struct Axlebolt_Standoff_Player_Mecanim_MecanimController_o* _GBADBDGCGACBEAB_k__BackingField;
    float EEDDABGAHHDBGCC;
    struct System_Collections_Generic_List_HEDCDHDHHGFFDBC_FHGGGCBEDDEGDCD___o* BECGFDBHGAAFEAF;
    struct System_Collections_Generic_List_HEDCDHDHHGFFDBC_FHGGGCBEDDEGDCD___o* FGDGCEGDCAACDBE;
    struct HEDCDHDHHGFFDBC_FHGGGCBEDDEGDCD__array* DHHEAHDAHDGEBHE;
};

struct AimController : Standoff::Player::Controller {
    bool _overrideSpineRotation;
    int32_t m_observationState;
    float sensitivityX;
    float sensitivityY;
    float minimumX;
    float maximumX;
    Transform* FPSP_go;
    Transform* spineDirector;
    Transform* FPSCamera;
    Transform* camTransform;
    void* aimingParameters;
    AimingData* aimingData;
    struct Axlebolt_Standoff_Player_Aim_AimController_CCCACBDHFHCFCHE_o* interpolatorsBunch;
    struct Axlebolt_Standoff_Player_Aim_AimController_TuningParams_o* tuningParams;
    float _headDampingSpeed;
    PlayerController* _player;
    MovementController* _movement;
    Transform* _movementDirector;
    struct Axlebolt_Standoff_Player_Mecanim_MecanimController_o* _mecanim;
    Transform* _helper;
    bool _bIsGpOffsetInitialized;
    struct GDCECECDHBFGBFH_o* _commands;
    StateSimple<ViewMode>* _viewMode;
    Vector3 _angCurSpineAim;
    StateSimple<WeaponOffsetState>* _weaponOffsetState;
    StateSimple<MoveState>* _moveState;
    TransformTR* m_weaponDirOnStand;
    TransformTR* m_weaponDirOnCrouch;
    Pose FBHDFCDCCCDAGCD;
    Pose CGCDHEEGCFGGBDC;
    Pose ECAEGDDGBEAGDHG;
    Pose AECCEDFEBDHFDEB;
    Pose CFCBFEHAGFGBHFB;
    struct Axlebolt_Standoff_Inventory_Animation_WeaponAnimationParameters_o* m_weaponAnimationPars;
    struct FCFGACBEHBEAFCB_o* m_syncData;
    Action* m_action;
    bool _ABCADCDFGACGHAG_k__BackingField;
    float _BCFHACBHBHGEABA_k__BackingField;
    float _AHBEGCFFFDCHFEH_k__BackingField;
    struct FFEGAHABDCBCCEA_o* ACFGDHDHEFFDEFF;
    float GGFBGBHBBCAHFDB;
    float CGHGAFDDAAAABFE;
    struct FFDGAEECEDGHDEE_array* BEFEABCGFCFDEFD;
    WeaponryController* _weaponryController;
    Quaternion AFHEGAGDDHHCAAG;
    bool ToolPivotTuningEnabled;
    bool SpineRotationEnabled;
    Vector3 m_angles1;
    Vector3 m_angles2;
    Vector3 m_angles3;
    Pose FECDDBFGBCDEBAC;
};

struct PlayerController_Wrapped {
    PlayerController* HFBEGBCAHEGHHEG;
    bool _FDABEBHGBEDEHEF_k__BackingField;
};

enum class HitboxType
{
    Box = 0,
    Capsule = 1,
    Mesh = 2
};

struct PlayerHitboxConfig_HitboxConfig : Il2CppObject {
    BoneID bone; // 0x8
    HitboxType hitboxType; // 0xC
    Vector3 center; // 0x10
    Vector3 size; // 0x1C
    int direction; // 0x28
    float radius; // 0x2C
    float height; // 0x30
};

struct PlayerHitbox : UnityEngine::MonoBehaviour {
    PlayerHitboxConfig_HitboxConfig* _hitboxConfig;
    Collider* _collider;
    SafeEnum<BoneID> _boneID;
    SafeFloat _safeFloat; // 0.f
    struct AFFHDACGFCEBAHE_o* _EFHFGADBHDDGGEF_k__BackingField;
};

struct HitController : Standoff::Player::Controller {
    struct Axlebolt_Standoff_Player_Hit_PlayerHitboxConfig_o* _config;
    Dictionary<BoneID, PlayerHitbox*>* _playerHitboxes;
    PlayerOcclusionController* _occlusionController;
    PlayerController_Wrapped EGGFFCBDHEBGCGH;
    bool HGEACCGHABBFEGC;
    Action* FHBBCHEFBGFADFF;
    struct System_Action_EBGBBBGFHHHAFFA__o* CABAACEAEEAEGAG;
    CapsuleCollider* _trigger;
};

struct PlayerHitController : HitController {
    AnimationCurve* HitAffectCurve;
    float CEHGBCEDCBCEAEG;
    float GCGBHFFEHABHAGC;
    float GCBDEGBAHEAAGFD;
    bool FHCAHDHHBHDDCGD;
    Action* CABAACEAEEAEGAG;
    Action* OnHit; // Action<PlayerController, HitData>
    Action* FHBBCHEFBGFADFF;
    PhotonPlayer* FAAHBEBEGAEHHGC;
    struct EHDHHCDFAFCHEAD_array* HGAHBAHCBFEAHGA;
    struct Axlebolt_NetCode_Serialization_Serializers_BinaryStream_o* AEEBCBBFEGDHDDD;
    struct EBGBBBGFHHHAFFA_o* GGACACAGBEHBGFF;
    struct System_Object_array* DAGFDFDBGEFGGHH;
    float LocalTime;
    bool BEEBEHABHEEBBGF;
};

struct PlayerControllerStaticFields {
    void* ECAFHBCBCBGEFFE; // 0x0
    int MaxCtHealth; // 0x4
    int MaxCtArmor; // 0x8
    int MaxTrHealth; // 0xC
    int MaxTrArmor; // 0x10

    static PlayerControllerStaticFields* GetInstance() {
        static PlayerControllerStaticFields* instance = nullptr;
        if (instance == nullptr) {
            static Il2CppClass* klass = Il2CppUtils::FindKlass(oxorany("Axlebolt.Standoff.Player"), oxorany("PlayerController"));
            instance = reinterpret_cast<PlayerControllerStaticFields*>(klass->static_fields);
        }
        return instance;
    }
};
static_assert(offsetof(PlayerControllerStaticFields, MaxTrArmor) == 0x10, "PlayerControllerStaticFields offsets are incorrect!");

struct PhotonViewID {
    int32_t ACADGEHGCAAFFDG;
    int32_t HFBEGBCAHEGHHEG;
};

struct GrenadePracticeController : Il2CppObject {
    struct UnityEngine_MeshRenderer_o* _lineMeshRenderer;
    struct UnityEngine_MeshRenderer_o* _finalPointMeshRenderer;
    Camera* _camera;
    Transform* _cameraTransform;
    struct Axlebolt_Ballistics_GrenadePractice_Tracer_TracerRuntimeSettings_o* _tracerRuntimeSettings;
    struct Axlebolt_Ballistics_GrenadePractice_SO_GrenadePracticeTracerParameters_o* _grenadePracticeTracerParameters;
    Vector3 _vector3Up;
    Vector3 _boundsSize;
    Quaternion _downRotation;
    struct System_Collections_Generic_Queue_Vector3__o* _grenadeFollowingPath;
    Transform* _playerTransform;
    struct Axlebolt_Ballistics_GrenadePractice_Abstractions_GameplayGrenade_IPracticableGrenade_o* _practicableGrenade;
    struct Axlebolt_Ballistics_GrenadePractice_Abstractions_GameplayGrenade_IDetonationableGrenade_o* _detonationableGrenade;
    struct Axlebolt_Ballistics_GrenadePractice_Abstractions_IFullTracerArcSource_o* _fullTracerArcSource;
    bool _isGrenadeThrowed;
    bool _isGrenadeDetonated;
    float _grenadeDetonationTime;
    Vector3 _detonationPosition;
    uint8_t _state;
    Vector3 _lastPositionOnTracerEnd;
    Vector3 _lastFollowCameraPosition;
    bool _isFullPathDrawedInLastTick;
    struct System_Collections_Generic_List_Vector3__o* _tracerRawDrawPositions;
    Nullable<Vector3> _tracerFinalPosition;
    struct Axlebolt_Ballistics_GrenadePractice_Abstractions_IGrenadePracticeView_o* _grenadePracticeView;
    struct Axlebolt_Ballistics_GrenadePractice_Tracer_TracerMeshGenerator_o* _tracerMeshGenerator;
    struct Axlebolt_Ballistics_GrenadePractice_Practice_CameraZoneLimits_o* _cameraZoneLimits;
    struct System_Collections_Generic_List_Vector2__o* _levelZonePoints;
    bool _isActive;
    bool _isCameraViewEnabled;
    struct Axlebolt_Ballistics_GrenadePractice_Tracer_GrenadePracticeTracerLineRendererView_o* _grenadePracticeTracerLineRendererView;
    struct Axlebolt_Ballistics_GrenadePractice_Tracer_GrenadePracticeTracerFinalPointView_o* _grenadePracticeTracerFinalPointView;
    bool _isCameraFinalPointFreeFromLimits;
    bool _isFollowingCameraFindLimit;
    float _followingCameraFindLimitTime;
};
static_assert(offsetof(GrenadePracticeController, _followingCameraFindLimitTime) == 0xC4, "FIELD OFFSETS!");

struct GrenadePracticeControllerComponent : UnityEngine::MonoBehaviour {
    GrenadePracticeController* _grenadePracticeController;
};

struct PhotonView : Photon::MonoBehaviour {
    int32_t ownerId;
    uint8_t group;
    bool mixedModeIsReliable;
    bool OwnerShipWasTransfered;
    int32_t prefixBackup;
    Il2CppArray<Il2CppObject*>* instantiationDataField;
    Il2CppArray<Il2CppObject*>* lastOnSerializeDataSent;
    Il2CppArray<Il2CppObject*>* lastOnSerializeDataReceived;
    ViewSynchronization synchronization;
    int32_t onSerializeTransformOption;
    int32_t onSerializeRigidBodyOption;
    int32_t ownershipTransfer;
    List<Component*>* ObservedComponents;
    Dictionary<Component*, MethodInfo*>* m_OnSerializeMethodInfos;
    PhotonViewID viewIdField;
    int32_t instantiationId;
    int32_t currentMasterID;
    bool didAwake;
    bool isRuntimeInstantiated;
    bool removedFromLocalViewList;
    Il2CppArray<UnityEngine::MonoBehaviour*>* RpcMonoBehaviours;
    MethodInfo* OnSerializeMethodInfo;
    bool failedToFindOnSerialize;
};
// static_assert(offsetof(PhotonView, synchronization) == 0x2C - 0x14, ("CHECK Standoff::Player::Controller FIELD OFFSETS!"));
static_assert(offsetof(PhotonView, synchronization) == 0x2C, "FIELD OFFSETS!");

struct PlayerController : Photon::PunBehaviour {
    Transform* m_MainCameraHolder;
    GameObject* m_FpsCameraHolder;
    GameObject* m_FpsDirective;
    void* m_LevelZones;
    PlayerCharacterView* m_CharacterTPS;
    PlayerCharacterView* m_CharacterFPS;
    bool m_ReconnectedInstance;
    Team m_Team;
    float m_LocalTime;
    AimController* m_AimController;
    WeaponryController* m_WeaponryController;
    void* m_MecanimController;
    MovementController* m_MovementController;
    ArmsAnimationController* m_ArmsAnimationController;
    PlayerHitController* m_PlayerHitController;
    PlayerOcclusionController* m_PlayerOcclusionController;
    void* m_NetworkController; // NetworkController
    ArmsLodGroup* m_ArmsLodGroup;
    PlayerCharacterView* m_PlayerCharacterView;
    bool m_IsCharacterVisible;
    bool m_IsCharacterVisible2;
    float m_SnapshotSetTime;
    void* m_PlayerSoundController;
    PlayerMainCamera* m_PlayerMainCamera;
    PlayerFPSCamera* m_PlayerFPSCamera;
    void* m_PlayerMarkerTrigger;
    Transform* m_Transform;
    void* m_ControllerArray;
    void* m_DictionaryTypeController;
    void* m_CharacterController;
    void* m_SkinnedMeshLodGroup;
    CharacterLodGroup* m_CharacterLodGroup;
    bool m_IsPreInitialized;
    bool m_IsPostInitialized;
    ViewMode m_ViewMode;
    Nullable<int> m_HHGCBGDEAFAEGBF;
    Nullable<bool> m_HHABDCAHHEGBFBG;
    Nullable<int> m_HGGGCEDGGGEGEHA;
    PhotonView* m_PhotonView;
    int32_t m_PlayerID;
    int32_t m_PlayerViewID;
    PhotonPlayer* m_PhotonPlayer;

    bool IsAlive() {
        if (m_PhotonPlayer != nullptr &&
            m_PlayerCharacterView != nullptr &&
            m_Transform != nullptr) {
            return m_PhotonPlayer->GetHealth() > 0;
        }

        return false;
    }
};
static_assert(offsetof(PlayerController, m_PhotonPlayer) == 0xB4, "FIELD OFFSETS!");

struct SimplePool : Il2CppObject {
    void* CGCBCCADHECDHEF;
    void* AFFCGGAEFFDAGFB;
    void* HEADEDEAHFHEAFC;
    void* CDBEBGADAFDHHEE;
    bool HCBBEFDDDEFEHFE;
};

struct GameSettings : Il2CppObject {
    Il2CppString* Region;
    int32_t FieldOfViewType;
    bool SwitchWeaponOnPickup;
    bool IsFpsCounterEnabled;
    bool InGameNotificationsEnabled;
    bool HideAvatars;
    int32_t HUDVisible;
};

struct GameSettingsManager : UnityEngine::MonoBehaviour {
    GameSettings* gameSettings;
    struct DFHGBBFBFEDFFAE_o* _EEHHHBCCDEGEAHC_k__BackingField;
    struct System_Action_GameSettings__o* AHGBHHFECGDBCGA;
    struct EHFBDDGBGDAEHGD_o* BBBDEGAEFHFAEFH;
};

struct PlayerPool : SimplePool {
    struct Axlebolt_Standoff_Player_PlayerController_o* BFCGEEFGBGFHGEA;
    struct System_Type_array* EAFHHEHDADEBACB;
    struct System_Collections_Generic_Dictionary_string__CharacterMaterial__o* DGAFHAABDAFADGE;
    struct Axlebolt_Standoff_Player_PlayerCharacters_o* AHCCFAAFBDFBDAF;
    BipedMap* FDFGGBGCGFHFCAH;
    struct System_Collections_Generic_Dictionary_string__ArmsMaterial__o* HHFGEHCFBCDHFCC;
    struct Axlebolt_Standoff_Player_PlayerArms_o* DCBGGDAEEHFBCBA;
    BipedMap* CDFFEEFADFGGADF;
    uint8_t BAABBCBDHDDGFCD;
    int32_t AABECGEHADGHHDH;
};
static_assert(offsetof(PlayerPool, AABECGEHADGHHDH) == 0x40, "CHECK PhotonPlayer FIELD OFFSETS!");

struct RoomInfo : Il2CppObject {
    bool _removedFromList_k__BackingField;
    struct ExitGames_Client_Photon_Hashtable_o* customPropertiesField;
    uint8_t maxPlayersField;
    int32_t emptyRoomTtlField;
    int32_t playerTtlField;
    Il2CppArray<Il2CppString*>* expectedUsersField;
    bool openField;
    bool visibleField;
    bool autoCleanUpField;
    Il2CppString* nameField;
    int32_t masterClientIdField;
    bool _serverSideMasterClient_k__BackingField;
    int32_t _PlayerCount_k__BackingField;
    bool _IsLocalClientInside_k__BackingField;
};

struct Room : RoomInfo {
    struct System_String_array* _PropertiesListedInLobby_k__BackingField;
};

struct PhotonPeer : Il2CppObject {
    int32_t _CommandBufferSize_k__BackingField;
    int32_t _LimitOfUnreliableCommands_k__BackingField;
    int32_t WarningSize;
    uint8_t ClientSdkId;
    Il2CppString* m_ClientVersion;
    int32_t _SerializationProtocolType_k__BackingField;
    struct System_Collections_Generic_Dictionary_ConnectionProtocol__Type__o* SocketImplementationConfig;
    struct System_Type_o* _SocketImplementation_k__BackingField;
    uint8_t DebugOut;
    struct ExitGames_Client_Photon_IPhotonPeerListener_o* _Listener_k__BackingField;
    bool _EnableServerTracing_k__BackingField;
    uint8_t quickResendAttempts;
    int32_t RhttpMinConnections;
    int32_t RhttpMaxConnections;
    uint8_t ChannelCount;
    bool crcEnabled;
    int32_t SentCountAllowance;
    int32_t TimePingInterval;
    int32_t DisconnectTimeout;
    uint8_t _TransportProtocol_k__BackingField;
    int32_t mtu;
    bool _IsSendingOnlyAcks_k__BackingField;
    bool RandomizeSequenceNumbers;
    struct System_Byte_array* RandomizedSequenceNumbers;
    struct ExitGames_Client_Photon_TrafficStats_o* _TrafficStatsIncoming_k__BackingField;
    struct ExitGames_Client_Photon_TrafficStats_o* _TrafficStatsOutgoing_k__BackingField;
    struct ExitGames_Client_Photon_TrafficStatsGameLevel_o* _TrafficStatsGameLevel_k__BackingField;
    struct System_Diagnostics_Stopwatch_o* trafficStatsStopwatch;
    bool trafficStatsEnabled;
    struct ExitGames_Client_Photon_PeerBase_o* peerBase;
    Il2CppObject* SendOutgoingLockObject;
    Il2CppObject* DispatchLockObject;
    Il2CppObject* EnqueueLock;
    struct System_Byte_array* PayloadEncryptionSecret;
    struct ExitGames_Client_Photon_EncryptorManaged_Encryptor_o* DgramEncryptor;
    struct ExitGames_Client_Photon_EncryptorManaged_Decryptor_o* DgramDecryptor;
};

struct LoadBalancingPeer : PhotonPeer {
    struct System_Collections_Generic_Dictionary_byte__object__o* opParameters;
};

struct NetworkingPeer : LoadBalancingPeer {
    Il2CppString* AppId;
    struct AuthenticationValues* _AuthValues_k__BackingField;
    Il2CppString* tokenCache;
    int32_t AuthMode;
    int32_t EncryptionMode;
    bool _IsUsingNameServer_k__BackingField;
    Il2CppString* _MasterServerAddress_k__BackingField;
    Il2CppString* _GameServerAddress_k__BackingField;
    int32_t _Server_k__BackingField;
    int32_t _State_k__BackingField;
    bool IsInitialConnect;
    bool insideLobby;
    struct TypedLobby_o* _lobby_k__BackingField;
    struct System_Collections_Generic_List_TypedLobbyInfo__o* LobbyStatistics;
    struct System_Collections_Generic_Dictionary_string__RoomInfo__o* mGameList;
    struct RoomInfo_array* mGameListCopy;
    Il2CppString* playername;
    bool mPlayernameHasToBeUpdated;
    Room* m_CurrentRoom;
    struct PhotonPlayer_o* _LocalPlayer_k__BackingField;
    int32_t _PlayersOnMasterCount_k__BackingField;
    int32_t _PlayersInRoomsCount_k__BackingField;
    int32_t _RoomsCount_k__BackingField;
    int32_t lastJoinType;
    struct EnterRoomParams_o* enterRoomParamsCache;
    bool didAuthenticate;
    struct System_String_array* friendListRequested;
    int32_t friendListTimestamp;
    bool isFetchingFriendList;
    struct System_Collections_Generic_List_Region__o* _AvailableRegions_k__BackingField;
    int32_t _CloudRegion_k__BackingField;
    struct System_Collections_Generic_Dictionary_int__PhotonPlayer__o* mActors;
    struct PhotonPlayer_array* mOtherPlayerListCopy;
    struct PhotonPlayer_array* mPlayerListCopy;
    bool hasSwitchedMC;
    struct System_Collections_Generic_HashSet_byte__o* allowedReceivingGroups;
    struct System_Collections_Generic_HashSet_byte__o* blockSendingGroups;
    struct System_Collections_Generic_Dictionary_int__PhotonView__o* photonViewList;
    struct PhotonStream_o* readStream;
    struct PhotonStream_o* pStream;
    struct System_Collections_Generic_Dictionary_int__Hashtable__o* dataPerGroupReliable;
    struct System_Collections_Generic_Dictionary_int__Hashtable__o* dataPerGroupUnreliable;
    int16_t currentLevelPrefix;
    bool loadingLevelAndPausedNetwork;
    struct IPunPrefabPool_o* ObjectPool;
    struct System_Collections_Generic_Dictionary_Type__List_MethodInfo___o* monoRPCMethodsCache;
    struct System_Collections_Generic_Dictionary_string__int__o* rpcShortcuts;
    Il2CppString* cachedServerAddress;
    Il2CppString* cachedApplicationName;
    int32_t cachedServerType;
    struct UnityEngine_AsyncOperation_o* _AsyncLevelLoadingOperation;
    struct RaiseEventOptions_o* _levelReloadEventOptions;
    bool _isReconnecting;
    struct System_Collections_Generic_Dictionary_int__object____o* tempInstantiationData;
    struct RaiseEventOptions_o* options;
    bool IsReloadingLevel;
    bool AsynchLevelLoadCall;
};

struct PhotonNetworkStaticFields {
    struct Axlebolt_Logging_Abstractions_ILogger_o* _logger;
    Il2CppString* m_GameVersion;
    struct PhotonHandler_o* photonMono;
    NetworkingPeer* m_NetworkingPeer;
    int32_t MAX_VIEW_IDS;
    struct ServerSettings_o* PhotonServerSettings;
    bool InstantiateInRoomOnly;
    int32_t logLevel;
    struct System_Collections_Generic_List_FriendInfo__o* _Friends_k__BackingField;
    float precisionForVectorSynchronization;
    float precisionForQuaternionSynchronization;
    float precisionForFloatSynchronization;
    bool UseRpcMonoBehaviourCache;
    bool UsePrefabCache;
    struct System_Collections_Generic_Dictionary_string__GameObject__o* PrefabCache;
    struct System_Collections_Generic_HashSet_GameObject__o* SendMonoMessageTargets;
    Type* SendMonoMessageTargetType;
    bool StartRpcsAsCoroutine;
    bool isOfflineMode;
    Room* offlineModeRoom;
    int32_t maxConnections;
    bool _mAutomaticallySyncScene;
    bool m_autoCleanUpPlayerObjects;
    int32_t sendInterval;
    int32_t sendIntervalOnSerialize;
    bool m_isMessageQueueRunning;
    struct System_Diagnostics_Stopwatch_o* startupStopwatch;
    float BackgroundTimeout;
    bool _UseAlternativeUdpPorts_k__BackingField;
    struct System_Collections_Generic_Dictionary_byte__HashSet_PhotonNetwork_EventCallback___o* EventCallbacks;
    int32_t lastUsedViewSubId;
    int32_t lastUsedViewSubIdStatic;
    struct System_Collections_Generic_List_int__o* manuallyAllocatedViewIds;

    static PhotonNetworkStaticFields* GetInstance() {
        static PhotonNetworkStaticFields* instance = nullptr;
        if (instance == nullptr) {
            static Il2CppClass* klass = Il2CppUtils::FindKlass("", oxorany("PhotonNetwork"));
            instance = reinterpret_cast<PhotonNetworkStaticFields*>(klass->static_fields);
        }
        return instance;
    }
};

struct PlayerManager : Il2CppObject {
    PlayerController* GetLocalPlayer() {
        // return memory->read<PlayerController*>((uintptr_t)this + 0x3C);
        return *(PlayerController**)((uintptr_t)this + 0x3C);
    }

    std::vector<PlayerController*> GetPlayers() {
        auto playersDict = *(Dictionary<int, PlayerController*>**)((uintptr_t)this + 0x18);
        return playersDict->GetValues();
    }

    Dictionary<Il2CppString*, PlayerPool*>* GetPlayerPools() {
        return *(Dictionary<Il2CppString*, PlayerPool*>**)((uintptr_t)this + 0x40);
    }
};
