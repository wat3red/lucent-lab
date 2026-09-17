// signature_resolver.h
#pragma once

#include <stdint.h>
#include <unordered_map>
#include <string>
#include <future>
#include <mutex>

#include "signature_database.h"
#include "logger.h"
#include "utils.h"

class SignatureResolver {
public:
    SignatureResolver(uintptr_t _base, size_t _libSize)
        : base(_base), libSize(_libSize) {
    }

    void Init() {
        std::vector<std::future<void>> futures;

        for (const auto& sig : kSignatures) {
            futures.push_back(std::async(std::launch::async, [this, sig]() {
                void* addr = FindPattern((uint8_t*)base, libSize, sig.GetPattern(), sig.isXref);
                if (addr) {
                    {
                        std::lock_guard<std::mutex> lock(mutex);
                        resolved[sig.name] = addr;
                    }
                    LOGD("[+] %s => %p", sig.name, addr);
                }
                else {
                    LOGE("[-] Signature not found: %s", sig.name);
                }
                }));
        }

        // Ждём завершения всех задач
        for (auto& future : futures) {
            future.get();
        }
    }


    void* Get(const char* name) const {
        auto it = resolved.find(name);
        return it != resolved.end() ? it->second : nullptr;
    }

private:
    uintptr_t base;
    size_t libSize;
    std::unordered_map<std::string, void*> resolved;
    std::mutex mutex;
};
