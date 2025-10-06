#pragma once

#include "vector.h"

/// @brief Stores vertex index
struct Face {
	int a;
	int b;
	int c;
};

/// @brief Sores the actual vec2 points of the triangle in the screen
struct Triangle {
	Vector2 points[3];
};

