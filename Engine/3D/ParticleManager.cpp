#include "ParticleManager.h"

void ParticleManager::Initialize()
{
	particles_.clear();
}

void ParticleManager::Update()
{
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

void ParticleManager::Draw(const Camera& camera)
{
	for (auto& particle : particles_) {
		particle->Draw(camera);
	}
}

void ParticleManager::Add(Camera* camera)
{
	FireworksParticle* fireworksParticle = new FireworksParticle();
	fireworksParticle->Initialize();
	fireworksParticle->GetCamera(camera);
	particles_.push_back(fireworksParticle);
}
