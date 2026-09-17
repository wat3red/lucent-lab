#include <oxorany/oxorany.h>
#include <xorstr.h>
#include "utils.h"

size_t GetModuleSize(const std::string& moduleName) {
    FILE* fp = fopen(oxorany("/proc/self/maps"), oxorany("r"));
    if (!fp) return 0;

    char line[512];
    uintptr_t start, end;

    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, moduleName.c_str())) {
            sscanf(line, oxorany("%p-%p"), (void**)&start, (void**)&end);
            fclose(fp);
            return end - start;
        }
    }

    fclose(fp);
    return 0;
}


uintptr_t GetModuleBase(const std::string& moduleName) {
    std::ifstream maps(oxorany("/proc/self/maps"));
    std::string line;

    while (std::getline(maps, line)) {
        if (line.find(moduleName) == std::string::npos) continue;

        size_t dashPos = line.find('-');
        if (dashPos == std::string::npos) continue;

        return std::stoull(line.substr(0, dashPos), nullptr, 16);
    }
    return 0;
}



#include <arm_neon.h> // Required for NEON intrinsics
// Helper to parse the signature string (assuming it's the same as your original)
// This function is not provided in the original code, so a plausible implementation is assumed.
void ParseSignature(const std::string& pattern, std::vector<uint8_t>& bytes, std::string& mask) {
    for (size_t i = 0; i < pattern.length(); ++i) {
        if (pattern[i] == '?') {
            bytes.push_back(0);
            mask += '?';
        }
        else if (isspace(pattern[i])) {
            continue;
        }
        else if (i + 1 < pattern.length() && isxdigit(pattern[i]) && isxdigit(pattern[i + 1])) {
            bytes.push_back(std::stoi(pattern.substr(i, 2), nullptr, 16));
            mask += 'x';
            i++; // Skip the next character
        }
    }
}


bool AllBytesMatch(const uint8x16_t& a, const uint8x16_t& b) {
    uint8x16_t cmp = vceqq_u8(a, b);
    uint8_t result[16];
    vst1q_u8(result, cmp);
    for (int i = 0; i < 16; ++i) {
        if (result[i] != 0xFF)
            return false;
    }
    return true;
}

void* FindPattern(const uint8_t* base, size_t size, const std::string& pattern, bool isXref) {
    if (pattern.empty() || !base || size == 0) return nullptr;

    std::vector<uint8_t> patternBytes;
    std::string maskStr;
    ParseSignature(pattern, patternBytes, maskStr);

    size_t patternLen = patternBytes.size();
    if (size < patternLen) return nullptr;

    // First 16 bytes (or less if pattern is shorter)
    size_t neonLen = std::min((size_t)16, patternLen);

    uint8_t patternChunk[16] = { 0 };
    uint8_t maskChunk[16] = { 0 };
    for (size_t i = 0; i < neonLen; ++i) {
        patternChunk[i] = patternBytes[i];
        maskChunk[i] = (maskStr[i] == 'x') ? 0xFF : 0x00;
    }

    uint8x16_t first16_pattern = vld1q_u8(patternChunk);
    uint8x16_t first16_mask = vld1q_u8(maskChunk);

    size_t end = size - patternLen;
    for (size_t i = 0; i <= end; ++i) {
        uint8x16_t memory_chunk = vld1q_u8(base + i);
        uint8x16_t comparison = vceqq_u8(memory_chunk, first16_pattern);
        uint8x16_t masked_comparison = vandq_u8(comparison, first16_mask);

        if (AllBytesMatch(masked_comparison, first16_mask)) {
            // First 16 matched, check rest
            bool found = true;
            for (size_t j = neonLen; j < patternLen; ++j) {
                if (maskStr[j] == 'x' && base[i + j] != patternBytes[j]) {
                    found = false;
                    break;
                }
            }

            if (found) {
                if (isXref) {
                    uint32_t instr = *(uint32_t*)(base + i);
                    if (((instr >> 24) & 0xFE) == 0xEA) {
                        int32_t offset = ((int32_t)(instr & 0x00FFFFFF) << 8) >> 6;
                        uintptr_t pc = (uintptr_t)(base + i) + 8;
                        return (void*)(pc + offset);
                    }
                }
                return (void*)(base + i);
            }
        }
    }

    return nullptr;
}