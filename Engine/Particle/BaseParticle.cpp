#include "BaseParticle.h"

void yunity::BaseParticle::Update()
{
	for (std::list<Particle>::iterator particleIterator = particles_.begin(); particleIterator != particles_.end();) {
		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
			particleIterator = particles_.erase(particleIterator);
			continue;
		}
		(*particleIterator).currentTime += fixedTime_;
		// 次のイテレーターへ
		++particleIterator;
	}
}

//std::list<Particle> yunity::BaseParticle::Update(std::list<Particle> particles)
//{
//	for (std::list<Particle>::iterator particleIterator = particles.begin(); particleIterator != particles.end();) {
//		if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
//			particleIterator = particles_.erase(particleIterator);
//			continue;
//		}
//		(*particleIterator).currentTime += fixedTime_;
//		// 次のイテレーターへ
//		++particleIterator;
//	}
//	return particles;
//}

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
            particleIterator->currentTime += fixedTime_;
            ++particleIterator;
        }
    }
}

void yunity::BaseParticle::Draw()
{
	particleDrawer_->Draw(particles_);
}
