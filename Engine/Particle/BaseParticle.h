#pragma once

#include "Particle.h"
#include "ParticleDrawer.h"

namespace yunity {
	class BaseParticle {
	public:
		virtual void Initialize(const Vector3& position) = 0;
		virtual void Update();
		virtual void Draw();

		bool GetIsActive() { return isActive_; }

		void SetCamera(yunity::Camera* camera) { camera_ = camera; }

	protected:
		bool isActive_ = true;
		yunity::Camera* camera_;
		std::list<Particle> particles_;
		std::unique_ptr<yunity::ParticleDrawer> particleDrawer_;
	};
}