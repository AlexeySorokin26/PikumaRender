#pragma once

#include "vector.h"

#include "array"

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
	std::array<Vector2, 3> points;
	uint32_t color;
	float averageDepth; // average of 3 z vertices
	bool operator<(Triangle& right) const {
		return this->averageDepth < right.averageDepth;
	}
	bool operator>(Triangle& right) const {
		return this->averageDepth > right.averageDepth;
	}
};

