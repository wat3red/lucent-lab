# Payload scripts

Lab-only deploy helpers. They expect `adb` and `ndk-build` on PATH.

## deploy-emulator.bat

Builds `payload/jni` via NDK and pushes `libdaemon.so` to an emulator/device.

```bat
deploy-emulator.bat [serial] [package]
```

Defaults: serial `127.0.0.1:5555`, package `com.axlebolt.standoff2`.

The injector binary (`NullInject_x86`) is **not** committed. If present at
`payload/tools/NullInject_x86`, the script will push and run it; otherwise it
only deploys the library.
