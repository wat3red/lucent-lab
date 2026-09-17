#pragma once
#include <stdint.h>

enum ViewMode : int32_t
{
    None = 0,
    FPS = 1,
    TPS = 2
};

enum class ViewSynchronization : int32_t
{
    Off = 0,
    ReliableDeltaCompressed = 1,
    Unreliable = 2,
    UnreliableOnChange = 3
};

enum class MoveState
{
    NotStated = 0,
    Crouch = 1,
    Stand = 2,
    FinishedCrouch = 3,
    FinishedStand = 4
};

enum class WeaponOffsetState
{
    NotStated = 0,
    Translation = 1
};

enum class SurfaceType : int
{
    Unknown = 0,
    Glass = 1,
    Cardboard = 2,
    MetalGrate = 3,
    Wood = 4,
    Plaster = 5,
    Tile = 6,
    Metal = 7,
    Concrete = 8,
    Brick = 9,
    SolidMetal = 10,
    ThinMetal = 11,
    ThinWood = 12,
    Character = 13,
    Ground = 14,
    SurfaceMarkTrigger = 15,
    SnowConcrete = 16,
    SnowMetal = 17,
    SnowGround = 18,
    SolidMetalNoHole = 19,
    Gravel = 20,
    Grass = 21,
    Paper = 22,
    Water = 23,
    GlassWaterfall = 24,
    WoodSolid = 25,
    GrassNoDecal = 26,
    Smoke = 27
};

enum class BombState : uint8_t {
    None = 0,
    Planting = 1,
    Switching = 2,
    TryToSwitch = 3,
    Ready = 4
};

enum class DefuseKitState : uint8_t {
    None = 0,
    Defusing = 1
};

enum class GunState : int32_t
{
    Reloading = 0,
    PreReload = 1,
    Switching = 2,
    TryToSwitch = 3,
    Ready = 4
};

enum class AimingMode : int32_t
{
    Aiming = 0,
    AimingReload = 1,
    NotAiming = 2,
    StartingAiming = 3,
    FinishingAiming = 4
};

enum class WeaponType : uint8_t {
    None = 0,
    Pistol = 1,
    Smg = 2,
    Rifle = 3,
    Shotgun = 4,
    Heavy = 5,
    Sniper = 6,
    Grenade = 7,
    Knife = 8,
};

enum class SightType : int32_t {
    Default = 0,
    CollimatorSight = 1,
    SniperScope = 2,
};

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

enum class DetonationType : uint8_t {
    ByTimer = 0,
    AfterStoping = 1,
    OneHit = 2,
    OneHitFloor = 3
};

enum class HideFlags
{
    None,
    HideInHierarchy,
    HideInInspector,
    DontSaveInEditor,
    NotEditable,
    DontSaveInBuild,
    DontUnloadUnusedAsset,
    DontSave,
    HideAndDontSave
};

enum class TranslationStates : int32_t
{
    Jump = 1,
    Walk = 2,
    Idle = 3,
    Crouch = 4
};

enum class Team : uint8_t {
    None = 0,
    Tr = 1,
    Ct = 2,
    Spec = 3,
};

enum class BoneID : int32_t
{
    Head = 0,
    Neck = 1,
    Spine1 = 2,
    Spine2 = 3,
    LeftUpperarm = 4,
    LeftForearm = 5,
    LeftHand = 6,
    LeftShoulder = 7,
    RightShoulder = 8,
    RightUpperarm = 9,
    RightForearm = 10,
    RightHand = 11,
    Hip = 12,
    LeftUpLeg = 13,
    LeftLeg = 14,
    LeftFoot = 15,
    RightUpLeg = 16,
    RightLeg = 17,
    RightFoot = 18,
    Spine = 19,
    LeftToeBase = 20,
    RightToeBase = 21,
    LeftInHandIndex = 22,
    LeftHandIndex1 = 23,
    LeftHandIndex2 = 24,
    LeftHandIndex3 = 25,
    LeftHandIndex4 = 26,
    LeftInHandMiddle = 27,
    LeftHandMiddle1 = 28,
    LeftHandMiddle2 = 29,
    LeftHandMiddle3 = 30,
    LeftHandMiddle4 = 31,
    LeftInHandPinky = 32,
    LeftHandPinky1 = 33,
    LeftHandPinky2 = 34,
    LeftHandPinky3 = 35,
    LeftHandPinky4 = 36,
    LeftInHandRing = 37,
    LeftHandRing1 = 38,
    LeftHandRing2 = 39,
    LeftHandRing3 = 40,
    LeftHandRing4 = 41,
    LeftInHandThumb = 42,
    LeftHandThumb1 = 43,
    LeftHandThumb2 = 44,
    LeftHandThumb3 = 45,
    RightInHandIndex = 46,
    RightHandIndex1 = 47,
    RightHandIndex2 = 48,
    RightHandIndex3 = 49,
    RightHandIndex4 = 50,
    RightInHandMiddle = 51,
    RightHandMiddle1 = 52,
    RightHandMiddle2 = 53,
    RightHandMiddle3 = 54,
    RightHandMiddle4 = 55,
    RightInHandPinky = 56,
    RightHandPinky1 = 57,
    RightHandPinky2 = 58,
    RightHandPinky3 = 59,
    RightHandPinky4 = 60,
    RightInHandRing = 61,
    RightHandRing1 = 62,
    RightHandRing2 = 63,
    RightHandRing3 = 64,
    RightHandRing4 = 65,
    RightInHandThumb = 66,
    RightHandThumb1 = 67,
    RightHandThumb2 = 68,
    RightHandThumb3 = 69,
};

inline const char* GetBoneName(BoneID bone)
{
    switch (bone)
    {
    case BoneID::Head: return oxorany("Head");
    case BoneID::Neck: return oxorany("Neck");
    case BoneID::Spine:
    case BoneID::Spine1:
    case BoneID::Spine2: return oxorany("Spine");
    case BoneID::LeftUpperarm: return oxorany("Left Upper Arm");
    case BoneID::LeftForearm: return oxorany("Left Forearm");
    case BoneID::LeftHand: return oxorany("Left Hand");
    case BoneID::LeftShoulder: return oxorany("Left Shoulder");
    case BoneID::RightShoulder: return oxorany("Right Shoulder");
    case BoneID::RightUpperarm: return oxorany("Right Upper Arm");
    case BoneID::RightForearm: return oxorany("Right Forearm");
    case BoneID::RightHand: return oxorany("Right Hand");
    case BoneID::Hip: return oxorany("Hip");
    case BoneID::LeftUpLeg: return oxorany("Left Upper Leg");
    case BoneID::LeftLeg: return oxorany("Left Leg");
    case BoneID::LeftFoot: return oxorany("Left Foot");
    case BoneID::RightUpLeg: return oxorany("Right Upper Leg");
    case BoneID::RightLeg: return oxorany("Right Leg");
    case BoneID::RightFoot: return oxorany("Right Foot");
    case BoneID::LeftToeBase: return oxorany("Left Toe Base");
    case BoneID::RightToeBase: return oxorany("Right Toe Base");
    case BoneID::LeftInHandIndex: return oxorany("Left In-Hand Index");
    case BoneID::LeftHandIndex1:
    case BoneID::LeftHandIndex2:
    case BoneID::LeftHandIndex3:
    case BoneID::LeftHandIndex4:
    case BoneID::LeftInHandMiddle:
    case BoneID::LeftHandMiddle1:
    case BoneID::LeftHandMiddle2:
    case BoneID::LeftHandMiddle3:
    case BoneID::LeftHandMiddle4:
    case BoneID::LeftInHandPinky:
    case BoneID::LeftHandPinky1:
    case BoneID::LeftHandPinky2:
    case BoneID::LeftHandPinky3:
    case BoneID::LeftHandPinky4:
    case BoneID::LeftInHandRing:
    case BoneID::LeftHandRing1:
    case BoneID::LeftHandRing2:
    case BoneID::LeftHandRing3:
    case BoneID::LeftHandRing4:
    case BoneID::LeftInHandThumb:
    case BoneID::LeftHandThumb1:
    case BoneID::LeftHandThumb2:
    case BoneID::LeftHandThumb3: return oxorany("Left Hand");
    case BoneID::RightInHandIndex:
    case BoneID::RightHandIndex1:
    case BoneID::RightHandIndex2:
    case BoneID::RightHandIndex3:
    case BoneID::RightHandIndex4:
    case BoneID::RightInHandMiddle:
    case BoneID::RightHandMiddle1:
    case BoneID::RightHandMiddle2:
    case BoneID::RightHandMiddle3:
    case BoneID::RightHandMiddle4:
    case BoneID::RightInHandPinky:
    case BoneID::RightHandPinky1:
    case BoneID::RightHandPinky2:
    case BoneID::RightHandPinky3:
    case BoneID::RightHandPinky4:
    case BoneID::RightInHandRing:
    case BoneID::RightHandRing1:
    case BoneID::RightHandRing2:
    case BoneID::RightHandRing3:
    case BoneID::RightHandRing4:
    case BoneID::RightInHandThumb:
    case BoneID::RightHandThumb1:
    case BoneID::RightHandThumb2:
    case BoneID::RightHandThumb3: return oxorany("Right Hand");
    default: return oxorany("Unknown Bone");
    }
}