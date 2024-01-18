#pragma once

#include <memory>

#include "MathFunction.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"
#include "Capsule.h"
#include "OBB.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {

public:
	enum Type
	{
		kSphere,
		kPlane,
		kAABB,
		kCapsule,
		kOBB,
	};

public:

	void Create(WorldTransform* worldTransform, Type type);

	void Step(RotationType rotationType);

	// 衝突時に呼ばれる関数
	void OnCollision();
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突属性(自分)を取得
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	// 衝突属性(自分)を設定
	void SetCollisionAttribute(uint32_t collisionAttribute) {
		collisionAttribute_ = collisionAttribute;
	}
	// 衝突属性(自分)を取得
	uint32_t GetCollisionMask() { return CollisionMask_; }
	// 衝突属性(自分)を設定
	void SetCollisionMask(uint32_t collisionMask) { CollisionMask_ = collisionMask; }

	Sphere* GetSphere() { return &sphere_; }
	Plane* GetPlane() { return &plane_; }
	AABB* GetAABB() { return &aabb_; }
	Capsule* GetCapsule() { return &capsule_; }
	OBB* GetOBB() { return &obb_; }

	void SetVelocity(const Vector3& velocity);

private:
	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;

	// HitBox
	/*std::unique_ptr<Sphere> sphere_;
	std::unique_ptr<Plane> sphere_;
	std::unique_ptr<AABB> sphere_;
	std::unique_ptr<Sphere> sphere_;
	std::unique_ptr<Capsule> sphere_;
	std::unique_ptr<OBB> sphere_;*/

	Sphere sphere_;
	Plane plane_;
	AABB aabb_;
	Capsule capsule_;
	OBB obb_;

	float mass_ = 1.0f;
	float gravityScale_ = 0.1f;
	float miu_ = 0.5f;




	Vector3 velocity_;
	Vector3 acceleration_;

	WorldTransform* worldTransform_;

	// HitBoxタイプ
	Type type_;
	
};
