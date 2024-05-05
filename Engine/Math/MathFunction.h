#pragma once

#include <cmath>
#include <numbers>

#include "Vecter2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Transform.h"

const float M_PI = 3.14159265359f;

// 比較
template <typename T>
inline T Min(T a, T b) {
    return a < b ? a : b;
}
inline Vector2 Min(const Vector2& a, const Vector2& b) {
    return Vector2(Min(a.x, b.x), Min(a.y, b.y));
}

template <typename T>
inline T Max(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
inline void Swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}


template <typename T>
inline T Abs(T a) {
    return a > T(0) ? a : -a;
}

inline Vector2 Abs(const Vector2& v) {
    return Vector2(Abs(v.x), Abs(v.y));
}

Vector2 Add(const Vector2& v1, const Vector2& v2);
Vector2 Subtract(const Vector2& v1, const Vector2& v2);
Vector2 Multiply(float k, const Vector2& v);
//Vector2 Multiply(const myTransform& t, const Vector2& v);
inline float Dot(const Vector2& v1, const Vector2& v2);

inline float DistanceSquared(const Vector2& v1, const Vector2& v2);

// ベクトルの加法
Vector3 Add(const Vector3& v1, const Vector3& v2);
// ベクトルの減法
Vector3 Subtract(const Vector3& v1, const Vector3 v2);
// スカラー倍
Vector3 Multiply(float k, const Vector3& v);
// 内積
float Dot(const Vector3& v1, const Vector3& v2);
// 長さ(ノルム)
float Length(const Vector3& v1);
// 正規化
Vector3 Normalize(const Vector3& v1);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

Vector3 GetXAxis(const Matrix4x4& m);
Vector3 GetYAxis(const Matrix4x4& m);
Vector3 GetZAxis(const Matrix4x4& m);

// 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);
// 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);
// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);
// 単位行列の作成
Matrix4x4 MakeIdentity4x4();

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);
// 座標変換
Vector3 TransformVector3(const Vector3& vector, const Matrix4x4& matrix);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);
// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// XYZ回転行列
Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotation);

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Matrix4x4& rotateMatrix, const Vector3& translate);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& quaternion, const Vector3& translate);
Matrix4x4 MakeAffineMatrix(const Transform& transform);

// 透視投射行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
// 正射影行列
Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);
// ビューポート変更行列
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

// ビュー行列
Matrix4x4 MakeViewMatrix(const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeViewMatrix(const Quaternion& quaternion, const Vector3& translate);

Vector3 MapWorldToScreen(const Vector3& worldPosition,const Matrix4x4& matView, const Matrix4x4& matProjection ,float width, float height );

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);


// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
// 最近接点
// Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

float ConvertToRadians(float degree);

// 最短角度補間
float LerpShortAngle(float a, float b, float t);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

// 乗法単位元
Quaternion IndentityQuaternion();

Quaternion Conjugate(const Quaternion& q);

Quaternion Inverse(const Quaternion& q);

Quaternion Normalize(const Quaternion& q);

Quaternion Multiply(const Quaternion& q, const Quaternion& r);

float Norm(const Quaternion& q);

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

Vector3 RotateVector(const Vector3& vector, const Quaternion& q);

Matrix4x4 MakeRotateMatrix(const Quaternion& q);

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

float Dot(const Quaternion& q1, const Quaternion& q2);