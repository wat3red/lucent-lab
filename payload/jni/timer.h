#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include "logger.h"

class Timer {
public:
    Timer(const std::string& name) : name(name), running(false) {
        Start();
    }

    ~Timer() {
        Stop();
    }

    void Start() {
        start = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void Stop() {
        if (!running) return;
        auto end = std::chrono::high_resolution_clock::now();
        float durationMs = std::chrono::duration<float, std::milli>(end - start).count();
        LOGD("%s took %.4f ms", name.c_str(), durationMs);
        running = false;
    }

    void Reset() {
        running = false;
    }

private:
    std::string name;
    std::chrono::high_resolution_clock::time_point start;
    bool running;
};
