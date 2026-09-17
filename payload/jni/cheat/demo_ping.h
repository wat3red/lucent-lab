#pragma once

#include "function.h"
#include "shared/protocol/function_ids.h"

#include <chrono>
#include <cstring>

#pragma pack(push, 1)
struct LabPingSettings {
    uint32_t intervalMs;
    uint32_t enabled;  // bool-sized on both ABIs; keep 4-byte
};

struct LabPingData {
    uint64_t tick;
    uint32_t uptimeMs;
    uint32_t version;
};
#pragma pack(pop)

class LabPing : public Function {
public:
    static LabPingSettings GetSettings() { return settings; }

    uint32_t GetId() const override { return kFnPing; }
    size_t GetSettingsSize() const override { return sizeof(LabPingSettings); }

    void Init() override {
        start = std::chrono::steady_clock::now();
        tick = 0;
    }

    void Update() override {
        if (!settings.enabled) return;

        const auto now = std::chrono::steady_clock::now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSend).count();
        if (settings.intervalMs > 0 && elapsed < settings.intervalMs) return;

        lastSend = now;
        ++tick;
    }

    void GetData(std::vector<uint8_t>& out) override {
        if (!settings.enabled) return;

        LabPingData data{};
        data.tick = tick;
        data.uptimeMs = static_cast<uint32_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - start).count());
        data.version = 1;

        out.resize(sizeof(data));
        std::memcpy(out.data(), &data, sizeof(data));
    }

    void ApplySettings(const void* data) override {
        if (!data) return;
        std::memcpy(&settings, data, sizeof(settings));
    }

    static LabPingSettings settings;

private:
    std::chrono::steady_clock::time_point start{};
    std::chrono::steady_clock::time_point lastSend{};
    uint64_t tick = 0;
};

inline LabPingSettings LabPing::settings{1000, 1};
