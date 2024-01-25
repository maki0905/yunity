#include "FireworksParticle.h"

void FireworksParticle::Initialize()
{
	particleDrawer_.reset(ParticleDrawer::Create(ParticleDrawer::Type::kBoard, "uvChecker.png"));
	particles_.clear();

	for (uint32_t i = 0; i < 10; ++i) {
		Particle* particle = new Particle();
		particle->velocity = { rng.NextFloatRange(-1.0f, 1.0), rng.NextFloatRange(-1.0f, 1.0f), 0.0f };
		particle->color = { rng.NextFloatRange(0.0f, 1.0f), rng.NextFloatRange(0.0f, 1.0f) , rng.NextFloatRange(0.0f, 1.0f) , 0.0f };
		particle->lifeTime = 5.0f;
		particles_.emplace_back(particle);
	}

}

void FireworksParticle::Update()
{

	for (auto& particle : particles_) {
		particle->currentTime += 1.0f / 60.0f;
	}

	particles_.remove_if([](Particle* particle) {
		if (particle->lifeTime <= particle->currentTime) {
			delete particle;
			return true;
		}
		return false;
	});
}

void FireworksParticle::Draw(const Camera& camera)
{
	for (auto& particle : particles_) {
		particle->BillboardMatrix(camera);
	}

	particleDrawer_->Draw(particles_, camera);

}
