// functions.h
#pragma once
#include <cstdint>

// #include "../sdk/axlebolt_classes.h"
// #include "../il2cpp/il2cpp_structs.h"
#include "Vector3.h"

struct MethodInfo;
struct Il2CppObject;
template <typename T = Il2CppObject*>
struct Il2CppArray;
struct Il2CppThread;
struct Il2CppDomain;
struct Il2CppAssembly;
struct Il2CppImage;
struct Il2CppType;
struct Il2CppClass;
struct Il2CppException;
struct Il2CppString;

template <typename T = Il2CppObject*>
struct HashSet;
template <typename TKey, typename TValue>
struct Dictionary;
struct Transform;
struct PlayerCharacterView;
struct Matrix4x4;
struct GameObject;
struct InventoryManager;
struct WeaponMaterialController;
struct WeaponController;
struct RaycastHit;
struct InventoryService;
struct Component;
struct Transform;
struct Camera;
struct PhotonPlayer;
struct PlayerController;
struct Collider;
struct Material;
struct Shader;
struct Renderer;
struct Color;
struct Scene;
struct AnimationCurve;
struct Texture;
struct Bounds;
struct Hashtable;

typedef void (*Il2CppMethodPointer)();
struct VirtualInvokeData {
    Il2CppMethodPointer* methodPtr;
    MethodInfo* method;
};

struct AssetBundle;
struct Type;
struct AssetBundleCreateRequest;
struct AssetBundleRequest;
struct AsyncOperation;
struct Object;
struct FieldInfo;
struct Room;

#define RESOLVE_BY_OFFSET(return_type, name, offset, params) \
    typedef return_type (*name##_t)params;                                                               \
    extern name##_t name;                                                                                \

#define RESOLVE_BY_NAME_AND_PARAMS(return_type, name, namespace, klass, methodName, params, paramsCount, ...) \
    typedef return_type (*name##_t)params;                                                               \
    extern name##_t name;                                                                                \
    static const std::initializer_list<std::string> name##_paramTypes = { __VA_ARGS__ };

#define RESOLVE_BY_NAME(return_type, name, namespace, klass, methodName, params) \
    typedef return_type (*name##_t)params; \
    extern name##_t name;

#define RESOLVE_BY_SIGNATURE(return_type, name, params) \
    typedef return_type(*name##_t)params; \
    extern name##_t name;

#include "il2cpp_api.h"
#include "functions_list.h"

#undef RESOLVE_BY_NAME_AND_PARAMS
#undef RESOLVE_BY_NAME
#undef RESOLVE_BY_SIGNATURE
#undef RESOLVE_BY_OFFSET

void InitFunctionPointers(uintptr_t baseAddress, size_t libSize);
