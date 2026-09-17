@echo off
setlocal
REM Emulator lab deploy — relative to repo, no personal absolute paths.
REM Requires: adb on PATH, ndk-build on PATH, an emulator/device reachable via ADB.
REM Usage: deploy-emulator.bat [serial] [package]

set "REPO_ROOT=%~dp0..\.."
set "PAYLOAD_DIR=%~dp0.."
set "SERIAL=%~1"
if "%SERIAL%"=="" set "SERIAL=127.0.0.1:5555"
set "PKG=%~2"
if "%PKG%"=="" set "PKG=com.axlebolt.standoff2"
set "INJECTOR=%PAYLOAD_DIR%\tools\NullInject_x86"

adb disconnect %SERIAL% >nul 2>&1
adb connect %SERIAL%

pushd "%PAYLOAD_DIR%"
call ndk-build NDK_PROJECT_PATH="%PAYLOAD_DIR%" NDK_DEBUG=1
if errorlevel 1 (
  echo ndk-build failed
  popd
  exit /b 1
)
popd

adb -s %SERIAL% shell "rm -rf /data/local/tmp/lab && mkdir -p /data/local/tmp/lab"
adb -s %SERIAL% push "%PAYLOAD_DIR%\libs\armeabi-v7a\libdaemon.so" /data/local/tmp/lab/
adb -s %SERIAL% shell "chmod 755 /data/local/tmp/lab/libdaemon.so"

if exist "%INJECTOR%" (
  adb -s %SERIAL% push "%INJECTOR%" /data/local/tmp/lab/
  adb -s %SERIAL% shell "chmod 755 /data/local/tmp/lab/NullInject_x86"
  adb -s %SERIAL% forward tcp:64108 tcp:64108
  adb -s %SERIAL% logcat --clear
  adb -s %SERIAL% shell "/data/local/tmp/lab/NullInject_x86 -p %PKG% -lib /data/local/tmp/lab/libdaemon.so"
  adb -s %SERIAL% logcat --format=raw -s libserver
) else (
  echo Injector binary not present at "%INJECTOR%".
  echo Place a local injector there ^(not committed^) or load libdaemon.so another way.
  echo Library is at /data/local/tmp/lab/libdaemon.so on device.
)
