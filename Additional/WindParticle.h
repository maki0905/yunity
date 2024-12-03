#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class WindParticle : public yunity::BaseParticle {

public:
	void Initialize(yunity::Camera* camera);
	void Spawn(const Vector3& position) override;
	void Update() override;
private:
	Random::RandomNumberGenerator rng;
	// 上昇量
	const float ascent_ = 0.1f;
	// 生存時間
	const float lifeTime_ = 2.0f;
};