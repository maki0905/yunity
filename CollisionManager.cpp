#include "CollisionManager.h"
#include "MathFunction.h"
#include "BaseObject.h"
#include "Collision.h"
#include "CollisionConfig.h"

void CollisionManager::CheckAllCollision() {

	// リスト内のペアを総当たり
	std::list<BaseObject*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		BaseObject* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<BaseObject*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			BaseObject* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(BaseObject* colliderA, BaseObject* colliderB) {
	// 衝突フィルタリング
	if (colliderA->GetCollisionAttribute() != kCollisionAttributePlayer) {
		return;
	}
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}
	Sphere sphereA = *colliderA->GetSphere();
	Sphere sphereB{};
	Plane planeB{};
	AABB aabbB{};

	uint32_t checker = 0;
	if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy) {
		sphereB = *colliderB->GetSphere();
	}
	else if (
		colliderB->GetCollisionAttribute() == kCollisionAttributeFloor || 
		colliderB->GetCollisionAttribute() == kCollisionAttributeMovingFloor ||
		colliderB->GetCollisionAttribute() == kCollisionAttributeGoal
		){
		aabbB = *colliderB->GetAABB();
		checker = 1;
	}

	if (sphereB.radius != 0.0f) {
		if (IsCollision(sphereA, sphereB)) {
			colliderA->OnCollision(colliderB->GetCollisionAttribute());
			colliderB->OnCollision(colliderA->GetCollisionAttribute());
		}

	}
	else if (checker == 1) {
		if (IsCollision(aabbB, sphereA)) {

			if (colliderB->GetCollisionAttribute() == kCollisionAttributeMovingFloor) {
				colliderA->OnCollision(colliderB->GetWorldTransform());
				colliderB->OnCollision(colliderA->GetCollisionAttribute());
			}
			else {
				colliderA->OnCollision(colliderB->GetCollisionAttribute());
				colliderB->OnCollision(colliderA->GetCollisionAttribute());
			}
		}
	}

	//// 球と球の交差判定
	//if (sqrtf(distance.x * distance.x) + sqrtf(distance.y * distance.y) +
	//	sqrtf(distance.z * distance.z) <=
	//	colliderA->GetRadius() + colliderB->GetRadius()) {
	//	// コライダーAの衝突時コールバックを呼び出す
	//	colliderA->OnCollision();
	//	// コライダーBの衝突時コールバックを呼び出す
	//	colliderB->OnCollision();
	//}
}