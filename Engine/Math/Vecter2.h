#pragma once
#include <cmath>
#include <cfloat>

inline bool myIsValid(float i) {
	return isfinite(i);
}


/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {

	Vector2() {};
	Vector2(float xIn, float yIn) : x(xIn), y(yIn){}

	void SetZero() { x = 0.0f; y = 0.0f; }

	void Set(float xIn, float yIn) { x = xIn; y = yIn; }

	float Length()const {
		return std::sqrtf(x * x + y * y);
	}

	float LengthSquared() const {
		return x * x + y * y;
	}

	float Normalize() {
		float length = Length();
		if (length < FLT_EPSILON) {
			return 0.0f;
		}

		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;

		return length;
	}

	bool IsValid() const {
		return myIsValid(x) && myIsValid(y);
	}

	Vector2 Skew() const {
		return Vector2(-y, x);
	}

	float x;
	float y;

};