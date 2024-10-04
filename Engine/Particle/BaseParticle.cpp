#include "BaseParticle.h"

void BaseParticle::Updata()
{
	// 固有処理
	Peculiar();

	for (std::list<Particle*>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if ((*particleIterator)->lifeTime <= (*particleIterator)->currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		(*particleIterator)->currentTime += 1.0f / 60.0f;
		// 次のイテレーターへ
		++particleIterator;
	}
}

