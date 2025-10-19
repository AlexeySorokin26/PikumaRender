#pragma once

#include "vector.h"

/// @brief Stores vertex index
struct Face {
	int a;
	int b;
	int c;
	uint32_t color;
};

/// @brief Sores the actual vec2 points of the triangle in the screen
struct Triangle {
	Triangle() {
		points[0] = Vector2{ 0., 0. };
		points[1] = Vector2{ 0., 0. };
		points[2] = Vector2{ 0., 0. };
	}
	Vector2 points[3];
	uint32_t color;
};

