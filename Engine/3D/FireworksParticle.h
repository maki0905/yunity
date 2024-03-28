#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class FireworksParticle : public BaseParticle {
public:
	void Initialize(const Vector3& position, std::string textureName) override;
	void Peculiar() override;
	void Draw() override;



private:
	Random::RandomNumberGenerator rng;
	std::unique_ptr<ParticleDrawer> particleDrawer_;
};