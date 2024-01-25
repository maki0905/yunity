#pragma once

#include "Particle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class FireworksParticle {
public:
	void Initialize();

	void Update();

	void Draw(const Camera& camera);

private:
	Random::RandomNumberGenerator rng;

	std::unique_ptr<ParticleDrawer> particleDrawer_;
	std::list<Particle*> particles_;


};