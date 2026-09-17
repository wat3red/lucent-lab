#pragma once

#include <cstdio>

#define ENABLE_LOGGING 1

#if ENABLE_LOGGING

#define LOGI(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) printf("[ERROR] " fmt "\n", ##__VA_ARGS__)
#define LOGD(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)

#else

#define LOGI(...) do {} while(0)
#define LOGE(...) do {} while(0)
#define LOGD(...) do {} while(0)

#endif
