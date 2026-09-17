#pragma once

#pragma pack(push, 1)
struct WorldControllerSettings {
    bool enableFog;
    float fogEndDistance;
    float fogStartDistance;
    Color fogColor;

    bool enableWorldColor;
    Color worldColor;

    bool enableSkyColor;
    Color skyColor;
};
#pragma pack(pop)

class WorldController : public Function {
public:
    uint32_t GetId() const override { return 7; }
    size_t GetSettingsSize() const override { return sizeof(WorldControllerSettings); }

    static inline bool reloadWorldColor;

    // Store original world colors here
    inline static std::unordered_map<Material*, Color> originalColors;

    static void ProcessWorldColor()
    {
        auto colorStr = Il2CppString::CreateString(oxorany("_Color"));
        auto texStr = Il2CppString::CreateString(oxorany("_MainTex"));

        Shader* shader = Shader::FindShaderByName(Il2CppString::CreateString(oxorany("Legacy Shaders/Diffuse")));
        if (!shader) return;

        static Type* meshType = Type::GetType(Il2CppString::CreateString(oxorany("UnityEngine.MeshRenderer, UnityEngine.CoreModule")));
        Il2CppArray<Renderer*>* worldRenderers = (Il2CppArray<Renderer*>*)Object_FindObjectsByType(meshType, false, false);
        if (!worldRenderers || !worldRenderers->size) return;

        if (settings.enableWorldColor && reloadWorldColor) {
            LOGD("Applying custom WorldColor...");

            Color newColor = { settings.worldColor.r, settings.worldColor.g, settings.worldColor.b, 1 };

            for (int i = 0; i < worldRenderers->size; ++i) {
                Renderer* renderer = worldRenderers->items[i];
                if (!renderer) continue;

                Il2CppArray<Material*>* materials = renderer->GetMaterials();
                if (!materials) continue;

                for (int j = 0; j < materials->size; ++j) {
                    Material* mat = materials->items[j];
                    if (!mat) continue;

                    if (mat->HasProperty(colorStr)) {
                        // Store original color if not stored
                        if (originalColors.find(mat) == originalColors.end()) {
                            Color orig = mat->GetColor(colorStr);
                            originalColors[mat] = orig;
                        }

                        mat->SetColor(colorStr, newColor);
                    } else {
                        // Replace unsupported material with diffuse shader
                        Material* newMat = Material::Create(shader);
                        Texture* oldTex = mat->GetTexture(texStr);
                        newMat->SetTexture(texStr, oldTex);
                        materials->items[j] = newMat;
                        newMat->SetColor(colorStr, newColor);
                    }
                }
                renderer->SetMaterials(materials);
            }

            reloadWorldColor = false;
        }
        else if (!settings.enableWorldColor && reloadWorldColor) {
            LOGD("Restoring original WorldColor...");

            for (auto& kv : originalColors) {
                Material* mat = kv.first;
                if (!mat) continue;

                if (mat->HasProperty(colorStr)) {
                    mat->SetColor(colorStr, kv.second);
                }
            }

            originalColors.clear();
            reloadWorldColor = false;
        }
    }

    inline static int reloadWorldColorCountdown = 512;
    static void GameUpdateCallback(GameController* instance) {
        if (reloadWorldColorCountdown > 0) {
            --reloadWorldColorCountdown;
            if (reloadWorldColorCountdown == 0)
                reloadWorldColor = true;
        }

        ProcessWorldColor();

        if (settings.enableSkyColor) {
            Camera* mainCamera = Camera::GetMain();
            if (mainCamera) {
                mainCamera->SetBackgroundColor(settings.skyColor);
                mainCamera->SetClearFlags(2);
            }
        }
    }

    static void SceneLoadedCallback(Scene s, int m) {
        reloadWorldColorCountdown = oxorany(512);
        reloadWorldColor = true; // re-apply or restore colors on new scene
    }

    void Init() override {
        Global::Hooks::AddGameUpdateCallback(GameUpdateCallback);
        Global::Hooks::AddSceneLoadedCallback(SceneLoadedCallback);
    }

    void ApplySettings(const void* data) override {
        memcpy(&settings, data, sizeof(WorldControllerSettings));
        reloadWorldColor = true;
    }
    void GetCurrentSettings(void* out) override {
        memcpy(out, &settings, sizeof(WorldControllerSettings));
    }
private:
    inline static WorldControllerSettings settings;
};