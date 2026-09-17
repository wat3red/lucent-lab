#pragma once

#include <cstdint>

// Stable function IDs used by the lab/demo modules.
// Product feature IDs (1–10) stay reserved for local private builds.
enum LabFunctionId : uint32_t {
    kFnPing = 9001,
    kFnConfigEcho = 9002,
    kFnFrameStats = 9003,
};
