#pragma once
#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class PointParticle : public yunity::BaseParticle {
public:
	void Initialize(yunity::Camera* camera) override;
	void Spawn(const Vector3& position);
	void Update() override;
	void Draw() override;
private:
	Random::RandomNumberGenerator rng;
	std::list<Particle> circleParticles_;
	std::unique_ptr<yunity::ParticleDrawer> circleParticleDrawer_;
	std::unique_ptr<yunity::ParticleDrawer> ellipseParticleDrawer_;
	// 移動量
	const float power_ = 0.2f;
	// 生存時間
	std::array<const float, 2> lifeTime_ = { 0.2f, 0.2f };
	// 進行時間
	const float fixedTime_ = 1.0f / 30.0f;
	// 
	const float degree_ = 90.0f;

	float frequencyTime_;
	float spawnTime_;
};