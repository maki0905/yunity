#pragma once

#include "Vector3.h"
#include "Vector4.h"

// ポイントライト
struct PointLight {
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f }; // ライトの色
	Vector3 position = { 0.0f, 2.0f, 0.0f };	// ライトの位置
	float intensity = 1.0f;						// 輝度
	float radius = 10.0f;					    // ライトの届く最大距離
	float decay = 1.0f;							// 減衰率
};												