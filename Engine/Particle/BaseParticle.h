#pragma once

#include "Particle.h"
#include "ParticleDrawer.h"

class BaseParticle {
public:
	virtual void Initialize(const Vector3& position) = 0;
	virtual void Update();
	virtual void Draw();

	bool GetIsActive() { return isActive_; }

	void SetCamera(Camera* camera) { camera_ = camera; }

protected:
	bool isActive_ = true;
	Camera* camera_;
	std::list<Particle> particles_;
	std::unique_ptr<ParticleDrawer> particleDrawer_;
};