#pragma once

#include <memory>
#include <list>

#include "Camera.h"
#include "BaseParticle.h"
#include "ParticleDrawer.h"

#include "FireworksParticle.h"

class ParticleManager
{
public:
	void Initialize();

	void Update();

	void Draw(const Camera& camera);

	void Add(Camera* camera);


private:
	std::list<BaseParticle*> particles_;
};

