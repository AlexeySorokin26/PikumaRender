#include <cmath>
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


float Len(Vector2 v) {
	return std::sqrt(v.x * v.x + v.y * v.y);
}

float Len(Vector3 v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector2 Add(Vector2 v1, Vector2 v2) {
	Vector2 res = { v1.x + v2.x, v1.y + v2.y };
	return res;
}

Vector3 Add(Vector3 v1, Vector3 v2) {
	Vector3 res = { v1.x + v2.x, v1.y + v2.y,  v1.z + v2.z };
	return res;
}

Vector2 Subtract(Vector2 v1, Vector2 v2) {
	Vector2 res = { v1.x - v2.x, v1.y - v2.y };
	return res;
}

Vector3 Subtract(Vector3 v1, Vector3 v2) {
	Vector3 res = { v1.x - v2.x, v1.y - v2.y,  v1.z - v2.z };
	return res;
}

Vector2 Mult(Vector2 v, float factor) {
	Vector2 res = { v.x * factor, v.y * factor };
	return res;
}

Vector3 Mult(Vector3 v, float factor) {
	Vector3 res = { v.x * factor, v.y * factor, v.z * factor };
	return res;
}

Vector2 Div(Vector2 v, float factor) {
	Vector2 res = { v.x / factor, v.y / factor };
	return res;
}

Vector3 Div(Vector3 v, float factor) {
	Vector3 res = { v.x / factor, v.y / factor, v.z / factor };
	return res;
}

Vector3 Cross(Vector3 v1, Vector3 v2) {
	Vector3 res = {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x,
	};
	return res;
}

float Dot(Vector2 v1, Vector2 v2) {
	float res = v1.x * v2.x + v1.y * v2.y;
	return res;
}
float Dot(Vector3 v1, Vector3 v2) {
	float res = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return res;
}

void Normalize(Vector3& v1) {
	Div(v1, Len(v1));
}

void Normalize(Vector2& v1) {
	Div(v1, Len(v1));
}