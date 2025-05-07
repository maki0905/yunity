#pragma once

#include <cmath>
#include <numbers>
#include <iostream>
#include <algorithm>

#include "Vecter2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "Quaternion.h"
#include "Transform.h"

//float DegToRad() { return 0.0174532924f; }
//float RadToDeg() { return 57.29578f; }

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

/// <summary>
/// 角度変換
/// </summary>
/// <returns></returns>
float DegToRad();

/// <summary>
/// 角度変換
/// </summary>
/// <returns></returns>
float RadToDeg();

/// <summary>
/// 加法
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector2 Add(const Vector2& v1, const Vector2& v2);

/// <summary>
/// 減法
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector2 Subtract(const Vector2& v1, const Vector2& v2);

/// <summary>
/// スカラー倍
/// </summary>
/// <param name="k"></param>
/// <param name="v"></param>
/// <returns></returns>
Vector2 Multiply(float k, const Vector2& v);

/// <summary>
/// 正規化
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
Vector2 Normalize(const Vector2& v);
//Vector2 Multiply(const myTransform& t, const Vector2& v);

/// <summary>
/// 内積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector2& v1, const Vector2& v2);

/// <summary>
/// 距離二乗
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float DistanceSquared(const Vector2& v1, const Vector2& v2);

/// <summary>
/// クロス積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Cross(const Vector2& v1, const Vector2& v2);

/// <summary>
/// 加法
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 減法
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Subtract(const Vector3& v1, const Vector3 v2);

/// <summary>
/// スカラー倍
/// </summary>
/// <param name="k"></param>
/// <param name="v"></param>
/// <returns></returns>
Vector3 Multiply(float k, const Vector3& v);

/// <summary>
/// 乗法
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Multiply(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 内積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 長さ(ノルム)
/// </summary>
/// <param name="v1"></param>
/// <returns></returns>
float Length(const Vector3& v1);

/// <summary>
/// 正規化
/// </summary>
/// <param name="v1"></param>
/// <returns></returns>
Vector3 Normalize(const Vector3& v1);

/// <summary>
/// ベクトル変換
/// </summary>
/// <param name="v"></param>
/// <param name="m"></param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

/// <summary>
/// クロス積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 行列のx軸回転
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Vector3 GetXAxis(const Matrix4x4& m);

/// <summary>
/// 行列のy軸回転
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Vector3 GetYAxis(const Matrix4x4& m);

/// <summary>
/// 行列のz軸回転
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Vector3 GetZAxis(const Matrix4x4& m);

/// <summary>
/// 
/// </summary>
/// <param name="v"></param>
/// <returns></returns>
Vector3 ConvertToRadians(const Vector3& v);

/// <summary>
/// 
/// </summary>
/// <param name="v"></param>
/// <param name="s"></param>
/// <returns></returns>
Vector3 Divide(const Vector3& v, float s);



/// <summary>
/// 加法
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 減法
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 積
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 逆行列
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix4x4 Inverse(const Matrix4x4& m);

/// <summary>
/// 転置行列
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix4x4 Transpose(const Matrix4x4& m);

/// <summary>
/// 単位行列
/// </summary>
/// <returns></returns>
Matrix4x4 MakeIdentity4x4();


/// <summary>
/// 平行移動行列
/// </summary>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// 拡大縮小行列
/// </summary>
/// <param name="scale"></param>
/// <returns></returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// 座標変換
/// </summary>
/// <param name="vector"></param>
/// <param name="matrix"></param>
/// <returns></returns>
Vector3 TransformVector3(const Vector3& vector, const Matrix4x4& matrix);

/// <summary>
/// X軸回転行列
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
Matrix4x4 MakeRotateXMatrix(float radian);

/// <summary>
/// Y軸回転行列
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
Matrix4x4 MakeRotateYMatrix(float radian);


/// <summary>
/// Z軸回転行列
/// </summary>
/// <param name="radian"></param>
/// <returns></returns>
Matrix4x4 MakeRotateZMatrix(float radian);

/// <summary>
/// XYZ回転行列
/// </summary>
/// <param name="rotation"></param>
/// <returns></returns>
Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotation);

/// <summary>
/// アフィン変換行列
/// </summary>
/// <param name="scale"></param>
/// <param name="rot"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

/// <summary>
/// アフィン変換行列
/// </summary>
/// <param name="scale"></param>
/// <param name="rotateMatrix"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Matrix4x4& rotateMatrix, const Vector3& translate);

/// <summary>
/// アフィン変換行列
/// </summary>
/// <param name="scale"></param>
/// <param name="quaternion"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& quaternion, const Vector3& translate);

/// <summary>
/// アフィン変換行列
/// </summary>
/// <param name="transform"></param>
/// <returns></returns>
Matrix4x4 MakeAffineMatrix(const EulerTransform& transform);

/// <summary>
/// 透視投射行列
/// </summary>
/// <param name="fovY"></param>
/// <param name="aspectRatio"></param>
/// <param name="nearClip"></param>
/// <param name="farClip"></param>
/// <returns></returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

/// <summary>
/// 正射影行列
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="nearClip"></param>
/// <param name="farClip"></param>
/// <returns></returns>
Matrix4x4 MakeOrthographicMatrix(
    float left, float top, float right, float bottom, float nearClip, float farClip);

/// <summary>
/// 正射影行列
/// </summary>
/// <param name="viewWidth"></param>
/// <param name="viewHight"></param>
/// <param name="nearZ"></param>
/// <param name="farZ"></param>
/// <returns></returns>
Matrix4x4 MakeOrthographicMatrix(float viewWidth, float viewHight, float nearZ, float farZ);

/// <summary>
/// ビューポート変更行列
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="minDepth"></param>
/// <param name="maxDepth"></param>
/// <returns></returns>
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

/// <summary>
/// ビュー行列
/// </summary>
/// <param name="rotate"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeViewMatrix(const Vector3& rotate, const Vector3& translate);


/// <summary>
/// ビュー行列
/// </summary>
/// <param name="quaternion"></param>
/// <param name="translate"></param>
/// <returns></returns>
Matrix4x4 MakeViewMatrix(const Quaternion& quaternion, const Vector3& translate);

/// <summary>
/// ビュー行列
/// </summary>
/// <param name="eyePostion"></param>
/// <param name="targetPosition"></param>
/// <param name="upDirection"></param>
/// <returns></returns>
Matrix4x4 MakeMatrixLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& upDirection);


/// <summary>
/// ワールド座標からスクリーン座標
/// </summary>
/// <param name="worldPosition"></param>
/// <param name="matView"></param>
/// <param name="matProjection"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
Vector3 MapWorldToScreen(const Vector3& worldPosition, const Matrix4x4& matView, const Matrix4x4& matProjection, float width, float height);

/// <summary>
/// スクリーン座標からワールド座標
/// </summary>
/// <param name="worldPosition"></param>
/// <param name="matView"></param>
/// <param name="matProjection"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <returns></returns>
Vector2 WorldToScreen(const Vector3& worldPosition, const Matrix4x4& matView, const Matrix4x4& matProjection, float width, float height);

/// <summary>
/// 行列線形補間
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <returns></returns>
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

/// <summary>
/// 正射影ベクトル
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Project(const Vector3& v1, const Vector3& v2);

// 最近接点
// Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

/// <summary>
/// クロースポイント
/// </summary>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <returns></returns>
Vector3 ClosestPoint(const Vector3& p1, const Vector3& p2, const Vector3& q1, const Vector3& q2);

Vector3 TransformPoint(const Vector3& point, const Matrix4x4& m);

/// <summary>
/// 
/// </summary>
/// <param name="degree"></param>
/// <returns></returns>
float ConvertToRadians(float degree);

/// <summary>
/// 最短角度補間
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="t"></param>
/// <returns></returns>
float LerpShortAngle(float a, float b, float t);

/// <summary>
/// 線形補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t);

/// <summary>
/// 線形補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// 曲線補間
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// 乗法単位元
/// </summary>
/// <returns></returns>
Quaternion IndentityQuaternion();

/// <summary>
/// 共役
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
Quaternion Conjugate(const Quaternion& q);

/// <summary>
/// 逆クォータニオン
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
Quaternion Inverse(const Quaternion& q);

/// <summary>
/// 正規化
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
Quaternion Normalize(const Quaternion& q);

/// <summary>
/// 乗算
/// </summary>
/// <param name="q"></param>
/// <param name="r"></param>
/// <returns></returns>
Quaternion Multiply(const Quaternion& q, const Quaternion& r);

/// <summary>
/// ノルム
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
float Norm(const Quaternion& q);

/// <summary>
/// 回転クォータニオン
/// </summary>
/// <param name="axis"></param>
/// <param name="angle"></param>
/// <returns></returns>
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

/// <summary>
/// 回転ベクトル
/// </summary>
/// <param name="vector"></param>
/// <param name="q"></param>
/// <returns></returns>
Vector3 RotateVector(const Vector3& vector, const Quaternion& q);

/// <summary>
/// 回転行列
/// </summary>
/// <param name="q"></param>
/// <returns></returns>
Matrix4x4 MakeRotateMatrix(const Quaternion& q);

/// <summary>
/// 曲線補間
/// </summary>
/// <param name="q0"></param>
/// <param name="q1"></param>
/// <param name="t"></param>
/// <returns></returns>
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

/// <summary>
/// 内積
/// </summary>
/// <param name="q1"></param>
/// <param name="q2"></param>
/// <returns></returns>
float Dot(const Quaternion& q1, const Quaternion& q2);

/// <summary>
/// 単位行列
/// </summary>
/// <returns></returns>
Matrix3x3 MakeIdentity3x3();

/// <summary>
/// 行列の積
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <returns></returns>
Matrix3x3 Multiply(const Matrix3x3& m1, const Matrix3x3& m2);

/// <summary>
/// 逆行列
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix3x3 Inverse(const Matrix3x3& m);

/// <summary>
/// 転置行列
/// </summary>
/// <param name="m"></param>
/// <returns></returns>
Matrix3x3 Transpose(const Matrix3x3& m);

/// <summary>
/// 回転行列
/// </summary>
/// <param name="rotation"></param>
/// <returns></returns>
Matrix3x3 MakeRotateMatrix(const Vector3& rotation);

/// <summary>
/// Matrix4x4からMatrix3x3に変換
/// </summary>
Matrix3x3 ExtractRotation3x3(const Matrix4x4& m);

/// <summary>
/// ベクトル変換
/// </summary>
/// /// <param name="vector"></param>
/// /// <param name="matrix"></param>
/// /// <returns></returns>
Vector3 TransformVector3(const Vector3& vector, const Matrix3x3& matrix);

void Barycentric(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& p, float& u, float& v, float& w);