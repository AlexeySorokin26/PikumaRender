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