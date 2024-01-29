#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class FireworksParticle : public BaseParticle {
public:
	void Initialize() override;
	void Peculiar() override;
	void Draw(const Camera& camera) override;

private:
	Random::RandomNumberGenerator rng;
	std::unique_ptr<ParticleDrawer> particleDrawer_;
};