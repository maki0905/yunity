#pragma once

#include <memory>
#include <list>

#include "Camera.h"
#include "Particle.h"
#include "ParticleDrawer.h"

class ParticleManger
{
public:
	void Initialize();

	void Update();

	void Draw(const Camera& camera);

	void Add();


private:
	std::unique_ptr<ParticleDrawer> particleDrawer_;
	std::list<Particle*> particles_;
};

