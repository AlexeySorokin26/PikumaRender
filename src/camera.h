#pragma once

#include "vector.h"

struct Camera {
	Vector3 position;
	Vector3 rotation;
	float fovAngle;
};