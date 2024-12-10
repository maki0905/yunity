#pragma once
#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class FloatingParticle : public yunity::BaseParticle {
public:

	void Initialize(yunity::Camera* camera) override;
	void Spawn(const Vector3& position);
	void Update() override;
	void SetTarget(const Vector3 position) { targetPosition_ = position; isLerp_ = true; }
	//void Draw() override;
private:
	Random::RandomNumberGenerator rng;
	Vector3 targetPosition_;
	Vector3 centerPosition_;
	// 移動量
	const float power_ = 0.2f;
	// 生存時間
	float lifeTime_ = 1.0f;
	// 
	const float speedDegree_ = 10.0f;
	float angle_;
	float frequencyTime_;
	bool isLerp_;
};