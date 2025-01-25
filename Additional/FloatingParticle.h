#pragma once
#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

/*
* @brief 浮遊パーティクル
*/
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
	float power_;
	// 生存時間
	float lifeTime_;
	// 
	float speedDegree_;
	float angle_;
	float frequencyTime_;
	bool isLerp_;
	Vector3 minPosition_ = {-2.0f, -2.0f, 0.0f};
	Vector3 maxPosition_ = {2.0f, 2.0f, 0.0f};
};