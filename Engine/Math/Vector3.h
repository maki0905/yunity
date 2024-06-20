#pragma once

#include <cmath>
#include <cfloat>


/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {

	void SetZero() { x = 0.0f; y = 0.0f; z = 0.0f; }

	float Length()const {
		return std::sqrtf(x * x + y * y + z * z);
	}

	float LengthSquared() const {
		return x * x + y * y + z * z;
	}

	Vector3 Normalize() {
		float length = Length();
		if (length < FLT_EPSILON) {
			return {0.0f, 0.0f, 0.0f};
		}

		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;
		z *= invLength;

		return { x, y, z };
	}

	float x;
	float y;
	float z;
};