#include "MathFunction.h"

#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <ranges>

float DegToRad()
{
	return 0.0174532924f;
}

float RadToDeg()
{
	return 57.29578f;
}

Vector2 Add(const Vector2& v1, const Vector2& v2)
{
	Vector2 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	return result;
}

Vector2 Subtract(const Vector2& v1, const Vector2& v2)
{
	Vector2 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	return result;
}

Vector2 Multiply(float k, const Vector2& v)
{
	Vector2 result;
	result.x = k * v.x;
	result.y = k * v.y;
	return result;
}

Vector2 Normalize(const Vector2& v)
{
	Vector2 result;
	float len = std::sqrtf(v.x * v.x + v.y * v.y);
	if (len == 0) {
		result = { 0.0f, 0.0f };
	}
	else {
		result = { v.x / len, v.y / len };
	}
	return result;
}

//Vector2 Multiply(const myTransform& t, const Vector2& v)
//{
//	float x = (t.q.c * v.x - t.q.s * v.y) + t.p.x;
//	float y = (t.q.s * v.x + t.q.c * v.y) + t.p.y;
//
//	return Vector2(x, y);
//}

inline float Dot(const Vector2& v1, const Vector2& v2)
{
	float result;
	result = v1.x * v2.x + v1.y * v2.y;
	return result;
}

inline float DistanceSquared(const Vector2& a, const Vector2& b)
{
	float result;
	Vector2 c = Subtract(a, b);
	result = Dot(c, c);
	return result;

}

float Cross(const Vector2& v1, const Vector2& v2)
{
	float result = v1.x * v2.y - v1.y * v2.x;
	return result;
}

// ベクトルの加法
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
}

// ベクトルの減法
Vector3 Subtract(const Vector3& v1, const Vector3 v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
}

// スカラー倍
Vector3 Multiply(float k, const Vector3& v) {
	Vector3 result;
	result.x = k * v.x;
	result.y = k * v.y;
	result.z = k * v.z;
	return result;
}

Vector3 Multiply(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.x * v2.x;
	result.y = v1.y * v2.y;
	result.z = v1.z * v2.z;
	return result;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	float result;
	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return result;
}

// 長さ(ノルム)
float Length(const Vector3& v1) {
	float result;
	result = sqrtf(Dot(v1, v1));
	return result;
}

// 正規化
Vector3 Normalize(const Vector3& v1) {
	Vector3 result = { 0, 0, 0 };
	float length = Length(v1);
	if (length != 0.0f) {
		result.x = v1.x / length;
		result.y = v1.y / length;
		result.z = v1.z / length;
	}

	return result;
}

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] };

	return result;
}

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	return result;
}

Vector3 GetXAxis(const Matrix4x4& m)
{
	return Vector3(m.m[0][0], m.m[0][1], m.m[0][2]);
}

Vector3 GetYAxis(const Matrix4x4& m)
{
	return Vector3(m.m[1][0], m.m[1][1], m.m[1][2]);
}

Vector3 GetZAxis(const Matrix4x4& m)
{
	return Vector3(m.m[2][0], m.m[2][1], m.m[2][2]);
}

Vector3 ConvertToRadians(const Vector3& v)
{
	Vector3 result = Multiply(float(M_PI) / 180.0f, v);

	return result;
}

Vector3 Divide(const Vector3& v, float s)
{
	if (s == 0.0f) {
		return Vector3(0.0f, 0.0f, 0.0f);
	}
	return Vector3(v.x / s, v.y / s, v.z / s);
}

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}

	return result;
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}

	return result;
}

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int j = 0; j < 4; j++) {
			result.m[line][j] = 0;
		}
	}
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] =
				m1.m[line][0] * m2.m[0][column] + m1.m[line][1] * m2.m[1][column] +
				m1.m[line][2] * m2.m[2][column] + m1.m[line][3] * m2.m[3][column];
		}
	}

	/*for (int32_t i = 0; i < 4; i++) {
		for (int32_t j = 0; j < 4; j++) {
			for (int32_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}*/


	return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result;
	float A = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	result.m[0][0] =
		(1 / A) * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
			m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
			m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][1] =
		(1 / A) * (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] -
			m.m[0][3] * m.m[2][1] * m.m[3][2] + m.m[0][3] * m.m[2][2] * m.m[3][1] +
			m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][2] =
		(1 / A) * (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
			m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
			m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	result.m[0][3] =
		(1 / A) * (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] -
			m.m[0][3] * m.m[1][1] * m.m[2][2] + m.m[0][3] * m.m[1][2] * m.m[2][1] +
			m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);
	result.m[1][0] =
		(1 / A) * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] -
			m.m[1][3] * m.m[2][0] * m.m[3][2] + m.m[1][3] * m.m[2][2] * m.m[3][0] +
			m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][1] =
		(1 / A) * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
			m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
			m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][2] =
		(1 / A) * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] -
			m.m[0][3] * m.m[1][0] * m.m[3][2] + m.m[0][3] * m.m[1][2] * m.m[3][0] +
			m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	result.m[1][3] =
		(1 / A) * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
			m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
			m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);
	result.m[2][0] =
		(1 / A) * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
			m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
			m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][1] =
		(1 / A) * (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] -
			m.m[0][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[2][1] * m.m[3][0] +
			m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][2] =
		(1 / A) * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
			m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
			m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	result.m[2][3] =
		(1 / A) * (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] -
			m.m[0][3] * m.m[1][0] * m.m[2][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] +
			m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);
	result.m[3][0] =
		(1 / A) * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] -
			m.m[1][2] * m.m[2][0] * m.m[3][1] + m.m[1][2] * m.m[2][1] * m.m[3][0] +
			m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][1] =
		(1 / A) * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
			m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
			m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][2] =
		(1 / A) * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] -
			m.m[0][2] * m.m[1][0] * m.m[3][1] + m.m[0][2] * m.m[1][1] * m.m[3][0] +
			m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	result.m[3][3] =
		(1 / A) * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
			m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
			m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);

	return result;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	return result;
}

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}

	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	return result;
}

// 座標変換
Vector3 TransformVector3(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
		1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
		1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
		1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
		1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}
	result.m[0][0] = std::cos(radian);
	result.m[0][2] = -std::sin(radian);
	result.m[2][0] = std::sin(radian);
	result.m[2][2] = std::cos(radian);
	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			if (line == column) {
				result.m[line][column] = 1;
			}
			else {
				result.m[line][column] = 0;
			}
		}
	}
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	return result;
}

Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotation)
{
	Matrix4x4 result = MakeIdentity4x4();
	result = Multiply(result, MakeRotateXMatrix(rotation.x));
	result = Multiply(result, MakeRotateYMatrix(rotation.y));
	result = Multiply(result, MakeRotateZMatrix(rotation.z));
	return result;
}

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}
	// スケーリング行列の作成
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	// X軸周り回転行列の作成
	Matrix4x4 rotateX = MakeRotateXMatrix(rot.x);
	// Y軸周り回転行列の作成
	Matrix4x4 rotateY = MakeRotateYMatrix(rot.y);
	// Z軸周り回転行列の作成
	Matrix4x4 rotateZ = MakeRotateZMatrix(rot.z);
	// 回転行列の合成(X回転 * Y回転 * Z回転)
	//Matrix4x4 rotateMatrix = Multiply(Multiply(rotateZ, rotateX), rotateY);
	//Matrix4x4 rotateMatrix = Multiply(rotateX,Multiply(rotateY, rotateZ));
	Matrix4x4 rotateMatrix = Multiply(rotateX, Multiply(rotateZ, rotateY));
	// 平行移動行列の作成
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Matrix4x4& rotateMatrix, const Vector3& translate)
{
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}
	// スケーリング行列の作成
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	// 平行移動行列の作成
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& quaternion, const Vector3& translate)
{
	Matrix4x4 result = MakeIdentity4x4();
	// スケーリング行列の作成
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	// 回転行列の作成
	Matrix4x4 rotateMatrix = MakeRotateMatrix(quaternion);

	// 平行移動行列の作成
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
	return result;
}

Matrix4x4 MakeAffineMatrix(const EulerTransform& transform)
{
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}
	// スケーリング行列の作成
	Matrix4x4 scaleMatrix = MakeScaleMatrix(transform.scale);

	// 回転行列の合成(Z回転 * X回転 * Y回転)
	Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(transform.rotate);

	// 平行移動行列の作成
	Matrix4x4 translateMatrix = MakeTranslateMatrix(transform.translate);

	result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);
	return result;
}

// 単位行列の作成
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				result.m[i][j] = 1;
			}
			else {
				result.m[i][j] = 0;
			}
		}
	}
	return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}

	result.m[0][0] = (1 / aspectRatio) * (1 / std::tan(fovY / 2.0f));
	result.m[1][1] = (1 / std::tan(fovY / 2.0f));
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1;
	result.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);
	return result;
}

Matrix4x4 MakeOrthographicMatrix(
	float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}

	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[3][3] = 1;
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);

	return result;
}

Matrix4x4 MakeOrthographicMatrix(float viewWidth, float viewHight, float nearZ, float farZ)
{
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}

	float left = -viewWidth / 2.0f;
	float right = viewWidth / 2.0f;
	float top = viewHight / 2.0f;
	float bottom = -viewHight / 2.0f;

	result.m[0][0] = 2.0f / (right - left);
	result.m[1][1] = 2.0f / (top - bottom);
	result.m[2][2] = 1.0f / (farZ - nearZ);
	result.m[3][3] = 1.0f;
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearZ / (nearZ - farZ);

	return result;
}

Matrix4x4 MakeViewportMatrix(
	float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result;
	for (int line = 0; line < 4; line++) {
		for (int column = 0; column < 4; column++) {
			result.m[line][column] = 0;
		}
	}

	result.m[0][0] = width / 2.0f;
	result.m[1][1] = -height / 2.0f;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][3] = 1;
	result.m[3][0] = left + (width / 2.0f);
	result.m[3][1] = top + (height / 2.0f);
	result.m[3][2] = minDepth;

	return result;
}

Matrix4x4 MakeViewMatrix(const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 result;
	result = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
	result = Inverse(result);
	return result;
}

Matrix4x4 MakeViewMatrix(const Quaternion& quaternion, const Vector3& translate)
{
	Matrix4x4 result;
	result = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, quaternion, translate);
	result = Inverse(result);
	return result;
}

Matrix4x4 MakeMatrixLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& upDirection)
{
	Vector3 eyeDirection = Subtract(targetPosition, eyePosition);

	Vector3 normal0 = Normalize(eyeDirection);

	Vector3 cross0 = Cross(upDirection, normal0);
	cross0 = Normalize(cross0);

	Vector3 cross1 = Cross(normal0, cross0);

	Vector3 negEyePosition = Multiply(-1.0f, eyePosition);

	float dot0 = Dot(cross0, negEyePosition);
	float dot1 = Dot(cross1, negEyePosition);
	float dot2 = Dot(normal0, negEyePosition);

	Matrix4x4 result;
	result.m[0][0] = cross0.x;
	result.m[0][1] = cross0.y;
	result.m[0][2] = cross0.z;
	result.m[0][3] = dot0;

	result.m[1][0] = cross1.x;
	result.m[1][1] = cross1.y;
	result.m[1][2] = cross1.z;
	result.m[1][3] = dot1;

	result.m[2][0] = normal0.x;
	result.m[2][1] = normal0.y;
	result.m[2][2] = normal0.z;
	result.m[2][3] = dot2;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	result = Transpose(result);

	return result;

}

Vector3 MapWorldToScreen(const Vector3& worldPosition, const Matrix4x4& matView, const Matrix4x4& matProjection, float width, float height)
{
	Vector3 result{};
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, width, height, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(matView, matProjection), matViewport);

	// ワールド->スクリーン座標変換
	result = TransformVector3(worldPosition, matViewProjectionViewport);
	return result;
}

Vector2 WorldToScreen(const Vector3& worldPosition, const Matrix4x4& matView, const Matrix4x4& matProjection, float width, float height)
{
	Vector2 result{};
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, width, height, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = Multiply(Multiply(matView, matProjection), matViewport);

	// ワールド->スクリーン座標変換
	result.x = TransformVector3(worldPosition, matViewProjectionViewport).x;
	result.y = TransformVector3(worldPosition, matViewProjectionViewport).y;
	return result;
}



Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
	Matrix4x4 result = MakeIdentity4x4();
	Vector3 n = Normalize(Cross(from, to));
	float cos = Dot(from, to);
	float sin = Length(Cross(from, to));
	result.m[0][0] = (n.x * n.x) * (1 - cos) + cos;
	result.m[0][1] = (n.x * n.y) * (1.0f - cos) + n.z * sin;
	result.m[0][2] = (n.x * n.z) * (1.0f - cos) - n.y * sin;
	result.m[1][0] = (n.x * n.y) * (1.0f - cos) - n.z * sin;
	result.m[1][1] = (n.y * n.y) * (1.0f - cos) + cos;
	result.m[1][2] = (n.y * n.z) * (1.0f - cos) + n.x * sin;
	result.m[2][0] = (n.x * n.z) * (1.0f - cos) + n.y * sin;
	result.m[2][1] = (n.y * n.z) * (1.0f - cos) - n.x * sin;
	result.m[2][2] = (n.z * n.z) * (1.0f - cos) + cos;
	return result;
}

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	float t = Dot(v1, v2) / (Length(v2) * Length(v2));
	result = Multiply(t, v2);
	return result;
}

Vector3 ClosestPoint(const Vector3& p1, const Vector3& p2, const Vector3& q1, const Vector3& q2)
{
	Vector3 direction1 = Subtract(p2, p1);
	Vector3 direction2 = Subtract(q2, q1);
	Vector3 diff = Subtract(p1, q1);

	float a = Dot(direction1, direction1);
	float e = Dot(direction2, direction2);
	float f = Dot(direction2, diff);

	float s = 0.0f;

	float t = 0.0f;

	float denom = a * e - Dot(direction1, direction2) * Dot(direction1, direction2);

	if (denom != 0.0f) {
		s = (Dot(direction1, direction2) * f - e * Dot(diff, direction1)) / denom;
		s = std::clamp(s, 0.0f, 1.0f);
	}

	t = (Dot(direction1, direction2) * s + f) / e;
	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 c1 = Add(p1, Multiply(s, direction1));
	Vector3 c2 = Add(q1, Multiply(t, direction2));

	Vector3 result = Multiply(0.5f, Add(c1, c2));

	return result;
}

float ConvertToRadians(float degree)
{
	float result = degree * float(M_PI) / 180.0f;
	return result;
}

float LerpShortAngle(float a, float b, float t) {
	// 角度差分を求める
	float diff = b - a;

	diff = std::fmodf(diff, float(M_PI));

	if (diff > M_PI) {
		diff = diff - float(M_PI);
	}
	else if (diff < -M_PI) {
		diff = diff + float(M_PI);

	}

	return a + diff * t;
}

Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t)
{
	Vector2 result = Add(v1, Multiply(t, Subtract(v2, v1)));
	return result;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result = Add(v1, Multiply(t, Subtract(v2, v1)));
	return result;
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	float s = ((1 - t) * Length(v1)) + (t * Length(v2));
	Vector3 e1 = Normalize(v1);
	Vector3 e2 = Normalize(v2);
	float an = std::acosf(Dot(v1, v2) * (1.0f / (Length(v1) * Length(v2))));
	if (an > 0.0f || an < 180.0f) {
		Vector3 v1e = Multiply(std::sinf((1 - t) * an) / std::sinf(an), e1);
		Vector3 v2e = Multiply(std::sinf(t * an) / std::sinf(an), e2);
		Vector3 result = Multiply(s, Add(v1e, v2e));
		return result;
	}
	return v1;
}

// 最近接点
// Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
//{
//	Vector3 result;
//	result = Add(segment.origin, Project(Subtract(point, segment.origin), segment.diff));
//	ret

Quaternion IndentityQuaternion()
{
	Quaternion result = { 0.0f, 0.0f, 0.0f, 1.0f };

	return result;
}

Quaternion Conjugate(const Quaternion& q)
{
	Quaternion result = { -q.x, -q.y, -q.z, q.w };

	return result;
}

Quaternion Inverse(const Quaternion& q)
{
	Quaternion result;
	Quaternion conj = Conjugate(q);
	float norm = Norm(q);
	norm = std::powf(norm, 2);
	result = { conj.x / norm, conj.y / norm, conj.z / norm, conj.w / norm };

	return result;
}

Quaternion Normalize(const Quaternion& q)
{
	Quaternion result = IndentityQuaternion();
	float norm = Norm(q);
	if (norm != 0.0f) {
		result = { q.x / norm, q.y / norm, q.z / norm, q.w / norm };
	}

	return result;
}

Quaternion Multiply(const Quaternion& q, const Quaternion& r)
{
	Quaternion result;
	Vector3 qxr = Cross({ q.x, q.y, q.z }, { r.x, r.y, r.z });
	Vector3 rq = Multiply(r.w, { q.x, q.y, q.z });
	Vector3 qr = Multiply(q.w, { r.x, r.y, r.z });
	result.w = (q.w * r.w) - Dot(Vector3{ q.x, q.y, q.z }, Vector3{ r.x, r.y, r.z });
	result.x = qxr.x + rq.x + qr.x;
	result.y = qxr.y + rq.y + qr.y;
	result.z = qxr.z + rq.z + qr.z;

	return result;
}

float Norm(const Quaternion& q)
{
	float result;
	float w2 = std::powf(q.w, 2);
	float x2 = std::powf(q.x, 2);
	float y2 = std::powf(q.y, 2);
	float z2 = std::powf(q.z, 2);
	result = sqrtf(w2 + x2 + y2 + z2);
	return result;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	Quaternion result;
	float sin = std::sinf(angle / 2.0f);
	result.w = std::cosf(angle / 2.0f);
	result.x = axis.x * sin;
	result.y = axis.y * sin;
	result.z = axis.z * sin;
	return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& q)
{

	Vector3 result;
	Quaternion r = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion conj = Conjugate(q);
	r = Multiply(Multiply(q, r), conj);
	result.x = r.x;
	result.y = r.y;
	result.z = r.z;
	return result;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& q)
{
	Matrix4x4 result = MakeIdentity4x4();

	result.m[0][0] = std::powf(q.w, 2) + std::powf(q.x, 2) - std::powf(q.y, 2) - std::powf(q.z, 2);
	result.m[0][1] = 2.0f * ((q.x * q.y) + (q.w * q.z));
	result.m[0][2] = 2.0f * ((q.x * q.z) - (q.w * q.y));

	result.m[1][0] = 2.0f * ((q.x * q.y) - (q.w * q.z));
	result.m[1][1] = std::powf(q.w, 2) - std::powf(q.x, 2) + std::powf(q.y, 2) - std::powf(q.z, 2);
	result.m[1][2] = 2.0f * ((q.y * q.z) + (q.w * q.x));

	result.m[2][0] = 2.0f * ((q.x * q.z) + (q.w * q.y));
	result.m[2][1] = 2.0f * ((q.y * q.z) - (q.w * q.x));
	result.m[2][2] = std::powf(q.w, 2) - std::powf(q.x, 2) - std::powf(q.y, 2) + std::powf(q.z, 2);

	return result;
}

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
	Quaternion result;
	float dot = Dot(q0, q1);
	float theta = 0.0f;
	float sin0 = 0.0f;
	float sin1 = 0.0f;
	if (dot < 0.0f) {
		dot = -dot;
		theta = std::acosf(dot);
		sin0 = std::sinf((1.0f - t) * theta) / std::sinf(theta);
		sin1 = std::sinf(t * theta) / std::sinf(theta);
		result.w = sin0 * -q0.w + sin1 * q1.w;
		result.x = sin0 * -q0.x + sin1 * q1.x;
		result.y = sin0 * -q0.y + sin1 * q1.y;
		result.z = sin0 * -q0.z + sin1 * q1.z;
	}
	else if (dot >= 1.0f - std::numeric_limits<float>::epsilon()) {
		result.w = (1.0f - t) * q0.w + t * q1.w;
		result.x = (1.0f - t) * q0.x + t * q1.x;
		result.y = (1.0f - t) * q0.y + t * q1.y;
		result.z = (1.0f - t) * q0.z + t * q1.z;
		return result;
	}
	else {
		theta = std::acosf(dot);
		sin0 = std::sinf((1.0f - t) * theta) / std::sinf(theta);
		sin1 = std::sinf(t * theta) / std::sinf(theta);
		result.w = sin0 * q0.w + sin1 * q1.w;
		result.x = sin0 * q0.x + sin1 * q1.x;
		result.y = sin0 * q0.y + sin1 * q1.y;
		result.z = sin0 * q0.z + sin1 * q1.z;
	}
	return result;
}

float Dot(const Quaternion& q1, const Quaternion& q2)
{
	float result = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
	return result;
}

Matrix3x3 MakeIdentity3x3()
{
	Matrix3x3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == j) {
				result.m[i][j] = 1;
			}
			else {
				result.m[i][j] = 0;
			}
		}
	}
	return result;
}

Matrix3x3 Multiply(const Matrix3x3& m1, const Matrix3x3& m2)
{
	Matrix3x3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = 0;
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			for (int k = 0; k < 3; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix3x3 Inverse(const Matrix3x3& m)
{
	Matrix3x3 result;
	float A = m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
		m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
		m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
	if (A == 0) {
		return MakeIdentity3x3();
	}
	result.m[0][0] = (1.0f / A) * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]);
	result.m[0][1] = (1.0f / A) * (m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2]);
	result.m[0][2] = (1.0f / A) * (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]);

	result.m[1][0] = (1.0f / A) * (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]);
	result.m[1][1] = (1.0f / A) * (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]);
	result.m[1][2] = (1.0f / A) * (m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2]);

	result.m[2][0] = (1.0f / A) * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);
	result.m[2][1] = (1.0f / A) * (m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1]);
	result.m[2][2] = (1.0f / A) * (m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0]);

	return result;
}

Matrix3x3 Transpose(const Matrix3x3& m)
{
	Matrix3x3 result;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = m.m[j][i];
		}
	}
	return result;
}

Matrix3x3 MakeRotateMatrix(const Vector3& rotation)
{
	Matrix3x3 result = MakeIdentity3x3();
	Matrix4x4 matrix = MakeIdentity4x4();
	matrix = Multiply(matrix, MakeRotateXMatrix(rotation.x));
	matrix = Multiply(matrix, MakeRotateYMatrix(rotation.y));
	matrix = Multiply(matrix, MakeRotateZMatrix(rotation.z));
	for (uint32_t i = 0; i < 3; i++) {
		for (uint32_t j = 0; j < 3; j++) {
			result.m[i][j] = matrix.m[i][j];
		}
	}
	return result;
}

Matrix3x3 ExtractRotation3x3(const Matrix4x4& m)
{
	Matrix3x3 result;
	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			result.m[y][x] = m.m[y][x];
		}
	}
	return result;
}

Vector3 TransformVector3(const Vector3& vector, const Matrix3x3& matrix)
{
	Vector3 result;
	result.x = matrix.m[0][0] * vector.x + matrix.m[0][1] * vector.y + matrix.m[0][2] * vector.z;
	result.y = matrix.m[1][0] * vector.x + matrix.m[1][1] * vector.y + matrix.m[1][2] * vector.z;
	result.z = matrix.m[2][0] * vector.x + matrix.m[2][1] * vector.y + matrix.m[2][2] * vector.z;

	return result;
}

void Barycentric(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& p, float& u, float& v, float& w)
{
	Vector3 v0 = Subtract(b, a), v1 = Subtract(c, a), v2 = Subtract(p, a);
	float d00 = Dot(v0, v0);
	float d01 = Dot(v0, v1);
	float d11 = Dot(v1, v1);
	float d20 = Dot(v2, v0);
	float d21 = Dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0f - v - w;
}
