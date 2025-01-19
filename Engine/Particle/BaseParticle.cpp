#include "BaseParticle.h"

#include "EngineTimeStep.h"

void yunity::BaseParticle::Update()
{
	for (std::list<Particle>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		(*particleIterator).currentTime += fixedTimeStep_;
		// 次のイテレーターへ
		++particleIterator;
	}
}

void yunity::BaseParticle::Update(std::list<Particle>* particles)
{
    if (!particles) {
        std::cerr << "Error: particles is nullptr!" << std::endl;
        return;
    }

    for (auto particleIterator = particles->begin(); particleIterator != particles->end();) {
        if (particleIterator->lifeTime <= particleIterator->currentTime) {
            std::cerr << "Removing particle with lifetime: " << particleIterator->lifeTime << std::endl;
            particleIterator = particles->erase(particleIterator);
        }
        else {
            particleIterator->currentTime += fixedTimeStep_;
            ++particleIterator;
        }
    }
}

void yunity::BaseParticle::Draw()
{
	particleDrawer_->Draw(particles_);
}
