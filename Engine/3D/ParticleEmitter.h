#pragma once

#include "WorldTransform.h"
#include "Camera.h"
#include "PrimitiveDrawer.h"

class ParticleEmitter {
public:
	void Initialize(Transform transform, float frequency, uint32_t count);

	bool Update();

	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; }
	void SetTexturen(std::string textureName) { textureName_ = textureName; }

	Vector3 GetPosition() { return worldTransform_.translation_; }
	std::string GetTextureName() { return textureName_; }

private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
	// 発生頻度
	float frequency_;
	// 頻度用時刻
	float frequencyTime_;
	// 発生数
	uint32_t count_;

	std::string textureName_;

	std::unique_ptr<PrimitiveDrawer> primitiveDrawer_;
};