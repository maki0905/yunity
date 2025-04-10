#pragma once

#include <memory>
#include <list>

#include "Camera.h"
#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "ParticleEmitter.h"

#include "Particle/FireworksParticle.h"

class ParticleManager
{
public:
	void Initialize();

	void Update();

	void Draw();

	void AddEmitter(ParticleEmitter* emitter);

	void SetCamera(yunity::Camera* camera) { camera_ = camera; }

private:
	yunity::Camera* camera_ = nullptr;
	std::list<ParticleEmitter*> emitters_;
	std::list<std::unique_ptr<yunity::BaseParticle>> particles_;
};

