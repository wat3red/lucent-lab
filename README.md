# standoff2_internal

Unity **IL2CPP research lab**: an Android in-process payload and a Windows ImGui control client, connected by a compact binary protocol.

> Lab/portfolio build. Competitive game modules, bypass, and ban-evasion scripts are **not registered** and should not be used against online games.

## Layout

| Path | Role |
|---|---|
| [`payload/`](payload/) | Android NDK shared library (`libdaemon.so`) — hooks, SDK, protocol server |
| [`client/`](client/) | Windows OpenGL/ImGui client (`LucentClient`) |
| [`shared/protocol/`](shared/protocol/) | One definition of `FunctionPacket` + lab function IDs |
| [`docs/`](docs/) | Architecture, protocol, build, portfolio map |
| [`licenses/`](licenses/) | Third-party license texts |

## Quick architecture

```text
Android game process          TCP :64108           Windows host
payload/libdaemon.so   <------------------->   client/LucentClient.dll
IL2CPP + Function modules                      ImGui Function UI
```

Details: [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) · [docs/PROTOCOL.md](docs/PROTOCOL.md)

## Lab modules (default)

| ID | Module | Shows |
|---|---|---|
| 9001 | LabPing | settings + telemetry tick/uptime |
| 9002 | LabConfigEcho | packed struct round-trip |
| 9003 | LabFrameStats | dt/fps from the payload loop |

## Build

- **Payload:** `ndk-build NDK_PROJECT_PATH=payload` — produced `payload/libs/armeabi-v7a/libdaemon.so` on this machine
- **Client:** `client/LucentClient.sln` — Release|x64 produced `client/x64/Release/LucentClient.dll` (OpenSSL + curl includes via local vcpkg paths; retarget if your SDK layout differs)

Offline auth is the default (`LoginUser` returns success without a backend). Remote auth requires `ENABLE_REMOTE_AUTH` and a local `auth_secrets.h`.

See [docs/BUILD.md](docs/BUILD.md).

## Status

- [x] Monorepo (`payload` + `client`)
- [x] Shared protocol headers
- [x] Lab demo modules registered
- [x] Secrets/binaries/unban removed from the working tree
- [x] Third-party license files present
- [x] Payload NDK build verified
- [x] Client MSVC build verified
- [ ] Fresh public remote (export prepared separately — do not push the private history)

## Reviewers

Start at [docs/PORTFOLIO.md](docs/PORTFOLIO.md).

## License

MIT — see [LICENSE](LICENSE). Vendored components keep their own licenses ([NOTICE.md](NOTICE.md)).
