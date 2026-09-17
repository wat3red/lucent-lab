#pragma once

#include <vector>

#include "../il2cpp/il2cpp_structs.h"
#include "utils.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "functions.h"
#include "world_to_screen.h"

#include <string>
#include <codecvt>
#include <locale>
// #pragma pack(push, 1)

struct Bounds {
    Vector3 m_Center;
    Vector3 m_Extents;
};

template <typename T>
struct Il2CppArray : Il2CppObject {
    Il2CppArrayBounds* bounds;
    uintptr_t size;
    T items[65535];
};

template <typename T>
struct List : Il2CppObject {
    Il2CppArray<T>* items;
    int32_t size;
    int32_t version;
    Il2CppObject* syncRoot;

    std::vector<T> GetValues()
    {
        std::vector<T> values;

        for (int i = 0; i < size; i++)
            values.push_back(items->items[i]);

        return values;
    }
};

struct Object : Il2CppObject {
    void* m_CachedPtr;

    inline void DontDestroyOnLoad() {
        Object_DontDestroyOnLoad(this);
    }

    inline Il2CppString* GetName() {
        return Object_GetName(this);
    }

    static inline Object* Instantiate(Object* original) {
        return Object_Instantiate(original);
    }

    inline void Destroy() {
        Object_Destroy(this);
    }


    inline void SetHideFlags(int value) {
        Object_set_hideFlags(this, value);
    }
};

struct Component : Object {
    Transform* GetTransform() {
        return Component_get_transform(this);
    }

    Il2CppString* GetTag() {
        return Component_get_tag(this);
    }
};

namespace UnityEngine {
    struct Behaviour : Component {};

    struct MonoBehaviour : Behaviour {
        struct System_Threading_CancellationTokenSource* m_CancellationTokenSource;
    };
};

struct Type {
    static Type* GetType(Il2CppString* typeName) {
        if (typeName)
            return Type_GetType(typeName);
        return nullptr;
    }
};

struct AsyncOperation : Il2CppObject {
    inline bool IsDone() {
        return AsyncOperation_get_isDone(this);
    }
};

struct AssetBundleRequest : AsyncOperation {
    inline Il2CppObject* GetResult() {
        return AssetBundleRequest_GetResult(this);
    }

    inline Il2CppArray<Il2CppObject*>* GetAllLoadedAssets() {
        return AssetBundleRequest_get_allAssets(this);
    }
};

struct AssetBundleCreateRequest : AsyncOperation {
    inline AssetBundle* GetAssetBundle() {
        return AssetBundleCreateRequest_get_assetBundle(this);
    }
};

struct Delegate : Il2CppObject {
    intptr_t method_ptr;
    void* invoke_impl;
    Il2CppObject* m_target;
    intptr_t method;
    intptr_t delegate_trampoline;
    intptr_t extra_arg;
    intptr_t method_code;
    intptr_t interp_method;
    intptr_t interp_invoke_impl;
    MethodInfo* method_info;
    MethodInfo* original_method_info;
    struct System_DelegateData_o* data;
    bool method_is_virtual;
};

struct MulticastDelegate : Delegate {
    Il2CppArray<Delegate*>* delegates;
};

struct Action : MulticastDelegate {
};

template <typename TKey, typename TValue>
struct Dictionary : Il2CppObject {
    struct Entry {
        int hashCode;
        int next;
        TKey key;
        TValue value;
    };

    Il2CppArray<int>* buckets;
    Il2CppArray<Entry>* entries;
    int count;
    int version;
    int freeList;
    int freeCount;
    void* compare;
    Il2CppArray<TKey>* keys;
    Il2CppArray<TValue>* values;
    void* syncRoot;

    std::vector<TValue> GetValues()
    {
        std::vector<TValue> values;

        for (int i = 0; i < count; i++)
        {
            values.push_back(entries->items[i].value);
        }

        return values;
    }

    std::vector<TKey> GetKeys()
    {
        std::vector<TKey> keys;
        for (int i = 0; i < count; i++)
        {
            keys.push_back(entries->items[i].key);
        }

        return keys;
    }

    TValue GetValue(TKey key)
    {
        for (int i = 0; i < count; i++)
        {
            if (entries->items[i].key == key)
            {
                return entries->items[i].value;
            }
        }

        return NULL;
    }

    void SetValue(TKey key, TValue value)
    {
        for (int i = 0; i < entries->size; i++)
        {
            if (entries->items[i].key == key)
            {
                entries->items[i].value = value;
            }
        }
    }

    std::string ToString() const
    {
        if (count <= 0)
        {
            return oxorany("{}");
        }

        std::stringstream ss;
        ss << oxorany("{");

        bool isFirst = true;
        for (int i = 0; i < entries->size; i++)
        {
            // Skip empty entries
            if (entries->items[i].hashCode >= 0)
            {
                if (!isFirst)
                {
                    ss << oxorany(", ");
                }

                ss << entries->items[i].key << oxorany(": ") << entries->items[i].value;
                isFirst = false;
            }
        }

        ss << oxorany("}");
        return ss.str();
    }
};

struct Il2CppString : Il2CppObject
{
    int length;
    uint16_t firstChar;

    static Il2CppString* CreateString(const char* string) {
        return il2cpp_string_new(string);
    }

    const char* c_str() {
        return Marshal_StringToHGlobalAnsi(this);
    };
};

struct Hashtable
{
    template<typename T>
    T GetValue(const char* key)
    {
        return *(T*)((uintptr_t)Photon_Hashtable_get_Item(this, Il2CppString::CreateString(key)) + sizeof(Il2CppObject));
    }

    template<typename T>
    void SetValue(const char* key, T value)
    {
        Photon_Hashtable_set_Item(this, Il2CppString::CreateString(key), value);
    }
};

struct Color
{
    float r;
    float g;
    float b;
    float a;

    Color() { r = 0; g = 0; b = 0; a = 0; }
    Color(float value[4]) { r = value[0]; g = value[1]; b = value[2]; a = value[3]; }
    Color(const float value[4]) { r = value[0]; g = value[1]; b = value[2]; a = value[3]; }
    Color(float _r, float _g, float _b, float _a) { r = _r; g = _g; b = _b; a = _a; }
};

struct AnimationCurve {
    void ClearKeys() {
        AnimationCurve_ClearKeys(this);
    }
};

struct Transform {
    Vector3 GetPosition() {
        Vector3 position;
        Transform_get_position(&position, this);
        return position;
    }

    Vector3 GetLocalPosition() {
        Vector3 position;
        Transform_get_localPosition(&position, this);
        return position;
    }

    Vector3 GetLocalEulerAngles() {
        Vector3 angles;
        Transform_get_localEulerAngles(&angles, this);
        return angles;
    }

    void SetPosition(Vector3 position) {
        Transform_set_position(this, position);
    }

    void SetLocalScale(Vector3 position) {
        Transform_set_localScale(this, position);
    }

    void SetLocalPosition(Vector3 position) {
        Transform_set_localPosition(this, position);
    }

    void SetLocalEulerAngles(Vector3 angles) {
        Transform_set_localEulerAngles(this, angles);
    }

    void SetRotation(Quaternion rotation) {
        Transform_set_rotation(this, rotation);
    }

    void SetEulerAngles(Vector3 value) {
        Transform_set_eulerAngles(this, value);
    }

    Vector3 TransformPoint(Vector3 position) {
        return Transform_TransformPoint(this, position);
    }

    Vector3 GetEulerAngles() {
        Vector3 res;
        Transform_get_eulerAngles(&res, this);
        return res;
    }

    Vector3 GetRight() {
        Vector3 res;
        Transform_get_right(&res, this);
        return res;
    }

    Vector3 GetForward() {
        Vector3 res;
        Transform_get_forward(&res, this);
        return res;
    }

    Vector3 GetUp() {
        Vector3 res;
        Transform_get_up(&res, this);
        return res;
    }

    void LookAt(Vector3 pos) {
        Transform_LookAt(this, pos);
    }

    Quaternion GetRotation() {
        Quaternion rotation;
        Transform_get_rotation(&rotation, this);
        return rotation;
    }
};

struct Screen {
    static int GetHeight() {
        return Screen_get_height();
    }

    static int GetWidth() {
        return Screen_get_width();
    }
};

struct Matrix4x4 {
    float value[16];
};

struct Transform;


struct ParameterOverride : Il2CppObject {
    bool overrideState;
};

struct BoolParameter : ParameterOverride {
    bool value;
};

struct FloatParameter : ParameterOverride {
    float value;
};

struct ColorParameter : ParameterOverride {
    Color value;
};

struct TextureParameter : ParameterOverride {
    void* value;
};

struct PostProcessEffectSettings : Il2CppObject {
    void* m_CachedPtr;
    bool active; // 0xC
    BoolParameter enabled; // 0x10
    void* parameters; // 0x14
};

struct Bloom : PostProcessEffectSettings {
    // [Tooltip(oxorany("Strength of the bloom filter. Values higher than 1 will make bloom contribute more energy to the final render."))]
    // [Min(0)]
    FloatParameter* intensity; // 0x18
    // [Min(0)]
    // [Tooltip(oxorany("Filters out pixels under this level of brightness. Value is in gamma-space."))]
    FloatParameter* threshold; // 0x1C
    // [Tooltip(oxorany("Makes transitions between under/over-threshold gradual. 0 for a hard threshold, 1 for a soft threshold)."))]
    // [Range(0, 1)]
    FloatParameter* softKnee; // 0x20
    // [Tooltip(oxorany("Clamps pixels to control the bloom amount. Value is in gamma-space."))]
    FloatParameter* clamp; // 0x24
    // [Range(1, 10)]
    // [Tooltip(oxorany("Changes the extent of veiling effects. For maximum quality, use integer values. Because this value changes the internal iteration count, You should not animating it as it may introduce issues with the perceived radius."))]
    FloatParameter* diffusion; // 0x28
    // [Range(-1, 1)]
    // [Tooltip(oxorany("Distorts the bloom to give an anamorphic look. Negative values distort vertically, positive values distort horizontally."))]
    FloatParameter* anamorphicRatio; // 0x2C
    // [ColorUsage(False, True)]
    // [Tooltip(oxorany("Global tint of the bloom filter."))]
    ColorParameter* color; // 0x30
    // [FormerlySerializedAs(oxorany("mobileOptimized"))]
    // [Tooltip(oxorany("Boost performance by lowering the effect quality. This settings is meant to be used on mobile and other low-end platforms but can also provide a nice performance boost on desktops and consoles."))]
    BoolParameter* fastMode; // 0x34
    // [Tooltip(oxorany("The lens dirt texture used to add smudges or dust to the bloom effect."))]
    // [DisplayName(oxorany("Texture"))]
    TextureParameter* dirtTexture; // 0x38
    // [Min(0)]
    // [Tooltip(oxorany("The intensity of the lens dirtiness."))]
    // [DisplayName(oxorany("Intensity"))]
    FloatParameter* dirtIntensity; // 0x3C
};

inline float overrideFov = 60;
struct Camera : Component {
    Component* GetComponent() {
        return (Component*)this;
    }

    Vector3 WorldToScreen(Vector3 position) {
        Transform* transform = GetTransform();
        return WorldToScreen_Internal(position, transform->GetPosition(), transform->GetRotation(), GetFOV());
    }

    void SetClearFlags(int value) {
        Camera_set_clearFlags(this, value);
    }

    void SetBackgroundColor(Color color) {
        Camera_set_backgroundColor(this, color);
    }

    void SetAllowHDR(bool value) {
        Camera_set_allowHDR(this, value);
    }

    float GetFOV() {
        // if (overrideFov == )
            // return atan(1.0f / Camera::GetMain()->GetProjectionMatrix().value[5]) * 2.0f * (360 / (3.14159265359 * 2));
        return overrideFov;
    }

    // Matrix4x4 GetProjectionMatrix() {
    //     Matrix4x4 output;
    //     Camera_get_projectionMatrix(&output, this);
    //     return output;
    // }

    static Camera* GetMain() {
        return Camera_get_main();
    }

    void SetFOV(float value) {
        overrideFov = value;
        // if (value == -1) {
            // overrideFov = 60;
            //return Camera_set_fieldOfView(this, 60);
        // }
        Camera_set_fieldOfView(this, overrideFov);
    }
};


struct Time : Il2CppObject {
    static inline float GetDeltaTime() {
        return Time_get_deltaTime();
    }

    static inline float GetTime() {
        return Time_get_time();
    }
};

struct GameObject : Object {
    // Il2CppArray<Component*>* GetComponents() {
    //     static Il2CppClass* gameObjectKlass = Il2CppUtils::FindKlass(oxorany("UnityEngine"), oxorany("GameObject"));
    //     static MethodInfo* getComponentsMethod = FindMethod(gameObjectKlass, oxorany("GetComponents"), 0);
    //     return (Il2CppArray<Component*>*)GameObject_GetComponents(this, getComponentsMethod);
    // }

    inline Component* AddComponent(Type* componentType) {
        return GameObject_AddComponent(this, componentType);
    }

    inline void Initialize(Il2CppString* name) {
        GameObject_Internal_CreateGameObject(this, name);
    }

    inline Transform* GetTransform() {
        return GameObject_get_transform(this);
    }

    void SetActive(bool value) {
        GameObject_SetActive(this, value);
    }

    inline int GetLayer() {
        return GameObject_get_layer(this);
    }

    static Il2CppArray<GameObject*>* GetAllGameObjects() {
        static Type* gameObjectType = Type::GetType(Il2CppString::CreateString(oxorany("UnityEngine.GameObject, UnityEngine.CoreModule")));
        return (Il2CppArray<GameObject*>*)Object_FindObjectsByType(gameObjectType, 0, 0);
    }

    // static GameObject* Find(const char* name) {
    //     return GameObject_Find(Il2CppString::CreateString(name));
    // }
};

struct Ray {
public:
    Vector3 m_VecOrigin;
    Vector3 m_VecDirection;

    Ray() = default;
    ~Ray() = default;

    Ray(const Vector3& vecOrigin, const Vector3& vecDirection) {
        this->m_VecOrigin = vecOrigin;
        this->m_VecDirection = vecDirection;
    }
};

struct Collider : Component {
    Bounds GetBounds() {
        Bounds bounds;
        Collider_get_bounds(&bounds, this);
        return bounds;
    }

    void SetEnabled(bool value) {
        Collider_set_enabled(this, value);
    }
};

struct CharacterController : Collider {
};

struct CapsuleCollider : Collider {
};

struct RaycastHit {
    Vector3 m_Point;
    Vector3 m_Normal;
    uint32_t m_FaceID;
    float m_Distance;
    Vector2 m_UV;
    int32_t m_Collider;

    Collider* GetCollider() {
        return RaycastHit_get_collider(this);
    }

    std::string ToString() {
        return (oxorany("m_Point: ") + m_Point.ToString() +
            oxorany(", m_Normal: ") + m_Normal.ToString() +
            oxorany(", m_FaceID: ") + std::to_string(m_FaceID) +
            oxorany(", m_Distance: ") + std::to_string(m_Distance) +
            oxorany(", m_UV: ") + m_UV.ToString() +
            oxorany(", m_Collider: ") + std::to_string(m_Collider));
    }
};
static_assert(offsetof(RaycastHit, m_Collider) == 0x28, "CHECK RaycastHit FIELD OFFSETS!");

struct Scene {
    int m_Handle;
};

struct Shader : Object {
    static Shader* FindShaderByName(Il2CppString* shaderName) {
        if (shaderName)
            return ResourcesAPIInternal_FindShaderByName(shaderName);
        return nullptr;
    }

};

struct Renderer : Component {
    inline void SetSharedMaterial(Material* mat) {
        return Renderer_set_sharedMaterial(this, mat);
    }

    inline Material* GetSharedMaterial() {
        return Renderer_GetSharedMaterial(this);
    }

    inline Material* GetMaterial() {
        return Renderer_GetMaterial(this);
    }

    inline Il2CppArray<Material*>* GetMaterials() {
        return Renderer_GetMaterialArray(this);
    }

    inline void SetMaterials(Il2CppArray<Material*>* array) {
        return Renderer_set_materials(this, array);
    }

    inline void SetMaterial(Material* m) {
        return Renderer_SetMaterial(this, m);
    }
};

struct AssetBundle : Object {
    inline AssetBundleRequest* LoadAllAssetsAsync(Type* type) {
        return AssetBundle_LoadAllAssetsAsync(this, type);
    }

    inline AssetBundleRequest* LoadAssetAsync(Il2CppString* name, Type* type) {
        return AssetBundle_LoadAssetAsync(this, name, type);
    }

    inline void Unload(bool unloadAllLoadedObjects) {
        AssetBundle_Unload(this, unloadAllLoadedObjects);
    }

    static inline AssetBundleCreateRequest* LoadFromFileAsync(Il2CppString* path) {
        return AssetBundle_LoadFromFileAsync(path);
    }
};

template <typename T>
struct HashSet : Il2CppObject {
    Il2CppArray<int32_t>* _buckets;
    void* _slots;
    int32_t _count;
    int32_t _lastIndex;
    int32_t _freeList;
    void* _comparer;
    int32_t _version;
    void* _siInfo;

    void Clear() {
        static MethodInfo* clearMethod = Il2CppUtils::FindMethod(klass, oxorany("Clear"));
        typedef void(*HashSet_object_Clear_t)(HashSet<T>*);
        ((HashSet_object_Clear_t)(clearMethod->methodPointer))(this);
    }
};

struct Material : Object {
    bool HasProperty(Il2CppString* name) {
        return Material_HasProperty(this, name);
    }

    static Material* Create(Il2CppString* shaderName) {
        Shader* shader = Shader::FindShaderByName(shaderName);
        if (!shader) return nullptr;

        static Il2CppClass* materialKlass = Il2CppUtils::FindKlass(oxorany("UnityEngine"), oxorany("Material"));

        Material* newMaterial = (Material*)il2cpp_object_new(materialKlass);
        if (!newMaterial) return nullptr;

        Material_ctor(newMaterial, shader);

        return newMaterial;
    }

    static Material* Create(Shader* shader) {
        static Il2CppClass* materialKlass = Il2CppUtils::FindKlass(oxorany("UnityEngine"), oxorany("Material"));

        Material* newMaterial = (Material*)il2cpp_object_new(materialKlass);
        if (!newMaterial) return nullptr;

        Material_ctor(newMaterial, shader);

        return newMaterial;
    }

    inline Shader* GetShader() {
        return Material_get_shader(this);
    }

    inline void SetShader(Shader* shader) {
        return Material_set_shader(this, shader);
    }

    inline Texture* GetTexture(Il2CppString* name) {
        return Material_GetTexture(this, name);
    }

    inline void SetTexture(Il2CppString* name, Texture* value) {
        Material_SetTexture(this, name, value);
    }

    inline Color GetColor(Il2CppString* name) {
        Color ret;
        Material_GetColor(&ret, this, name);
        return ret;
    }

    inline void SetColor(Il2CppString* name, Color color) {
        Material_SetColor(this, name, color);
    }

    inline void SetInt(Il2CppString* name, int value) {
        Material_SetInt(this, name, value);
    }

    inline void SetFloat(Il2CppString* name, float value) {
        Material_SetFloat(this, name, value);
    }

    inline void SetFloat(const char* name, float value) {
        Material_SetFloat(this, Il2CppString::CreateString(name), value);
    }

    inline void SetColor(const char* name, Color color) {
        Material_SetColor(this, Il2CppString::CreateString(name), color);
    }

    inline void SetInt(const char* name, int value) {
        Material_SetInt(this, Il2CppString::CreateString(name), value);
    }
};

// #pragma pack(pop)