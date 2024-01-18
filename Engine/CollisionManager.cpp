#include "CollisionManager.h"

#include "MathFunction.h"
#include "Collider.h"
#include "Collision.h"
#include "CollisionConfig.h"


void CollisionManager::CheckAllCollision() {

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// 衝突フィルタリング
	if (colliderA->GetCollisionAttribute() != kCollisionAttributePlayer) {
		return;
	}
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}

	sphere[0] = *colliderA->GetSphere();
	sphere[1] = *colliderB->GetSphere();

	if (IsCollision(sphere[0], sphere[1])) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}



	/*Sphere sphereA = *colliderA->GetSphere();
	Capsule capsuleA = *colliderA->GetCapsule();
	Sphere sphereB{};
	Plane planeB{};
	AABB aabbB{};
	OBB obbA = *colliderA->GetOBB();*/

	//uint32_t checker = 0;
	//if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy) {
	//	sphereB = *colliderB->GetSphere();
	//}
	//else if (
	//	colliderB->GetCollisionAttribute() == kCollisionAttributeFloor || 
	//	colliderB->GetCollisionAttribute() == kCollisionAttributeMovingFloor ||
	//	colliderB->GetCollisionAttribute() == kCollisionAttributeGoal
	//	){
	//	aabbB = *colliderB->GetAABB();
	//	checker = 1;
	//}

	//if (sphereB.radius != 0.0f) {
	//	if (IsCollision(sphereA, sphereB)) {
	//		colliderA->OnCollision(colliderB->GetCollisionAttribute());
	//		//colliderB->OnCollision(colliderA->GetCollisionAttribute());
	//	}

	//	if (IsCollision(capsuleA, sphereB)) {
	//		colliderA->OnCollision(colliderB->GetCollisionAttribute());
	//	}

	//	if (IsCollision(obbA, sphereB)) {
	//		colliderB->OnCollision(colliderA->GetCollisionAttribute());
	//	}
	//}
	//else if (checker == 1) {
	//	if (IsCollision(aabbB, sphereA)) {

	//		if (colliderB->GetCollisionAttribute() == kCollisionAttributeMovingFloor) {
	//			colliderA->OnCollision(colliderB->GetWorldTransform());
	//			colliderB->OnCollision(colliderA->GetCollisionAttribute());
	//		}
	//		else {
	//			colliderA->OnCollision(colliderB->GetCollisionAttribute());
	//			colliderB->OnCollision(colliderA->GetCollisionAttribute());
	//		}
	//	}
	//}

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