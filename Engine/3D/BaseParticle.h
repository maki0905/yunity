#pragma once

#include "Particle.h"
#include "ParticleDrawer.h"

class BaseParticle {
public:
	virtual void Initialize() = 0;
	virtual void Peculiar() = 0;
	void Updata();
	virtual void Draw(const Camera& camera) = 0;

	void GetCamera(Camera* camera) { camera_ = camera; }

public:
	bool isActive_ = true;
	Camera* camera_;
	std::list<Particle*> particles_;
	std::unique_ptr<ParticleDrawer> particleDrawer_;


};