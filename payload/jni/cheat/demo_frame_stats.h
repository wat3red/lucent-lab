#pragma once

#include "function.h"
#include "shared/protocol/function_ids.h"

#include <chrono>
#include <cstring>

#pragma pack(push, 1)
struct LabFrameStatsSettings {
    uint32_t enabled;
};

struct LabFrameStatsData {
    float dtMs;
    float fps;
    uint32_t frames;
};
#pragma pack(pop)

class LabFrameStats : public Function {
public:
    static LabFrameStatsSettings GetSettings() { return settings; }

    uint32_t GetId() const override { return kFnFrameStats; }
    size_t GetSettingsSize() const override { return sizeof(LabFrameStatsSettings); }

    void Init() override {
        last = std::chrono::steady_clock::now();
        frames = 0;
        dtMs = 0.f;
    }

    void Update() override {
        if (!settings.enabled) return;

        const auto now = std::chrono::steady_clock::now();
        dtMs = std::chrono::duration<float, std::milli>(now - last).count();
        last = now;
        ++frames;
    }

    void GetData(std::vector<uint8_t>& out) override {
        if (!settings.enabled) return;

        LabFrameStatsData data{};
        data.dtMs = dtMs;
        data.fps = dtMs > 0.f ? 1000.f / dtMs : 0.f;
        data.frames = frames;

        out.resize(sizeof(data));
        std::memcpy(out.data(), &data, sizeof(data));
    }

    void ApplySettings(const void* data) override {
        if (!data) return;
        std::memcpy(&settings, data, sizeof(settings));
    }

    static LabFrameStatsSettings settings;

private:
    std::chrono::steady_clock::time_point last{};
    uint32_t frames = 0;
    float dtMs = 0.f;
};

inline LabFrameStatsSettings LabFrameStats::settings{1};
