#pragma once

#pragma pack(push, 1)
struct ChamsModelSettings {
    bool enable;
    float visibleEmissionColor[4];
    float invisibleEmissionColor[4];
    float outlineColor[4];
    float outlineWidth;
    int outlineVisibilityMode; // OnBothParts - 0, OnVisiblePart - 2, OnInvisiblePart - 5
};

struct ChamsSettings {
    bool enable;

    ChamsModelSettings playerArms;
    ChamsModelSettings playerGloves;

    ChamsModelSettings enemy;

    ChamsModelSettings ally;
};
#pragma pack(pop)

struct Chams : public Function {
    static inline Shader* customShader = nullptr;

    // Map to store the original shader for each material
    static inline std::unordered_map<Material*, Shader*> originalShaders;

    uint32_t GetId() const override { return 6; }
    size_t GetSettingsSize() const override { return sizeof(ChamsSettings); }

    void Init() override;
    void ApplySettings(const void* data) override;
    void GetCurrentSettings(void* out) override;

    static const ChamsSettings& GetCurrentSettings() {
        return settings;
    }

    inline static ChamsSettings settings;

    enum class LoadStage {
        NotStarted,
        WaitingForBundle,
        WaitingForAssets,
        Done,
        Failed
    };

    static inline LoadStage loadStage = LoadStage::NotStarted;
    static inline AssetBundleCreateRequest* bundleCreateRequest = nullptr;
    static inline AssetBundleRequest* assetRequest = nullptr;
    static inline AssetBundle* bundle = nullptr;

    static inline void (*SceneManager_Internal_SceneLoaded_orig)(Scene scene, int mode);

    static void UpdateChams(GameController* _this);
    static void PlayerUpdateCallback(PlayerController* _this);

    static void StartShaderLoadProcess();
    static void HandleWaitingForBundle();
    static void HandleWaitingForAssets();
    static void FinalizeShaderLoading(Il2CppObject** assets, int count);

    static void ChamsSceneLoadedCallback(Scene scene, int mode);

    // Helper functions to apply and reset the chams shader on a material
    static void ApplyChamsMaterial(Shader* shader, Material* mat, const ChamsModelSettings& modelSettings);
    static void ResetChamsMaterial(Material* mat);
};

// Implementation

void Chams::Init() {
    Global::Hooks::AddSceneLoadedCallback(ChamsSceneLoadedCallback);
    bundleCreateRequest = AssetBundle::LoadFromFileAsync(Il2CppString::CreateString(oxorany("/data/local/tmp/bundle")));

    LOGE("bundleCreateRequest : %p", bundleCreateRequest);

    if (!bundleCreateRequest) {
        LOGE("Failed to start async shader load.");
        loadStage = LoadStage::Failed;
        return;
    }

    il2cpp_gchandle_new(bundleCreateRequest, true);

    StartShaderLoadProcess();

    Global::Hooks::AddGameUpdateCallback(UpdateChams);
    Global::Hooks::AddPlayerUpdateCallback(PlayerUpdateCallback);
}

void Chams::ApplySettings(const void* data) {
    memcpy(&settings, data, sizeof(ChamsSettings));
}

void Chams::GetCurrentSettings(void* out) {
    memcpy(out, &settings, sizeof(ChamsSettings));
}

void Chams::StartShaderLoadProcess() {
    if (loadStage == LoadStage::WaitingForBundle || loadStage == LoadStage::WaitingForAssets) return;
    loadStage = LoadStage::WaitingForBundle;
}

void Chams::UpdateChams(GameController*) {
    switch (loadStage) {
    case LoadStage::WaitingForBundle: HandleWaitingForBundle(); break;
    case LoadStage::WaitingForAssets: HandleWaitingForAssets(); break;
    default: break;
    }
}

void Chams::HandleWaitingForBundle() {
    if (!bundleCreateRequest || !bundleCreateRequest->IsDone()) return;

    bundle = bundleCreateRequest->GetAssetBundle();
    if (!bundle) {
        loadStage = LoadStage::Failed;
        return;
    }

    static Type* objectType = Type::GetType(Il2CppString::CreateString(oxorany("UnityEngine.Object, UnityEngine")));
    if (!objectType) {
        loadStage = LoadStage::Failed;
        return;
    }

    assetRequest = bundle->LoadAllAssetsAsync(objectType);
    if (!assetRequest) {
        loadStage = LoadStage::Failed;
        return;
    }
    il2cpp_gchandle_new(assetRequest, true);

    loadStage = LoadStage::WaitingForAssets;
}

void Chams::HandleWaitingForAssets() {
    if (!assetRequest || !assetRequest->IsDone()) return;

    auto* assetArray = assetRequest->GetAllLoadedAssets();
    FinalizeShaderLoading(assetArray->items, assetArray->size);
}

void Chams::FinalizeShaderLoading(Il2CppObject** assets, int count) {
    customShader = nullptr;

    for (int i = 0; i < count; ++i) {
        auto* asset = assets[i];
        if (!asset || !asset->klass) continue;

        if (strcmp(asset->klass->_1.name, oxorany("Shader")) == 0) {
            customShader = reinterpret_cast<Shader*>(asset);
            customShader->DontDestroyOnLoad();
            customShader->SetHideFlags((int)HideFlags::DontUnloadUnusedAsset);
            il2cpp_gchandle_new(customShader, true);
            break;
        }
    }

    loadStage = (customShader && customShader->m_CachedPtr) ? LoadStage::Done : LoadStage::Failed;
}

void Chams::ChamsSceneLoadedCallback(Scene scene, int mode) {
    if (loadStage == LoadStage::WaitingForBundle || loadStage == LoadStage::WaitingForAssets) return;

    if (loadStage == LoadStage::Failed) {
        customShader = nullptr;
        bundleCreateRequest = nullptr;
        assetRequest = nullptr;
        bundle = nullptr;
    }

    if (!bundleCreateRequest) {
        loadStage = LoadStage::Failed;
        return;
    }

    loadStage = LoadStage::WaitingForBundle;
}

void Chams::ApplyChamsMaterial(Shader* shader, Material* mat, const ChamsModelSettings& modelSettings) {
    try {
        // Cache the original shader if not already stored
        if (originalShaders.find(mat) == originalShaders.end()) {
            Shader* original = mat->GetShader();
            originalShaders[mat] = original;
        }

        mat->SetShader(shader);
        mat->SetColor(oxorany("_InvisibleEmissionColor"), Color(modelSettings.invisibleEmissionColor));
        mat->SetFloat(oxorany("_InvisibleEmissionStrength"), modelSettings.invisibleEmissionColor[3]);

        mat->SetColor(oxorany("_VisibleEmissionColor"), Color(modelSettings.visibleEmissionColor));
        mat->SetFloat(oxorany("_VisibleEmissionStrength"), modelSettings.visibleEmissionColor[3]);

        mat->SetColor(oxorany("_OutlineColor"), Color(modelSettings.outlineColor));
        mat->SetFloat(oxorany("_OutlineWidth"), modelSettings.outlineWidth);
        mat->SetInt(oxorany("_OutlineVisiblilityMode"), modelSettings.outlineVisibilityMode);
    }
    catch (...) {
        LOGE("Exception setting chams material.");
    }
}

void Chams::ResetChamsMaterial(Material* mat) {
    if (!mat) return;
    auto it = originalShaders.find(mat);
    if (it != originalShaders.end()) {
        try {
            mat->SetShader(it->second);
        }
        catch (...) {
            LOGE("Exception resetting material shader.");
        }
    }
}

void Chams::PlayerUpdateCallback(PlayerController* player) {
    // Timer t(oxorany("ChamsPlayerUpdateCallback"));

    if (loadStage != LoadStage::Done || !player || !customShader || !customShader->m_CachedPtr || !player->IsAlive()) return;

    PlayerManager* playerManager = SingletonManager::Instance().GetPlayerManager();
    if (!playerManager) return;
    PlayerController* localPlayer = playerManager->GetLocalPlayer();
    if (!localPlayer) return;

    Team localTeam = localPlayer->m_Team;
    Team playerTeam = player->m_Team;

    if (player == localPlayer) {
        // Local player arms and gloves
        ArmsLodGroup* armsGroup = player->m_ArmsLodGroup;
        if (armsGroup) {
            SkinnedMeshRenderer* arms = armsGroup->GetArmsMeshRenderer();
            SkinnedMeshRenderer* gloves = armsGroup->GetGlovesMeshRenderer();

            if (arms) {
                Material* mat = arms->GetMaterial();
                if (mat) {
                    if (settings.playerArms.enable && settings.enable) {
                        ApplyChamsMaterial(customShader, mat, settings.playerArms);
                    }
                    else {
                        ResetChamsMaterial(mat);
                    }
                }
            }
            if (gloves) {
                Material* mat = gloves->GetMaterial();
                if (mat) {
                    if (settings.playerGloves.enable && settings.enable) {
                        ApplyChamsMaterial(customShader, mat, settings.playerGloves);
                    }
                    else {
                        ResetChamsMaterial(mat);
                    }
                }
            }
        }
    }
    else {
        // Enemy or ally character mesh
        CharacterLodGroup* lodGroup = player->m_CharacterLodGroup;
        if (lodGroup) {
            SkinnedMeshRenderer* renderer = lodGroup->GetSkinnedMeshRenderer();
            if (renderer) {
                ChamsModelSettings* modelSettings = (playerTeam == localTeam) ? &settings.ally : &settings.enemy;
                Material* mat = renderer->GetMaterial();
                if (mat) {
                    if (modelSettings->enable && settings.enable) {
                        ApplyChamsMaterial(customShader, mat, *modelSettings);
                    }
                    else {
                        ResetChamsMaterial(mat);
                    }
                }
            }
        }
    }
}