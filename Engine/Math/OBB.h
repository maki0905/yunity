#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <cstdint>
#include "MathFunction.h"

struct OBB {
	Vector3 center; //!< 中心点
	Vector3 orientations[3]; //!< 座標軸。正規化・直交必須
	Vector3 size; //!< 座標軸方向の長さの半分。中心から面までの距離
};

bool TestAxis(const Vector3& axis, const OBB& obb1, const OBB& obb2);

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
