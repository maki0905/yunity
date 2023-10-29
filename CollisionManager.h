#pragma once
#include <list>

//class Collider;
class BaseObject;

class CollisionManager {
public:
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();
	// コライダーを取得
	void SetCollider(BaseObject* collider) { colliders_.push_back(collider); }
	// 衝突属性(自分)を設定
	void ClearCollider() { colliders_.clear(); }

private:
	// コライダーリスト
	std::list<BaseObject*> colliders_;
	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(BaseObject* colliderA, BaseObject* colliderB);
};