#include "BaseParticle.h"

void BaseParticle::Update()
{
	for (std::list<Particle>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		(*particleIterator).currentTime += 1.0f / 60.0f;
		// 次のイテレーターへ
		++particleIterator;
	}
}

void BaseParticle::Draw()
{
	particleDrawer_->Draw(particles_);
}
