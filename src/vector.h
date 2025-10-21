#pragma once

struct Vector2 {
	float x;
	float y;
	Vector2(float x = 0., float y = 0.) : x(x), y(y) {}
};

struct Vector3 : public Vector2 {
	float z;
	Vector3(float x = 0., float y = 0., float z = 0.) : Vector2(x, y), z(z) {}
};

struct Vector4 : public Vector3 {
	float w;
	Vector4(float x = 0., float y = 0., float z = 0., float w = 0.) : Vector3(x, y, z), w(w) {}
};

Vector3 Vec3RotateX(Vector3 v, float angle);

Vector3 Vec3RotateY(Vector3 v, float angle);

Vector3 Vec3RotateZ(Vector3 v, float angle);

float Len(Vector2 v);
float Len(Vector3 v);

Vector2 Add(Vector2 v1, Vector2 v2);
Vector3 Add(Vector3 v1, Vector3 v2);

Vector2 Subtract(Vector2 v1, Vector2 v2);
Vector3 Subtract(Vector3 v1, Vector3 v2);

Vector2 Mult(Vector2 v, float factor);
Vector3 Mult(Vector3 v, float factor);

Vector2 Div(Vector2 v, float factor);
Vector3 Div(Vector3 v, float factor);

Vector3 Cross(Vector3 v1, Vector3 v2);

float Dot(Vector2 v1, Vector2 v2);
float Dot(Vector3 v1, Vector3 v2);

void Normalize(Vector3& v1);
void Normalize(Vector2& v1);

inline Vector4 Vec3ToVec4(const Vector3 v3) {
	return Vector4(v3.x, v3.y, v3.z, 1.0);
}
inline Vector3 Vec4ToVec3(const Vector4 v4) {
	return Vector3(v4.x, v4.y, v4.z);
}