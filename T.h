#pragma once

#include "Triangle.h"

#include "WorldTransform.h"
#include "ViewProjection.h"

class T
{
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw(const ViewProjection& viewProjection);

	void GetTexture(uint32_t textureHandle) { textureHandle_ = textureHandle; }

private:
	WorldTransform worldTransform_;
	Triangle* triangle_;

	uint32_t textureHandle_ = 0u;

};

