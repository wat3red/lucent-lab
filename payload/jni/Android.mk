LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

DANDROID_CPP_FEATURES="rtti exceptions"
LOCAL_MODULE    := daemon
LOCAL_LDLIBS := -llog

LOCAL_SRC_FILES := 	main.cpp \
					utils.cpp \
					sdk\functions.cpp \
					oxorany\oxorany.cpp \
					network_manager\network_manager.cpp \
					sdk\Vector3.cpp \
					dhook\dhook.cpp \
					il2cpp\il2cpp_utils.cpp \


LOCAL_CPPFLAGS += -std=c++17
# Monorepo root for shared/protocol headers
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../..

LOCAL_LDFLAGS += -Wl,--version-script=$(LOCAL_PATH)/exports.map

include $(BUILD_SHARED_LIBRARY)

# post-build strip only if NDK_DEBUG=0
ifeq ($(NDK_DEBUG),0)
$(info Stripping binary...)
$(LOCAL_PATH)/../libs/$(APP_ABI)/libdaemon.so:
	$(NDK_HOME)/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-strip --strip-all $@
endif
