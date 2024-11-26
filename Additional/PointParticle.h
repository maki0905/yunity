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
	const float power_ = 0.1f;
	const float torqe_ = 0.2f;
	float frequencyTime_;
	float spawnTime_;
	const float fixedTime_ = 1.0f / 30.0f;
};