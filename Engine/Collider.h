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
#include "PrimitiveDrawer.h"

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

	void CreateCollider(WorldTransform* worldTransform, Type type, RotationType rotationType, Camera* camera, const Vector3& size);

	void HitBox();

	// 衝突時に呼ばれる関数
	void OnCollision();

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

	Type GetType() { return type_; }
	Sphere* GetSphere() { return sphere_.get(); }
	Plane* GetPlane() { return plane_.get(); }
	AABB* GetAABB() { return aabb_.get(); }
	Capsule* GetCapsule() { return capsule_.get(); }
	OBB* GetOBB() { return obb_.get(); }

private:

	WorldTransform* worldTransform_;

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;

	// HitBox
	std::unique_ptr<Sphere> sphere_;
	std::unique_ptr<Plane> plane_;
	std::unique_ptr<AABB> aabb_;
	std::unique_ptr<Capsule> capsule_;
	std::unique_ptr<OBB> obb_;

	RotationType rotationType_;

	// HitBoxタイプ
	Type type_;
	Vector3 size_;
	WorldTransform worldTransform_HitBox_;
	std::unique_ptr<PrimitiveDrawer> HitBox_;
	
};
