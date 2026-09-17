# Build

## Payload (Android NDK)

Requirements:

- Android NDK (`ndk-build` on PATH)
- JDK not required
- Emulator or device + `adb` for deploy

```bat
cd payload
ndk-build NDK_PROJECT_PATH=%CD% NDK_DEBUG=1
```

Output: `payload/libs/<abi>/libdaemon.so`

Include path for `shared/protocol` is set in `payload/jni/Android.mk` (`LOCAL_C_INCLUDES` → monorepo root).

Deploy helper: `payload/scripts/deploy-emulator.bat` (see `payload/scripts/README.md`).
The injector binary is **not** committed.

## Client (MSVC)

Requirements:

- Visual Studio with C++ desktop workload (toolset in `.vcxproj` is `v145` — adjust if needed)
- OpenSSL at `D:\OpenSSL-Win64` **or** update include/lib paths in `LucentClient.vcxproj`
- Optional: libcurl (Release config references it)

Open `client/LucentClient.sln` and build `Release|x64`.

`$(SolutionDir)..` is on the include path so `shared/protocol/*.h` resolves.

## Lab vs private features

The default `main.cpp` on both sides registers **only** lab demo modules:

- `LabPing`, `LabConfigEcho`, `LabFrameStats`
- Client also registers `LabSettingsUI` (theme/menus)

Older competitive headers may still exist under `payload/jni/cheat/` and `client/LucentClient/` for reference; they are not wired into `InitFunctions` / `AddFunction`.
