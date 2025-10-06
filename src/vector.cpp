#include <math.h>
#include "vector.h"

Vector3 Vec3RotateX(Vector3 v, float angle) {
	Vector3 rotated_vector = {
		v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

Vector3 Vec3RotateY(Vector3 v, float angle) {
	Vector3 rotated_vector = {
		v.x * cos(angle) - v.z * sin(angle),
		v.y,
		v.x * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

Vector3 Vec3RotateZ(Vector3 v, float angle) {
	Vector3 rotated_vector = {
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z
	};
	return rotated_vector;
}
