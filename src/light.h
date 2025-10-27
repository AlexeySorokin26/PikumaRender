#pragma once

#include "vector.h"

struct Light {
	Light(const Vector3 d = Vector3(0, 0, 0)) : direction(d) {
	}
	Vector3 direction;

	static uint32_t LightApplyIntensity(uint32_t originalColor, float percentageFactor) {
		if (percentageFactor < 0) percentageFactor = 0;
		if (percentageFactor > 1) percentageFactor = 1;

		uint32_t a = (originalColor & 0xFF000000);
		uint32_t r = (originalColor & 0x00FF0000) >> 16;;
		uint32_t g = (originalColor & 0x0000FF00) >> 8;
		uint32_t b = (originalColor & 0x000000FF);

		r = static_cast<uint32_t>(r * percentageFactor);
		g = static_cast<uint32_t>(g * percentageFactor);
		b = static_cast<uint32_t>(b * percentageFactor);

		uint32_t newColor = a | (r << 16) | (g << 8) | b;

		return newColor;
	}
};

