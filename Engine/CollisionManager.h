#pragma once
#include <list>

#include "Sphere.h"

class Collider;
//class BaseObject;

class CollisionManager {
public:
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();
	// コライダーを取得
	void SetCollider(Collider* collider) { colliders_.push_back(collider); }
	// 衝突属性(自分)を設定
	void ClearCollider() { colliders_.clear(); }

private:
	// コライダーリスト
	std::list<Collider*> colliders_;
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	Sphere sphere[2];
	/*Capsule capsule[2];
	Plane plane[2];
	AABB aabb[2];
	OBB obb[2];*/

};