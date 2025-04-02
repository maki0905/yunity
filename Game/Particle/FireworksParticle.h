#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class FireworksParticle : public yunity::BaseParticle {
public:
	void Initialize(yunity::Camera* camera);
	void Spawn(const Vector3& position) override;
	void Update() override;
private:
	Random::RandomNumberGenerator rng;
	std::unique_ptr<yunity::Model> model_;
	static const int particleQuantity_ = 10;
    float power_;
    float torqe_;
	float ascent_;
	float lifeTime_;
	Vector3 spawnPosition_;
	Vector3 minSpawnPosition_;
	Vector3 maxSpawnPosition_;
	Vector3 minVelocity_;
	Vector3 maxVelocity_;
};