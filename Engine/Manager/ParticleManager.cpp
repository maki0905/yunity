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

		}
	}

	for (std::list<std::unique_ptr<yunity::BaseParticle>>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if (!(*particleIterator)->GetIsActive()) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		(*particleIterator)->Update();
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
