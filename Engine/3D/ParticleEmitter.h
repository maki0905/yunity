#pragma once

#include "WorldTransform.h"
#include "Camera.h"
#include "PrimitiveDrawer.h"

class ParticleEmitter {
public:
	void Initialize(EulerTransform transform, float frequency, uint32_t count);

	bool Update();

	void Draw();

	void SetCamera(Camera* camera) { camera_ = camera; } 

	Vector3 GetPosition() { return worldTransform_.translation_; }

private:
	Camera* camera_ = nullptr;
	WorldTransform worldTransform_;
	// 発生頻度
	float frequency_;
	// 頻度用時刻
	float frequencyTime_;
	// 発生数
	uint32_t count_;

	std::unique_ptr<PrimitiveDrawer> primitiveDrawer_;
};