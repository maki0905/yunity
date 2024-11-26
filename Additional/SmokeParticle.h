#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class SmokeParticle : public yunity::BaseParticle {
public:
	void Initialize(yunity::Camera* camera);
	void Spawn(const Vector3& position) override;
	void Update() override;
private:
	Random::RandomNumberGenerator rng;
	float frequencyTime_;
	float spawnTime_;
	const float fixedTime_ = 1.0f / 30.0f;
};