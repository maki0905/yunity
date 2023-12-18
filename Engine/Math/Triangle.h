#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <stdint.h>
/// <param name="vertices[3]">3頂点</param>
struct Triangle {
	Vector3 vertices[3]; //!< 頂点
};

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);