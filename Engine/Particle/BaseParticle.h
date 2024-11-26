#pragma once

#include "Particle.h"
#include "ParticleDrawer.h"

namespace yunity {
	class BaseParticle {
	public:
		virtual ~BaseParticle() {};

		virtual void Initialize(Camera* camera) = 0;
		virtual void Spawn(const Vector3& position) = 0;
		virtual void Update();
		virtual void Update(std::list<Particle>* particles);
		virtual void Draw();

		bool GetIsActive() { return isActive_; }

		void SetCamera(yunity::Camera* camera) { camera_ = camera; }
		void SetFixedTime(float fixedTime) { fixedTime_ = fixedTime; }

	protected:
		bool isActive_ = true;
		yunity::Camera* camera_;
		std::list<Particle> particles_;
		std::unique_ptr<yunity::ParticleDrawer> particleDrawer_;
	private:
		float fixedTime_;
	};
}