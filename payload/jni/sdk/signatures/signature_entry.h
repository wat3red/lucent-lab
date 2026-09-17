// signature_entry.h
#pragma once

struct SignatureEntry {
    const char* name;
    const char* pattern_x64;
    const char* pattern_x32;
    bool isXref;

    constexpr SignatureEntry(const char* n, const char* p64, const char* p32, bool _isXref = false)
        : name(n), pattern_x64(p64), pattern_x32(p32), isXref(_isXref) {}

    const char* GetPattern() const {
    #ifdef __aarch64__
        return pattern_x64;
    #else
        return pattern_x32;
    #endif
    }
};


