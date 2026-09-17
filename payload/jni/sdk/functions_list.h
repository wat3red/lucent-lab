// ========== BEEBYTE_CONSTANTS =========================
// was written for 0.34.1
/*
ищешь сначала класс с двумя "private Dictionary<int, float> <", затем ищешь "private Dictionary<int, имя_найденного_класса", это INVENTORY_CLASS_NAME
BOLT_ITEM_CLASS_NAME это тип листа на смещении 0x68(32 бит)
*/

#define INVENTORY_CLASS_NAME xorstr_("FDGGBHBDCHGABFH")
#define BOLT_ITEM_CLASS_NAME xorstr_("AFAFGBGHCDEHGGA")

// ========== BEEBYTE_CONSTANTS =========================

// "UnityEngine.PhysicsScene::Internal_Raycast_Injected(UnityEngine.PhysicsScene&,UnityEngine.Ray&", XREFS, then take this off_xxx
// v7 = off_xxx;
// if (!off_xxx)
// {
//     v7 = (void*)sub_1B25F98((int)"UnityEngine.PhysicsScene::Internal_Raycast_Injected(UnityEngine.PhysicsScene&,UnityEng"
//         "ine.Ray&,System.Single,UnityEngine.RaycastHit&,System.Int32,UnityEngine.QueryTriggerInteraction)");
//     off_xxx = v7;
// }
#define RAYCAST_HOOK_ADDRESS oxorany(0x72321A4)
#define ISGROUNDED_HOOK_ADDRESS oxorany(0x7231FF4) // "UnityEngine.CharacterController::get_isGrounded()"

RESOLVE_BY_NAME(void, Collider_get_bounds, xorstr_("UnityEngine"), xorstr_("Collider"), xorstr_("get_bounds"), (Bounds* result, Collider* instance))
RESOLVE_BY_NAME(void, Collider_set_enabled, xorstr_("UnityEngine"), xorstr_("Collider"), xorstr_("set_enabled"), (Collider* result, bool value))

RESOLVE_BY_NAME(void, Transform_get_position, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("get_position"), (Vector3* result, Transform* instance))
RESOLVE_BY_NAME(void, Transform_get_localPosition, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("get_localPosition"), (Vector3* result, Transform* instance))
RESOLVE_BY_NAME(void, Transform_get_localEulerAngles, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("get_localEulerAngles"), (Vector3* result, Transform* instance))
// public void set_position(Vector3 value) { }
RESOLVE_BY_NAME(void, Transform_set_position, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("set_position"), (Transform* instance, Vector3 value))
RESOLVE_BY_NAME(void, Transform_set_localPosition, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("set_localPosition"), (Transform* instance, Vector3 value))
RESOLVE_BY_NAME(void, Transform_set_localEulerAngles, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("set_localEulerAngles"), (Transform* instance, Vector3 value))
RESOLVE_BY_NAME(void, Transform_set_localScale, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("set_localScale"), (Transform* instance, Vector3 value))
// public Quaternion get_rotation() { }
RESOLVE_BY_NAME(void, Transform_get_rotation, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("get_rotation"), (Quaternion* result, Transform* instance))
// public void set_rotation(Quaternion value) { }
RESOLVE_BY_NAME(void, Transform_set_rotation, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("set_rotation"), (Transform* instance, Quaternion value))
RESOLVE_BY_NAME(void, Transform_set_eulerAngles, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("set_eulerAngles"), (Transform* instance, Vector3 value))
RESOLVE_BY_NAME(void, Transform_get_eulerAngles, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("get_eulerAngles"), (Vector3* result, Transform* instance))
// public Vector3 get_forward() { }
RESOLVE_BY_NAME(void, Transform_get_forward, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("get_forward"), (Vector3* result, Transform* instance))
// public Vector3 get_right() { }
RESOLVE_BY_NAME(void, Transform_get_right, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("get_right"), (Vector3* result, Transform* instance))
// public Vector3 get_up() { }
RESOLVE_BY_NAME(void, Transform_get_up, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("get_up"), (Vector3* result, Transform* instance))
// public void LookAt(Vector3 worldPosition) { }
RESOLVE_BY_NAME_AND_PARAMS(void, Transform_LookAt, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("LookAt"), (Transform* instance, Vector3 worldPosition), 1, xorstr_("UnityEngine.Vector3"))
// public Vector3 TransformPoint(Vector3 position) { }
RESOLVE_BY_NAME(Vector3, Transform_TransformPoint, xorstr_("UnityEngine"), xorstr_("Transform"), xorstr_("TransformPoint"), (Transform* instance, Vector3 position))


// [FreeFunction(xorstr_("GetSceneManager().DontDestroyOnLoad"), ThrowsException = True)]
RESOLVE_BY_NAME(void, Object_DontDestroyOnLoad, xorstr_("UnityEngine"), xorstr_("Object"), xorstr_("DontDestroyOnLoad"), (Object* target))
// public static Object Instantiate(Object original) { }
RESOLVE_BY_NAME(Object*, Object_Instantiate, xorstr_("UnityEngine"), xorstr_("Object"), xorstr_("Instantiate"), (Object* original))
// public void set_hideFlags(HideFlags value) { }
RESOLVE_BY_NAME(void, Object_set_hideFlags, xorstr_("UnityEngine"), xorstr_("Object"), xorstr_("set_hideFlags"), (Object* instance, int value))
// [FreeFunction(xorstr_("UnityEngineObjectBindings::GetName"))]
RESOLVE_BY_NAME(Il2CppString*, Object_GetName, xorstr_("UnityEngine"), xorstr_("Object"), xorstr_("GetName"), (Object* obj))

// public int get_layer() { }
RESOLVE_BY_NAME(int, GameObject_get_layer, xorstr_("UnityEngine"), xorstr_("GameObject"), xorstr_("get_layer"), (GameObject* instance))
// [FreeFunction(Name = xorstr_("GameObjectBindings::Internal_CreateGameObject"))]
RESOLVE_BY_NAME(void, GameObject_Internal_CreateGameObject, xorstr_("UnityEngine"), xorstr_("GameObject"), xorstr_("Internal_CreateGameObject"), (GameObject* self, Il2CppString* name))
// public Component AddComponent(Type componentType) { }
RESOLVE_BY_NAME(Component*, GameObject_AddComponent, xorstr_("UnityEngine"), xorstr_("GameObject"), xorstr_("AddComponent"), (GameObject* instance, Type* componentType))
// [FreeFunction(xorstr_("GameObjectBindings::GetTransform"), HasExplicitThis = True)]
RESOLVE_BY_NAME(Transform*, GameObject_get_transform, xorstr_("UnityEngine"), xorstr_("GameObject"), xorstr_("get_transform"), (GameObject* instance))
// |-GameObject.GetComponents<object>
// RESOLVE_BY_NAME(Il2CppArray<void*>*, GameObject_GetComponents, xorstr_("UnityEngine"), xorstr_("GameObject"), xorstr_("GetComponents"), (GameObject* instance, MethodInfo* method))
// [NativeMethod(Name = xorstr_("SetSelfActive"))]
RESOLVE_BY_NAME(void, GameObject_SetActive, xorstr_("UnityEngine"), xorstr_("GameObject"), xorstr_("SetActive"), (GameObject* instance, bool value))

// public void set_allowHDR(bool value)
RESOLVE_BY_NAME(void, Camera_set_allowHDR, xorstr_("UnityEngine"), xorstr_("Camera"), xorstr_("set_allowHDR"), (Camera* instance, bool value))
// public void set_fieldOfView(float value)
RESOLVE_BY_NAME(void, Camera_set_fieldOfView, xorstr_("UnityEngine"), xorstr_("Camera"), xorstr_("set_fieldOfView"), (Camera* instance, float value))
// public Matrix4x4 get_projectionMatrix()
RESOLVE_BY_NAME(void, Camera_get_projectionMatrix, xorstr_("UnityEngine"), xorstr_("Camera"), xorstr_("get_projectionMatrix"), (Matrix4x4* matrix, Camera* instance))
// public void set_backgroundColor(Color value)
RESOLVE_BY_NAME(void, Camera_set_backgroundColor, xorstr_("UnityEngine"), xorstr_("Camera"), xorstr_("set_backgroundColor"), (Camera* instance, Color value))
// [FreeFunction(xorstr_("FindMainCamera"))]
RESOLVE_BY_NAME(Camera*, Camera_get_main, xorstr_("UnityEngine"), xorstr_("Camera"), xorstr_("get_main"), ())
// [FreeFunction(xorstr_("GetCurrentCameraPPtr"))]
RESOLVE_BY_NAME(Camera*, Camera_get_current, xorstr_("UnityEngine"), xorstr_("Camera"), xorstr_("get_current"), ())
// public void set_clearFlags(CameraClearFlags value) { }
RESOLVE_BY_NAME(void, Camera_set_clearFlags, xorstr_("UnityEngine"), xorstr_("Camera"), xorstr_("set_clearFlags"), (Camera* instance, int value))

// [FreeFunction(Name = xorstr_("RendererScripting::GetMaterial"), HasExplicitThis = True)]
RESOLVE_BY_NAME(Material*, Renderer_GetMaterial, xorstr_("UnityEngine"), xorstr_("Renderer"), xorstr_("GetMaterial"), (Renderer* instance))
// [FreeFunction(Name = xorstr_("RendererScripting::SetMaterial"), HasExplicitThis = True)]
RESOLVE_BY_NAME(void, Renderer_SetMaterial, xorstr_("UnityEngine"), xorstr_("Renderer"), xorstr_("SetMaterial"), (Renderer* instance, Material* m))
// [FreeFunction(Name = xorstr_("RendererScripting::GetMaterialArray"), HasExplicitThis = True)]
RESOLVE_BY_NAME(Il2CppArray<Material*>*, Renderer_GetMaterialArray, xorstr_("UnityEngine"), xorstr_("Renderer"), xorstr_("GetMaterialArray"), (Renderer* instance))
// public void set_materials(Material[] value) { }
RESOLVE_BY_NAME(void, Renderer_set_materials, xorstr_("UnityEngine"), xorstr_("Renderer"), xorstr_("set_materials"), (Renderer* instance, Il2CppArray<Material*>* array))
// [FreeFunction(Name = xorstr_("RendererScripting::GetSharedMaterial"), HasExplicitThis = True)]
RESOLVE_BY_NAME(Material*, Renderer_GetSharedMaterial, xorstr_("UnityEngine"), xorstr_("Renderer"), xorstr_("GetSharedMaterial"), (Renderer* instance))
// public void set_sharedMaterial(Material value) { }
RESOLVE_BY_NAME(void, Renderer_set_sharedMaterial, xorstr_("UnityEngine"), xorstr_("Renderer"), xorstr_("set_sharedMaterial"), (Renderer* instance, Material* value))

// public static float get_deltaTime() { }
RESOLVE_BY_NAME(float, Time_get_deltaTime, xorstr_("UnityEngine"), xorstr_("Time"), xorstr_("get_deltaTime"), ())
// public static float get_time() { }
RESOLVE_BY_NAME(float, Time_get_time, xorstr_("UnityEngine"), xorstr_("Time"), xorstr_("get_time"), ())

// [FreeFunction(xorstr_("GetShaderNameRegistry().FindShader"))]
RESOLVE_BY_NAME(Shader*, ResourcesAPIInternal_FindShaderByName, xorstr_("UnityEngine"), xorstr_("ResourcesAPIInternal"), xorstr_("FindShaderByName"), (Il2CppString* name))

RESOLVE_BY_SIGNATURE(void, RankedConfirmationDialog_Confirm, (void* instance))

// |-HashSet<object>.Clear
// RESOLVE_BY_SIGNATURE(void, HashSet_object_Clear, (HashSet<Il2CppObject>* instance))
// RESOLVE_BY_NAME(void, HashSet_object_Clear, xorstr_("System.Collections.Generic"), xorstr_("HashSet`1"), xorstr_("Clear"), (HashSet<Il2CppObject>* instance))

RESOLVE_BY_SIGNATURE(void, ObjectOccludee_SetVisible, (void* instance, bool state))

// public static [a-zA-Z]{15} [a-zA-Z]{15}\(Component [a-zA-Z]{15}\) \{ \}
RESOLVE_BY_SIGNATURE(int, SurfaceTypeUtility_FromComponent, (Component* component))

// 30 48 2D E9 ? ? ? E5 00 40 A0 E1 05 50 8F E0 ? ? ? E5 ? ? ? E3 ? ? ? 1A ? ? ? E5 ? ? ? E7 ? ? ? EB ? ? ? E3 ? ? ? E5 04 00 A0 E1 ? ? ? E3 ? ? ? EB ? ? ? E5 ? ? ? E3 ? ? ? 1A ? ? ? EB 05 00 A0 E1 ? ? ? E3 ? ? ? E3 ? ? ? EB ? ? ? E5 ? ? ? E7 ? ? ? E5 ? ? ? E5
RESOLVE_BY_SIGNATURE(void, FlashbangEffectController_ResetEffect, (void* instance))

// [FreeFunction(xorstr_("AnimationCurveBindings::ClearKeys"), HasExplicitThis = True, IsThreadSafe = True)]
RESOLVE_BY_NAME(void, AnimationCurve_ClearKeys, xorstr_("UnityEngine"), xorstr_("AnimationCurve"), xorstr_("ClearKeys"), (AnimationCurve* instance))

// public bool HasProperty(string name) { }
RESOLVE_BY_NAME_AND_PARAMS(bool, Material_HasProperty, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("HasProperty"), (Material* instance, Il2CppString* name), 1, xorstr_("string"))
// public void .ctor(Shader shader) { }
RESOLVE_BY_NAME(void, Material_ctor, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_(".ctor"), (Material* instance, Shader* shader))
// public Shader get_shader() { }
RESOLVE_BY_NAME(Shader*, Material_get_shader, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("get_shader"), (Material* instance))
// public void set_shader(Shader value) { }
RESOLVE_BY_NAME(void, Material_set_shader, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("set_shader"), (Material* instance, Shader* shader))
// public void SetInt(string name, int value) { }
RESOLVE_BY_NAME(void, Material_SetInt, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("SetInt"), (Material* instance, Il2CppString* name, int value))
// public void SetFloat(string name, float value) { }
RESOLVE_BY_NAME(void, Material_SetFloat, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("SetFloat"), (Material* instance, Il2CppString* name, float value))
// public void SetColor(string name, Color value) { }
RESOLVE_BY_NAME(void, Material_SetColor, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("SetColor"), (Material* instance, Il2CppString* name, Color value))
// public Color GetColor(string name) { }
RESOLVE_BY_NAME(Color*, Material_GetColor, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("GetColor"), (Color* ret, Material* instance, Il2CppString* name))
// public Texture GetTexture(string name) { }
RESOLVE_BY_NAME(Texture*, Material_GetTexture, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("GetTexture"), (Material* instance, Il2CppString* name))
// public void SetTexture(string name, Texture value) { }
RESOLVE_BY_NAME(void, Material_SetTexture, xorstr_("UnityEngine"), xorstr_("Material"), xorstr_("SetTexture"), (Material* instance, Il2CppString* name, Texture* value))

// [FreeFunction(xorstr_("UnityEngineObjectBindings::FindObjectsByType"))]
RESOLVE_BY_NAME_AND_PARAMS(Il2CppArray<Object*>*, Object_FindObjectsByType, xorstr_("UnityEngine"), xorstr_("Object"), xorstr_("FindObjectsByType"), (Type* type, int findObjectsInactive, int sortMode), 3, xorstr_("System.Type"), xorstr_("UnityEngine.FindObjectsInactive"), xorstr_("UnityEngine.FindObjectsSortMode"))

// [FreeFunction(xorstr_("UnityEngineObjectBindings::FindObjectsOfType"))]
// RESOLVE_BY_NAME(Il2CppArray<Object*>*, Object_FindObjectsOfType, xorstr_("UnityEngine"), xorstr_("Object"), xorstr_("FindObjectsOfType"), (Type* type, bool includeInactive), 2, { xorstr_("Type"), xorstr_("bool") })
// public static void Destroy(Object obj) { }
RESOLVE_BY_NAME(void, Object_Destroy, xorstr_("UnityEngine"), xorstr_("Object"), xorstr_("Destroy"), (Object* instance))


// public static AssetBundleCreateRequest LoadFromFileAsync(string path) { }
RESOLVE_BY_NAME(AssetBundleCreateRequest*, AssetBundle_LoadFromFileAsync, xorstr_("UnityEngine"), xorstr_("AssetBundle"), xorstr_("LoadFromFileAsync"), (Il2CppString* path))
// [NativeMethod("GetAssetBundleBlocking")]
RESOLVE_BY_NAME(AssetBundle*, AssetBundleCreateRequest_get_assetBundle, xorstr_("UnityEngine"), xorstr_("AssetBundleCreateRequest"), xorstr_("get_assetBundle"), (AssetBundleCreateRequest* instance))
// public AssetBundleRequest LoadAssetAsync(string name, Type type) { }
RESOLVE_BY_NAME(AssetBundleRequest*, AssetBundle_LoadAssetAsync, xorstr_("UnityEngine"), xorstr_("AssetBundle"), xorstr_("LoadAssetAsync"), (AssetBundle* instance, Il2CppString* name, Type* type))
// [NativeMethod(xorstr_("GetLoadedAsset"))]
RESOLVE_BY_NAME(Il2CppObject*, AssetBundleRequest_GetResult, xorstr_("UnityEngine"), xorstr_("AssetBundleRequest"), xorstr_("GetResult"), (AssetBundleRequest* instance))
// [NativeMethod(xorstr_("GetAllLoadedAssets"))]
RESOLVE_BY_NAME(Il2CppArray<Il2CppObject*>*, AssetBundleRequest_get_allAssets, xorstr_("UnityEngine"), xorstr_("AssetBundleRequest"), xorstr_("get_allAssets"), (AssetBundleRequest* instance))
// public AssetBundleRequest LoadAllAssetsAsync(Type type) { }
RESOLVE_BY_NAME_AND_PARAMS(AssetBundleRequest*, AssetBundle_LoadAllAssetsAsync, xorstr_("UnityEngine"), xorstr_("AssetBundle"), xorstr_("LoadAllAssetsAsync"), (AssetBundle* instance, Type* type), 1, xorstr_("System.Type"))
// [NativeMethod(xorstr_("Unload"))]
RESOLVE_BY_NAME(void, AssetBundle_Unload, xorstr_("UnityEngine"), xorstr_("AssetBundle"), xorstr_("Unload"), (AssetBundle* instance, bool unloadAllLoadedObjects))

// xorstr_("public static bool ") in xorstr_("class GameManager ")
// RESOLVE_BY_SIGNATURE(bool, GameManager_get_InGame, ())

// [NativeMethod(xorstr_("IsDone"))]
RESOLVE_BY_NAME(bool, AsyncOperation_get_isDone, xorstr_("UnityEngine"), xorstr_("AsyncOperation"), xorstr_("get_isDone"), (AsyncOperation* instance))

// public [A-Z]{15} [a-zA-Z]{15}\(\) not compiler generated
// RESOLVE_BY_SIGNATURE(uintptr_t, PlayerController_get_team, (PlayerController* instance))
// public PhotonPlayer [a-zA-Z]{15}\(\)
// RESOLVE_BY_SIGNATURE(PhotonPlayer*, PlayerController_get_PhotonPlayer, (PlayerController* instance))

// for x64 xref: ? ? ? A9 ? ? ? B9 ? ? ? 71 ? ? ? 54 ? ? ? A8 C0 03 5F D6 ? ? ? F9 F3 03 00 AA ? ? ? 94
// for x32 xref: ? ? ? EB ? ? ? EA ? ? ? EB ? ? ? E3 07 10 A0 E1 ? ? ? E3 ? ? ? EB ? ? ? E5 ? ? ? E3 
RESOLVE_BY_SIGNATURE(void, PlayerController_SetTPSView, (PlayerController* instance))
// for x32 xref: ? ? ? EB ? ? ? EA ? ? ? EB ? ? ? E3 07 10 A0 E1 ? ? ? E3 ? ? ? EB ? ? ? E5 ? ? ? EB ? ? ? E3
// for x64 xref: ? ? ? A9 ? ? ? B9 ? ? ? 71 ? ? ? 54 ? ? ? A8 C0 03 5F D6 ? ? ? F9 F3 03 00 AA ? ? ? 97
RESOLVE_BY_SIGNATURE(void, PlayerController_SetFPSView, (PlayerController* instance))

RESOLVE_BY_SIGNATURE(void*, Fog_GetProperties, ())
RESOLVE_BY_SIGNATURE(void, Fog_Update, (void* properties))

// RESOLVE_BY_NAME(void, RenderSettings_set_fogColor, xorstr_("UnityEngine"), xorstr_("RenderSettings"), xorstr_("set_fogColor"), (Color value))
// RESOLVE_BY_NAME(void, RenderSettings_set_fogEndDistance, xorstr_("UnityEngine"), xorstr_("RenderSettings"), xorstr_("set_fogEndDistance"), (float value))
// RESOLVE_BY_NAME(void, RenderSettings_set_fog, xorstr_("UnityEngine"), xorstr_("RenderSettings"), xorstr_("set_fog"), (bool value))

RESOLVE_BY_SIGNATURE(Il2CppString*, PhotonRoomExtension_GetGameVersion, (Room* room))
// RESOLVE_BY_NAME(Il2CppString*, PhotonNetwork_get_gameVersion, "", xorstr_("PhotonNetwork"), xorstr_("get_gameVersion"), ())

RESOLVE_BY_NAME(Il2CppString*, PhotonPlayer_get_NickName, "", xorstr_("PhotonPlayer"), xorstr_("get_NickName"), (PhotonPlayer* instance))
RESOLVE_BY_NAME(void, PhotonPlayer_set_NickName, "", xorstr_("PhotonPlayer"), xorstr_("set_NickName"), (PhotonPlayer* instance, Il2CppString* value))

// public bool get_isLocal() { }
RESOLVE_BY_NAME(bool, PhotonPlayer_get_isLocal, "", xorstr_("PhotonPlayer"), xorstr_("get_isLocal"), (PhotonPlayer* instance))

RESOLVE_BY_NAME(Il2CppObject*, Photon_Hashtable_get_Item, xorstr_("ExitGames.Client.Photon"), xorstr_("Hashtable"), xorstr_("get_Item"), (Hashtable* instance, Il2CppObject* key))
RESOLVE_BY_NAME(void, Photon_Hashtable_set_Item, xorstr_("ExitGames.Client.Photon"), xorstr_("Hashtable"), xorstr_("set_Item"), (Hashtable* instance, Il2CppObject* key, Il2CppObject* value))

// public static bool Linecast(Vector3 start, Vector3 end, out RaycastHit hitInfo, int layerMask) { }
RESOLVE_BY_NAME_AND_PARAMS(bool, Physics_Linecast, xorstr_("UnityEngine"), xorstr_("Physics"), xorstr_("Linecast"), (Vector3 start, Vector3 end, RaycastHit* hitInfo, int layerMask), 4, xorstr_("UnityEngine.Vector3"), xorstr_("UnityEngine.Vector3"), xorstr_("UnityEngine.RaycastHit"), xorstr_("int"))

// public Collider get_collider() { }
RESOLVE_BY_NAME(Collider*, RaycastHit_get_collider, xorstr_("UnityEngine"), xorstr_("RaycastHit"), xorstr_("get_collider"), (RaycastHit* instance))

// [FreeFunction(xorstr_("GetGameObject"), HasExplicitThis = True)]
RESOLVE_BY_NAME(GameObject*, Component_get_gameObject, xorstr_("UnityEngine"), xorstr_("Component"), xorstr_("get_gameObject"), (Component* instance))
// [FreeFunction(xorstr_("GetTransform"), HasExplicitThis = True, ThrowsException = True)]
RESOLVE_BY_NAME(Transform*, Component_get_transform, xorstr_("UnityEngine"), xorstr_("Component"), xorstr_("get_transform"), (Component* instance))
// public string get_tag() { }
RESOLVE_BY_NAME(Il2CppString*, Component_get_tag, xorstr_("UnityEngine"), xorstr_("Component"), xorstr_("get_tag"), (Component* instance))

// public static Type GetType(string typeName) { }
RESOLVE_BY_NAME_AND_PARAMS(Type*, Type_GetType, xorstr_("System"), xorstr_("Type"), xorstr_("GetType"), (Il2CppString* typeName), 1, xorstr_("string"))

// public static IntPtr StringToHGlobalAnsi(string s) { }
RESOLVE_BY_NAME_AND_PARAMS(const char*, Marshal_StringToHGlobalAnsi, xorstr_("System.Runtime.InteropServices"), xorstr_("Marshal"), xorstr_("StringToHGlobalAnsi"), (Il2CppString* string), 1, xorstr_("string"))

// [NativeMethod(Name = xorstr_("GetHeight"), IsThreadSafe = True)]
RESOLVE_BY_NAME(int, Screen_get_height, xorstr_("UnityEngine"), xorstr_("Screen"), xorstr_("get_height"), ())
// [NativeMethod(Name = xorstr_("GetWidth"), IsThreadSafe = True)]
RESOLVE_BY_NAME(int, Screen_get_width, xorstr_("UnityEngine"), xorstr_("Screen"), xorstr_("get_width"), ())
