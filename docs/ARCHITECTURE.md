# Architecture

```text
┌─────────────────────────────┐         TCP 127.0.0.1:64108        ┌──────────────────────────────┐
│  Android process (game)     │  ◄──────────────────────────────►  │  Windows host                │
│  payload/libdaemon.so       │         FunctionPacket stream      │  client/LucentClient.dll     │
│                             │                                    │                              │
│  JNI constructor → Main     │                                    │  OpenGL hook → ImGui menu    │
│  IL2CPP resolve + hooks     │                                    │  Function UI cards           │
│  FunctionsManager           │                                    │  FunctionsManager            │
│    LabPing / Echo / Stats   │                                    │    LabPing / Echo / Stats    │
│  NetworkManager (server)    │                                    │  NetworkManager (client)     │
└─────────────────────────────┘                                    └──────────────────────────────┘
```

## Split rationale

| Half | Why it exists |
|---|---|
| `payload/` | Must run **in-process** to touch IL2CPP objects and hooks on Android |
| `client/` | Operator UI, fonts, OpenGL overlay — awkward/unsafe on-device |
| `shared/protocol` | One definition of the wire format so the two halves cannot silently drift |

## Runtime loop (payload)

1. `constructor_main` starts `MainThread`.
2. Locate `libunity.so`, resolve IL2CPP exports / signatures.
3. Register lab `Function` modules and `Init()` them.
4. Optionally start TCP server (`NetworkManager`, port 64108).
5. Each tick: `Function::Update()` then `FunctionsManager::SendAllData()`.

## Runtime loop (client)

1. Hook `wglSwapBuffers`, init ImGui + fonts.
2. Optional auth UI (offline lab stub after secret scrub).
3. `InitFunctions()` registers UI + protocol modules.
4. TCP thread connects to payload; settings out, data in.
5. ImGui renders each `Function::RenderUI()`; changed settings are packed and sent.

## Module model

A `Function` is a named, ID-stable unit:

- **Settings** — client → payload (`PACKET_SETTINGS`)
- **Data** — payload → client (`PACKET_DATA`)

See [PROTOCOL.md](PROTOCOL.md).
