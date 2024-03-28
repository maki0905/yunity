#include "ParticleManager.h"

void ParticleManager::Initialize()
{
	emitters_.clear();
	particles_.clear();
}

void ParticleManager::Update()
{
	for (auto& emitter : emitters_) {
		if (emitter->Update()) {

			FireworksParticle* fireworksParticle = new FireworksParticle();
			fireworksParticle->SetCamera(camera_);
			fireworksParticle->Initialize(emitter->GetPosition(), emitter->GetTextureName());
			particles_.push_back(fireworksParticle);
		}
	}

	for (std::list<BaseParticle*>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if (!(*particleIterator)->isActive_) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		(*particleIterator)->Updata();
		// 次のイテレーターへ
		++particleIterator;
	}
}

void ParticleManager::Draw()
{
	for (auto& particle : particles_) {
		particle->Draw();
	}
	for (auto& emitter : emitters_) {
		emitter->Draw();
	}
}

void ParticleManager::AddEmitter(ParticleEmitter* emitter)
{
	emitters_.push_back(emitter);
}

void ParticleManager::Add(Camera* camera)
{
	/*FireworksParticle* fireworksParticle = new FireworksParticle();
	fireworksParticle->SetCamera(camera);
	fireworksParticle->Initialize();
	particles_.push_back(fireworksParticle);*/
}
