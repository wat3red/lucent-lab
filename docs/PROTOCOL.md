# Protocol

Single source of truth: `shared/protocol/function_packet.h` and `shared/protocol/function_ids.h`.

## Packet header (12 bytes, little-endian assumed)

```text
offset  size  field
0       4     functionId   uint32
4       4     type         PacketType (0=SETTINGS, 1=DATA)
8       4     dataSize     uint32
12      N     payload      dataSize bytes
```

Packets are concatenated in one TCP write (`SendAllData` / settings send).
Receiver loops `while (size >= sizeof(FunctionPacket))` and dispatches on `functionId`.

## Directions

| type | sender | receiver | meaning |
|---|---|---|---|
| `PACKET_SETTINGS` | client | payload | module configuration |
| `PACKET_DATA` | payload | client | telemetry / results for UI |

## Lab function IDs

| ID | Module | Settings payload | Data payload |
|---|---|---|---|
| 9001 | `LabPing` | `LabPingSettings` | `LabPingData` |
| 9002 | `LabConfigEcho` | `LabConfigEchoSettings` | `LabConfigEchoData` |
| 9003 | `LabFrameStats` | `LabFrameStatsSettings` | `LabFrameStatsData` |

IDs 1–10 were used by private product features; they are **not registered** in this lab build.

## Compatibility rules

1. Never change field order/size of an existing struct without bumping a `version` field or ID.
2. Add `static_assert` on packed size when you add a field.
3. Prefer `uint32_t` flags over `bool` inside packed settings (ABI-safe).
4. Keep this document in sync with `shared/protocol/`.
