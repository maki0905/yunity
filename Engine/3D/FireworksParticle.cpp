#include "FireworksParticle.h"

void FireworksParticle::Initialize(const Vector3& position, std::string textureName)
{
	isActive_ = true;
	particleDrawer_.reset(ParticleDrawer::Create(textureName));
	particleDrawer_->SetCamera(camera_);
	particles_.clear();

	for (uint32_t i = 0; i < 10; ++i) {
		/*Particle* particle = new Particle();
		particle->transform.translate = { i * 2.0f, i * 2.0f, 0.0f };
		particle->velocity = { rng.NextFloatRange(-0.1f, 0.1f), rng.NextFloatRange(-0.1f, 0.1f), 0.0f };
		particle->particleForCPU.color = { rng.NextFloatRange(0.0f, 1.0f),rng.NextFloatRange(0.0f, 1.0f),rng.NextFloatRange(0.0f, 1.0f), 1.0f };
		particle->lifeTime = 5.0f;
		particles_.push_back(particle);*/
		Particle* particle = new Particle();
		particle->transform.translate = position;
		/*particle->velocity = { 
			float(0.01f * cosf((36.0f * float(i)) * std::numbers::pi_v<float> / 180.0f)) - 0.01f * sinf((36.0f * float(i)) * std::numbers::pi_v<float> / 180.0f)), 
			float(0.01f * sinf((36.0f * float(i)) * std::numbers::pi_v<float> / 180.0f)) + 0.01f * cosf((36.0f * float(i)) * std::numbers::pi_v<float> / 180.0f)), 
			0.0f 
		};*/

		particle->velocity = { 
			float(0.05f * std::cosf(36.0f * i * std::numbers::pi_v<float> / 180.0f) - 0.05f * std::sinf(36.0f * i * std::numbers::pi_v<float> / 180.0f)),
			float(0.05f * std::sinf(36.0f * i * std::numbers::pi_v<float> / 180.0f) + 0.05f * std::cosf(36.0f * i * std::numbers::pi_v<float> / 180.0f)),
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
		particle->particleForCPU.color.w = 1.0f - (particle->currentTime / particle->lifeTime);
		particle->BillboardMatrix(*camera_);
	}

}

void FireworksParticle::Draw()
{
	particleDrawer_->Draw(particles_);
}

