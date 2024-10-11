#pragma once

#include <memory>
#include <list>

#include "Camera.h"
#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "ParticleEmitter.h"

#include "FireworksParticle.h"

class ParticleManager
{
public:
	void Initialize();

	void Update();

	void Draw();

	void AddEmitter(ParticleEmitter* emitter);

	void SetCamera(Camera* camera) { camera_ = camera; }

private:
	Camera* camera_ = nullptr;
	std::list<ParticleEmitter*> emitters_;
	std::list<BaseParticle*> particles_;
};

