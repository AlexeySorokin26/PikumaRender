#pragma once

#include "vector.h"

#include <array>
#include <cmath>

class Mat4
{
private:
	std::array<float, 16> data;

public:
	Mat4() { SetIdentity(); }

	void SetIdentity()
	{
		std::fill(data.begin(), data.end(), 0.0f);
		data[0] = data[5] = data[10] = data[15] = 1.0f;
	}

	float &operator()(int row, int col)
	{
		return data[row * 4 + col];
	}

	const float &operator()(int row, int col) const
	{
		return data[row * 4 + col];
	}

	float &operator[](int pos)
	{
		return data[pos];
	}

	const float &operator[](int pos) const
	{
		return data[pos];
	}

	static Mat4 Scale(float sx, float sy, float sz)
	{
		Mat4 res;
		res[0] = sx;
		res[5] = sy;
		res[10] = sz;
		return res;
	}

	static Mat4 Translation(float tx, float ty, float tz)
	{
		Mat4 res;
		res[3] = tx;
		res[7] = ty;
		res[11] = tz;
		return res;
	}

	Vector4 operator*(const Vector4 &b) const
	{
		Vector4 res;
		res.x = data[0] * b.x + data[1] * b.y + data[2] * b.z + data[3] * b.w;
		res.y = data[4] * b.x + data[5] * b.y + data[6] * b.z + data[7] * b.w;
		res.z = data[8] * b.x + data[9] * b.y + data[10] * b.z + data[11] * b.w;
		res.w = data[12] * b.x + data[13] * b.y + data[14] * b.z + data[15] * b.w;
		return res;
	}
	
	Mat4 operator*(const Mat4 &b) const
	{
		Mat4 res;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				res.data[i * 4 + j] = 0;
				for (int k = 0; k < 4; k++)
				{
					res.data[i * 4 + j] += data[i * 4 + k] * b.data[k * 4 + j];
				}
			}
		}
		return res;
	}

	static Mat4 RotationAroundXAxis(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		Mat4 res;
		res[0] = 1;
		res[1] = 0;
		res[2] = 0;
		res[3] = 0;
		res[4] = 0;
		res[5] = c;
		res[6] = -s;
		res[7] = 0;
		res[8] = 0;
		res[9] = s;
		res[10] = c;
		res[11] = 0;
		res[12] = 0;
		res[13] = 0;
		res[14] = 0;
		res[15] = 1;
		return res;
	}

	static Mat4 RotationAroundYAxis(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		Mat4 res;
		res[0] = c;
		res[1] = 0;
		res[2] = s;
		res[3] = 0;
		res[4] = 0;
		res[5] = 1;
		res[6] = 0;
		res[7] = 0;
		res[8] = -s;
		res[9] = 0;
		res[10] = c;
		res[11] = 0;
		res[12] = 0;
		res[13] = 0;
		res[14] = 0;
		res[15] = 1;
		return res;
	}

	static Mat4 RotationAroundZAxis(float angle)
	{
		float c = cos(angle);
		float s = sin(angle);
		Mat4 res;
		res[0] = c;
		res[1] = -s;
		res[2] = 0;
		res[3] = 0;
		res[4] = s;
		res[5] = c;
		res[6] = 0;
		res[7] = 0;
		res[8] = 0;
		res[9] = 0;
		res[10] = 1;
		res[11] = 0;
		res[12] = 0;
		res[13] = 0;
		res[14] = 0;
		res[15] = 1;
		return res;
	}
};