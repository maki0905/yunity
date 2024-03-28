#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <stdint.h>
struct AABB{
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
	Vector3 extents; //!< 各軸方向の長さの半分
};

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);