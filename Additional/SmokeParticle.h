#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class SmokeParticle : public yunity::BaseParticle {
public:
	void Initialize(yunity::Camera* camera);
	void Spawn(const Vector3& position) override;
	void Update() override;
private:
	Random::RandomNumberGenerator rng;
	// 発生間隔
	float spawnInterval_ = 0.05f;
	// 上昇量
	float ascent_ = 0.05f;
	// 生存時間
	float lifeTime_ = 1.0f;
	// 最小スケール
	float minScale_;
	// 最大スケール
	float maxScale_;
	// 最小位置
	float minTranslate_;
	// 最大位置
	float maxTranslate_;
	// 生成時間
	float spawnTime_;
};