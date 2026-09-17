#pragma once

#include <android/log.h>

constexpr const char* LOG_TAG = "libserver";
#define ENABLE_LOGGING 1

#if ENABLE_LOGGING

#define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, "[INFO] " fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "[ERROR] " fmt, ##__VA_ARGS__)
#define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "[DEBUG] " fmt, ##__VA_ARGS__)

#else

#define LOGI(...) do {} while(0)
#define LOGE(...) do {} while(0)
#define LOGD(...) do {} while(0)

#endif