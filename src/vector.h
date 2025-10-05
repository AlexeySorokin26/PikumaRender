#pragma once

struct Vector2 {
	float x;
	float y;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

Vector3 Vec3RotateX(Vector3 v, float angle);

Vector3 Vec3RotateY(Vector3 v, float angle);

Vector3 Vec3RotateZ(Vector3 v, float angle);
