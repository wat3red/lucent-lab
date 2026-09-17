# ABI для сборки
APP_ABI := armeabi-v7a
# APP_ABI := x86 armeabi-v7a arm64-v8a

# Целевая версия Android
APP_PLATFORM := android-23

# Статическая STL
APP_STL := c++_static

# Position Independent Executable
APP_PIE := true

ifeq ($(NDK_DEBUG),1)
    # Debug build
    APP_CFLAGS     := -O0 -g
    APP_CPPFLAGS   += -DDEBUG
    APP_STRIP_MODE := none
else
    # Release build
    APP_CPPFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden -fno-rtti -fno-exceptions
    APP_CFLAGS     := -O0 -DNDEBUG -g0 # -02 
    APP_LDFLAGS   += -Wl,--strip-all, -s
    APP_CPPFLAGS   += -DNDEBUG
    APP_STRIP_MODE := --strip-all
endif

APP_CPPFLAGS += -fexceptions

APP_LDFLAGS := -llog
