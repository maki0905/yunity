#pragma once
#pragma once
#include "MathFunction.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	// 半径を取得
	float GetRadius() { return radius_; }
	// 半径を設定
	void SetRadius(float radius) { radius_ = radius; }
	// 衝突時に呼ばれる関数
	virtual void OnCollision() = 0;
	// ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

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

private:
	// 衝突半径
	float radius_ = 2.0f;
	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t CollisionMask_ = 0xffffffff;
};
