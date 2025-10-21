#pragma once

#include "vector.h"

#include <array>

class Mat4 {
private:
	std::array<float, 16> data;

public:
	Mat4() { SetIdentity(); }

	void SetIdentity() {
		std::fill(data.begin(), data.end(), 0.0f);
		data[0] = data[5] = data[10] = data[15] = 1.0f;
	}

	float& operator()(int row, int col) {
		return data[row * 4 + col];
	}

	const float& operator()(int row, int col) const {
		return data[row * 4 + col];
	}

	float& operator[](int pos) {
		return data[pos];
	}

	const float& operator[](int pos) const {
		return data[pos];
	}

	static Mat4 Scale(float sx, float sy, float sz) {
		Mat4 res;
		res[0] = sx;
		res[5] = sy;
		res[10] = sz;
		return res;
	}

	Vector4 operator*(const Vector4& b) const {
		Vector4 res;
		res.x = data[0] * b.x + data[1] * b.y + data[2] * b.z + data[3] * b.w;
		res.y = data[4] * b.x + data[5] * b.y + data[6] * b.z + data[7] * b.w;
		res.z = data[8] * b.x + data[9] * b.y + data[10] * b.z + data[11] * b.w;
		res.w = data[12] * b.x + data[13] * b.y + data[14] * b.z + data[15] * b.w;
		return res;
	}

};