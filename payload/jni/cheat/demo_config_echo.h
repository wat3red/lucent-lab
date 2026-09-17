#pragma once

#include "function.h"
#include "shared/protocol/function_ids.h"

#include <cstring>

#pragma pack(push, 1)
struct LabConfigEchoSettings {
    float value;
    int32_t mode;
    char label[32];
};

struct LabConfigEchoData {
    float value;
    int32_t mode;
    uint32_t applyCount;
    char label[32];
};
#pragma pack(pop)

class LabConfigEcho : public Function {
public:
    static LabConfigEchoSettings GetSettings() { return settings; }

    uint32_t GetId() const override { return kFnConfigEcho; }
    size_t GetSettingsSize() const override { return sizeof(LabConfigEchoSettings); }

    void ApplySettings(const void* data) override {
        if (!data) return;
        std::memcpy(&settings, data, sizeof(settings));
        ++applyCount;
    }

    void GetData(std::vector<uint8_t>& out) override {
        LabConfigEchoData data{};
        data.value = settings.value;
        data.mode = settings.mode;
        data.applyCount = applyCount;
        std::memcpy(data.label, settings.label, sizeof(data.label));

        out.resize(sizeof(data));
        std::memcpy(out.data(), &data, sizeof(data));
    }

    static LabConfigEchoSettings settings;

private:
    uint32_t applyCount = 0;
};

inline LabConfigEchoSettings LabConfigEcho::settings{0.5f, 0, "echo"};
