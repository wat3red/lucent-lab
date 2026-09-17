#include "config_manager.h"

std::string ConfigManager::CurrentConfig = oxorany("default.json");

void ConfigManager::Save(const json& config) {
    char* userProfile = nullptr;
    size_t len = 0;
    errno_t err = _dupenv_s(&userProfile, &len, oxorany("USERPROFILE"));

    if (err == 0 && userProfile != nullptr) {
        std::string directory = std::string(userProfile) + oxorany("\\Lucent");
        free(userProfile);  // Clean up allocated memory

        std::filesystem::create_directories(directory);
        std::ofstream file(directory + oxorany("\\") + CurrentConfig);
        if (file.is_open()) {
            file << config.dump(4);
            file.close();
        }
    }
}

json ConfigManager::Load() {
    char* userProfile = nullptr;
    size_t len = 0;
    errno_t err = _dupenv_s(&userProfile, &len, oxorany("USERPROFILE"));

    if (err != 0 || userProfile == nullptr) {
        return json{};
    }

    std::string directory = std::string(userProfile) + oxorany("\\Lucent");
    free(userProfile);  // Clean up allocated memory

    std::ifstream file(directory + oxorany("\\") + CurrentConfig);
    if (!file.is_open()) return json{};

    json config;
    file >> config;
    return config;
}


std::vector<std::string> ConfigManager::SplitPath(const std::string& path) {
    std::vector<std::string> result;
    size_t start = 0, end;
    while ((end = path.find(oxorany("."), start)) != std::string::npos) {
        result.push_back(path.substr(start, end - start));
        start = end + 1;
    }
    result.push_back(path.substr(start));
    return result;
}

void ConfigManager::RemoveSection(const std::string& path) {
    json config = Load();
    json* section = &config;
    std::vector<std::string> parts = SplitPath(path);
    if (parts.empty())
        return;

    for (size_t i = 0; i < parts.size() - 1; ++i) {
        if (!section->contains(parts[i]))
            return;
        section = &((*section)[parts[i]]);
    }

    section->erase(parts.back());
    Save(config);
}