# Portfolio map

If you are reviewing this repo for a role, start here.

## Read in this order

1. [ARCHITECTURE.md](ARCHITECTURE.md) — why two processes and how they talk  
2. `shared/protocol/function_packet.h` — packed wire format + static asserts  
3. `payload/jni/cheat/demo_ping.h` — payload-side module (settings in, data out)  
4. `client/LucentClient/demo_ping.h` — client-side twin (ImGui + same ID)  
5. `payload/jni/main.cpp` `MainThread` — load, register, loop  
6. `client/LucentClient/main.cpp` `InitFunctions` / TCP thread  
7. `payload/jni/il2cpp/` + `payload/jni/sdk/` — IL2CPP resolve surface  

## What this lab demonstrates

- Cross-language, cross-OS product split (Android NDK + Windows MSVC)
- Binary protocol design (fixed header, batching, stable IDs)
- Plugin-style modules (`Function` vtable) on both sides
- ImGui operator UI with theming
- Monorepo hygiene: shared headers, relative scripts, gitignored secrets

## Intentionally not demonstrated in the default build

- Competitive game features, anti-cheat bypass, ban evasion, live signatures
- Hardcoded auth backend credentials (removed; `auth_secrets.h` is local-only)

Private product history exists in older commits; a public publish should use a **fresh history**.
