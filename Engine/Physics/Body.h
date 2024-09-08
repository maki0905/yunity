#pragma once

#include "Collider.h"
#include "World.h"
#include "WorldTransform.h"

class Body : public Collider {
public:


	enum class FrictionCombine {
		kNone,    // 計算しない
		kAverage, // 平均化
		kMinimum, // 小さい方 
		kMaximum, // 大きい方
		kMultiply // 乗算
	};

	enum class BounceCombine {
		kNone,    // 計算しない
		kAverage, // 平均化
		kMinimum, // 小さい方 
		kMaximum, // 大きい方
		kMultiply // 乗算
	};
public:
	void CreateBody(World* world, WorldTransform* worldTransform, float mass = 0.0f);
	void Solve(float time);

	Vector3 RubberMovement(const Vector3& start, const Vector3& end, float limitLength, float stiffness, float dampingCoefficient);
	Vector3 Spring(const Vector3& anchor, const Vector3& position, float naturalLength, float stiffness, float dampingCoefficient);

	void AddForce(const Vector3& force, uint32_t mode);
	void AddTorque(const Vector3& torque, uint32_t mode);

	void Reset();

	void OnCollision(Body* body);

	// 衝突時に呼ばれる固有の処理
	/*virtual void OnCollisionEvent(Body* body = nullptr) = 0;
	virtual void OnTriggerEvent(Body* body = nullptr) = 0;*/

	World* GetWorld() { return world_; }

	Vector3 GetVelocity() { return velocity_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	Vector3 GetAngularVelocity() { return angularVelocity_; }
	void SetAngularVelocity(const Vector3& angularVelocity) { angularVelocity_ = angularVelocity; }

	float GetMass() { return mass_; }
	void SetMass(float mass) { mass_ = mass; }

	float GetInertiaTensor() { return inertiaTensor_; }
	void SetInertiaTensor(float inertiaTensor) { inertiaTensor_ = inertiaTensor; }

	float GetDrag() { return drag_; }
	void SetDrag(float drag) { drag_ = drag; }
	float GetAngularDrag() { return angularDrag_; }
	void SetAngularDrag(float angularDrag) { angularDrag_ = angularDrag; }

	void SetGravity(const Vector3& gravity) { gravityAcceleration_ = gravity; }

	WorldTransform* GetWorldTransform() { return worldTransform_; };
	Vector3 GetTranslation() { return worldTransform_->translation_; }
	Vector3 GetMatWorldTranslation() { return { worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2] }; }


	Vector3 GetVertical() { return vertical_; }

	bool GetIsTrigger() { return isTrigger_; }
	void SetIsTrigger(bool isTrigger) { isTrigger_ = isTrigger; }

	float GetMiu() { return miu_; }
	void SetMiu(float miu) { miu_ = miu; }
	void SetFirictionCombine(FrictionCombine frictionCombine) { frictionCombine_ = frictionCombine; }

	float GetBounciness() { return bounciness_; }
	void SetBounciness(float bounciness) { bounciness_ = bounciness; }
	void SetBounceCombine(BounceCombine bounceCombine) { bounceCombine_ = bounceCombine; }

	Vector3 GetNormalVector() { return normalVector_; }

private:

	World* world_;
	WorldTransform* worldTransform_;
	// 速度
	Vector3 velocity_;
	// 角速度
	Vector3 angularVelocity_;
	// 加速度
	Vector3 acceleration_;
	Vector3 angularAcceleration_;
	// 衝撃
	Vector3 impulse_;
	// 力
	Vector3 force_;
	Vector3 torque_;
	// 質量
	float mass_;
	// 慣性テンソル
	float inertiaTensor_;
	// 空気抵抗
	float drag_ = 0.0005f;
	float angularDrag_ = 0.005f;
	// 重力加速度
	Vector3 gravityAcceleration_;
	// 摩擦計算方法
	FrictionCombine frictionCombine_;
	// 摩擦係数
	float miu_ = 0.0f;
	// 動摩擦力
	float magnitude_ = 0.0f;
	// 反発計算方法
	BounceCombine bounceCombine_;
	// 反発係数
	float bounciness_ = 0.0f;
	// 法線
	Vector3 normalVector_;
	
	// 垂直抗力
	Vector3 vertical_;
	bool isTrigger_;
};