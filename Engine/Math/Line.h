#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <stdint.h>
/// <summary>
/// 直線
/// </summary>
/// <param name="origin">始点</param>
/// <param name="diff">終点への差分ベクトル</param>
struct Line {
	Vector3 origin; //!< 始点
	Vector3 diff; //!< 終点への差分ベクトル
};

/// <summary>
/// 半直線
/// </summary>
struct Ray {
	Vector3 origin; //!< 始点
	Vector3 diff; //!< 終点への差分ベクトル
};

/// <summary>
/// 線分
/// </summary>
struct Segment
{
	Vector3 origin; //!< 始点
	Vector3 diff; //!< 終点への差分ベクトル
};

void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);