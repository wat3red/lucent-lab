#pragma once
#include <math.h>

inline float easeInSine(float x) {
	return 1.0f - cosf((x * IM_PI) / 2.0f);
}

inline float easeOutExpo(float x) {
	return x == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * x);
}

inline float easeInOutCubic(float x) {
	return x < 0.5 ? 4 * x * x * x : 1 - powf(-2 * x + 2, 3) / 2;
}

inline float easeInExpo(float x) {
	return x == 0 ? 0 : powf(2, 10 * x - 10);
}

inline float easeOutQuint(float x) {
	return 1 - powf(1 - x, 5);
}