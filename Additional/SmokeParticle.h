#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class SmokeParticle : public yunity::BaseParticle {
	struct smoke : public Particle {
		bool flag;
	};

public:
	void Initialize(yunity::Camera* camera);
	void Spawn(const Vector3& position) override;
	void Update() override;
private:
	Random::RandomNumberGenerator rng;
	// 進行時間
	const float fixedTime_ = 1.0f / 30.0f;
	// 発生間隔
	const float spawnInterval_ = 0.15f;
	// 上昇量
	const float ascent_ = 0.05f;
	// 生存時間
	const float lifeTime_ = 2.0f;

	float frequencyTime_;
	float spawnTime_;
};