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
	enum Shape
	{
		kSphere,
		kPlane,
		kAABB,
		kCapsule,
		kOBB,
	};

public:
	/// <summary>
	/// コライダー生成
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="shape"></param>
	/// <param name="camera"></param>
	void CreateCollider(WorldTransform* worldTransform, Shape shape, Camera* camera);
	
	/// <summary>
	/// コライダー生成
	/// </summary>
	/// <param name="worldTransform"></param>
	/// <param name="shape"></param>
	/// <param name="camera"></param>
	/// <param name="size"></param>
	void CreateCollider(WorldTransform* worldTransform, Shape shape, Camera* camera, const Vector3& size);
	
	/// <summary>
	/// ヒットボックス
	/// </summary>
	void HitBox();
	
	/// <summary>
	/// ヒットボックス
	/// </summary>
	/// <param name="camera"></param>
	void HitBox(Camera* camera);

	WorldTransform GetWorldTransform() { return *worldTransform_; }

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

	/// <summary>
	/// getter
	/// </summary>
	/// <returns></returns>

	Shape GetShape() { return shape_; }
	Vector3 GetHitBoxSize();
	Sphere* GetSphere() { return sphere_.get(); }
	Plane* GetPlane() { return plane_.get(); }
	AABB GetAABB();
	Capsule* GetCapsule() { return capsule_.get(); }
	OBB* GetOBB() { return obb_.get(); }
	Vector3 GetSize() { return size_; }
	Vector3 GetCenter() { return center_; }
	Vector3 GetColliderCenter() { return Add(worldTransform_->GetMatWorldTranslation(), Multiply(worldTransform_->scale_, center_)); }

	/// <summary>
	/// setter
	/// </summary>
	/// <returns></returns>

	void SetShape(Shape shape) { shape_ = shape; }
	void SetHitBoxSize(Vector3 size) { size_ = size; }
	void SetCenter(const Vector3& center) { center_ = center; }
	

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
	Shape shape_;
	Vector3 center_;
	Vector3 size_;
	WorldTransform worldTransform_HitBox_;
	std::unique_ptr<PrimitiveDrawer> HitBox_;
	
};
