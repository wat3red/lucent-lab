#pragma once

#include <cstdint>

// Shared wire format between Android payload and Windows client.
// Keep this header platform-agnostic (no sockets, no OS types).

#pragma pack(push, 1)

enum PacketType : uint32_t {
    PACKET_SETTINGS = 0,
    PACKET_DATA = 1,
};

struct FunctionPacket {
    uint32_t functionId;
    PacketType type;
    uint32_t dataSize;
};

#pragma pack(pop)

static_assert(sizeof(FunctionPacket) == 12, "FunctionPacket must stay packed to 12 bytes");
static_assert(sizeof(PacketType) == 4, "PacketType must be 4 bytes");
