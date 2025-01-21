#pragma once
#include "BaseParticle.h"
#include "ParticleDrawer.h"
#include "Random.h"

class PointParticle : public yunity::BaseParticle {
public:
	void Initialize(yunity::Camera* camera) override;
	void Spawn(const Vector3& position);
	void Update() override;
	void Draw() override;
private:
	Random::RandomNumberGenerator rng;
	std::list<Particle> circleParticles_;
	std::unique_ptr<yunity::ParticleDrawer> circleParticleDrawer_;
	std::unique_ptr<yunity::ParticleDrawer> ellipseParticleDrawer_;

	static const int particleQuantity_ = 2;
	const int spawnQuantity_ = 4;
	// 移動量
	float power_;
	// 移動方向の角度
	float directionDegree_;
	// 生存時間
	std::array<float, particleQuantity_> lifeTime_;
	// パーティクルの角度
	std::array<float, particleQuantity_> degree_;
};