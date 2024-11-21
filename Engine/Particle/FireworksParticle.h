#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class FireworksParticle : public BaseParticle {
public:
	void Initialize(const Vector3& position);
	void Update() override;
	//void Draw() override;
private:
	Random::RandomNumberGenerator rng;
};