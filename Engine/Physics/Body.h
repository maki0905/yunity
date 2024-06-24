#pragma once

#include "Collider.h"
#include "World.h"
#include "WorldTransform.h"

class Body : public Collider {
public:
	void CreateBody(World* world, WorldTransform* worldTransform, float mass);
	void Solve();

	Vector3 RubberMovement(const Vector3& start, const Vector3& end, float limitLength, float stiffness, float dampingCoefficient);

	void AddForce(const Vector3& force, uint32_t mode);

	void Reset();

	Vector3 GetVelocity() { return velocity_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	float GetMass() { return mass_; }
	void SetMass(float mass) { mass_ = mass; }

	void SetDrag(float drag) { drag_ = drag; }

	void SetGravity(const Vector3& gravity) { gravityAcceleration_ = gravity; }

	WorldTransform GetWorldTransform() { return *worldTransform_; };

private:

	World* world_;
	WorldTransform* worldTransform_;
	// 速度
	Vector3 velocity_;
	// 加速度
	Vector3 acceleration_;
	// 衝撃
	Vector3 impulse_;
	// 力
	Vector3 force_;
	// 質量
	float mass_;
	// 空気抵抗
	float drag_ = 0.05f;
	// 重力加速度
	Vector3 gravityAcceleration_;
	// 摩擦係数
	float miu_ = 0.5f;
};