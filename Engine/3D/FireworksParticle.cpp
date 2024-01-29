#include "FireworksParticle.h"

void FireworksParticle::Initialize()
{
	isActive_ = true;
	particleDrawer_.reset(ParticleDrawer::Create("uvChecker.png"));
	particles_.clear();

	for (uint32_t i = 0; i < 10; ++i) {
		/*Particle* particle = new Particle();
		particle->transform.translate = { i * 2.0f, i * 2.0f, 0.0f };
		particle->velocity = { rng.NextFloatRange(-0.1f, 0.1f), rng.NextFloatRange(-0.1f, 0.1f), 0.0f };
		particle->particleForCPU.color = { rng.NextFloatRange(0.0f, 1.0f),rng.NextFloatRange(0.0f, 1.0f),rng.NextFloatRange(0.0f, 1.0f), 1.0f };
		particle->lifeTime = 5.0f;
		particles_.push_back(particle);*/
		Particle* particle = new Particle();
		particle->velocity = { 
			float(0.01f * cosf((36.0f * float(i)) * float(std::numbers::pi / 180.0f)) - 0.01f * sinf((36.0f * float(i)) * float(std::numbers::pi / 180.0f))), 
			float(0.01f * sinf((36.0f * float(i)) * float(std::numbers::pi / 180.0f)) + 0.01f * cosf((36.0f * float(i)) * float(std::numbers::pi / 180.0f))), 
			0.0f 
		};
		particle->particleForCPU.color = { rng.NextFloatRange(0.0f, 1.0f), rng.NextFloatRange(0.0f, 1.0f) , rng.NextFloatRange(0.0f, 1.0f) , 1.0f };
		particle->lifeTime = 5.0f;
		particles_.push_back(particle);
	}
}

void FireworksParticle::Peculiar()
{
	for (auto& particle : particles_) {
		particle->transform.translate = Add(particle->transform.translate, particle->velocity);
		particle->BillboardMatrix(*camera_);
	}

}

void FireworksParticle::Draw(const Camera& camera)
{
	particleDrawer_->Draw(particles_, camera);
}