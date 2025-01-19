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
	const float power_ = 0.1f;
	const float torqe_ = 0.2f;
};