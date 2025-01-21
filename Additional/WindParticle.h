#pragma once

#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class WindParticle : public yunity::BaseParticle {

public:
	void Initialize(yunity::Camera* camera);
	void Spawn(const Vector3& position) override;
	void Clear();
	void Update() override;
private:
	Random::RandomNumberGenerator rng;
	// 上昇量
	float ascent_;
	// 生存時間
	float lifeTime_;
	// パーティクルカラー
	Vector4 particleColor_;
	// パーティクルの最大数
	int maxParticle_;
	// パーティクルの最小位置
	Vector2 minTranslate_;
	// パーティクルの最大位置
	Vector2 maxTranslate_;
	// パーティクルのスケール
	Vector3 particleScale_;
};