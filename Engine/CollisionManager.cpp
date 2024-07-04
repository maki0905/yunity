#include "CollisionManager.h"

#include "MathFunction.h"
#include "Collision.h"
#include "CollisionConfig.h"
#include "ImGuiManager.h"
#include "Object3D.h"


void CollisionManager::CheckAllCollision() {

	// リスト内のペアを総当たり
	std::list<Object3D*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Object3D* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Object3D*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Object3D* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Object3D* colliderA, Object3D* colliderB)
{
	/*if (colliderA->GetMass() == 0 && colliderB->GetMass() == 0) {
		return;
	}*/

	if (colliderA->GetCollisionAttribute() == colliderB->GetCollisionAttribute()) {
		return;
	}

	switch (colliderA->GetShape())
	{
	case Collider::Shape::kSphere:
		Sphere sphereA;
		sphereA.center = colliderA->GetWorldTransform().translation_;
		sphereA.radius = colliderA->GetHitBoxSize().x;
		switch (colliderB->GetShape())
		{
		case Collider::Shape::kAABB:
			/*if (IsCollision(
				Sphere(colliderA->GetWorldTransform().translation_, colliderA->GetHitBoxSize().x),
				AABB(Subtract(colliderB->GetWorldTransform().translation_, colliderB->GetHitBoxSize()), Add(colliderB->GetWorldTransform().translation_, colliderB->GetHitBoxSize())))) {
				colliderA->OnCollision();
				colliderB->OnCollision();
			}*/

			break;
		default:
			break;
		}
		break;
	case Collider::Shape::kAABB:
		switch (colliderB->GetShape())
		{
		case Collider::Shape::kAABB:
			if (IsCollision(
				AABB(Subtract(colliderA->GetWorldTransform().translation_, colliderA->GetHitBoxSize()), Add(colliderA->GetWorldTransform().translation_, colliderA->GetHitBoxSize())),
				AABB(Subtract(colliderB->GetWorldTransform().translation_, colliderB->GetHitBoxSize()), Add(colliderB->GetWorldTransform().translation_, colliderB->GetHitBoxSize())))) {

				if (colliderA->GetIsTrigger() || colliderB->GetIsTrigger()) {
					colliderA->OnTriggerEvent(colliderB);
					colliderB->OnTriggerEvent(colliderA);
				}
				else {
					colliderA->OnCollision(colliderB);
					colliderB->OnCollision(colliderA);
					colliderA->OnCollisionEvent(colliderB);
					colliderB->OnCollisionEvent(colliderA);
				}
			}

			break;
		default:
			break;
		}

		break;
	default:
		break;
	}
}

//void CollisionManager::CheckCollisionPair(Body* colliderA, Body* colliderB) {
//
//	/*if (colliderA->GetMass() == 0 && colliderB->GetMass() == 0) {
//		return;
//	}*/
//
//	if (colliderA->GetCollisionAttribute() == colliderB->GetCollisionAttribute()) {
//		return;
//	}
//
//	switch (colliderA->GetShape())
//	{
//	case Collider::Shape::kSphere:
//		Sphere sphereA;
//		sphereA.center = colliderA->GetWorldTransform().translation_;
//		sphereA.radius = colliderA->GetHitBoxSize().x;
//		switch (colliderB->GetShape())
//		{
//		case Collider::Shape::kAABB:
//			/*if (IsCollision(
//				Sphere(colliderA->GetWorldTransform().translation_, colliderA->GetHitBoxSize().x),
//				AABB(Subtract(colliderB->GetWorldTransform().translation_, colliderB->GetHitBoxSize()), Add(colliderB->GetWorldTransform().translation_, colliderB->GetHitBoxSize())))) {
//				colliderA->OnCollision();
//				colliderB->OnCollision();
//			}*/
//
//			break;
//		default:
//			break;
//		}
//		break;
//	case Collider::Shape::kAABB:
//		switch (colliderB->GetShape())
//		{
//		case Collider::Shape::kAABB:
//			if (IsCollision(
//				AABB(Subtract(colliderA->GetWorldTransform().translation_, colliderA->GetHitBoxSize()), Add(colliderA->GetWorldTransform().translation_, colliderA->GetHitBoxSize())),
//				AABB(Subtract(colliderB->GetWorldTransform().translation_, colliderB->GetHitBoxSize()), Add(colliderB->GetWorldTransform().translation_, colliderB->GetHitBoxSize())))) {
//
//				if (colliderA->GetIsTrigger() || colliderB->GetIsTrigger()) {
//					colliderA->OnTriggerEvent(colliderB);
//					colliderB->OnTriggerEvent(colliderA);
//				}
//				else {
//					colliderA->OnCollision(colliderB);
//					colliderB->OnCollision(colliderA);
//					colliderA->OnCollisionEvent(colliderB);
//					colliderB->OnCollisionEvent(colliderA);
//				}
//			}
//
//			break;
//		default:
//			break;
//		}
//
//		break;
//	default:
//		break;
//	}
//
//	//switch (colliderB->GetType())
//	//{
//	//case Collider::kSphere:
//
//	//	break;
//	//case Collider::kPlane:
//	//	break;
//	//case Collider::kAABB:
//	//	if (IsCollision(*colliderA->GetAABB(), *colliderB->GetAABB())) {
//	//		colliderA->OnCollision();
//	//		/*if (colliderA->GetMass() != 0) {
//	//			colliderA->resolveCollision(*colliderB->GetAABB());
//	//		}
//	//		else {
//	//			colliderB->resolveCollision(*colliderA->GetAABB());
//	//		}*/
//	//		//colliderA->resolveCollision(*colliderB->GetAABB());
//	//		colliderB->OnCollision();
//	//		ImGui::Text("TURE");
//
//	//	}
//	//	else {
//	//		ImGui::Text("FALSE");
//	//	}
//	//	break;
//	//case Collider::kCapsule:
//	//	break;
//	//case Collider::kOBB:
//	//	break;
//	//default:
//	//	break;
//	//}
//	
//
//
//	// 衝突フィルタリング
//	/*if (colliderA->GetCollisionAttribute() != kCollisionAttributePlayer) {
//		return;
//	}
//	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
//		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
//		return;
//	}*/
//
//	/*sphere[0] = *colliderA->GetSphere();
//	sphere[1] = *colliderB->GetSphere();
//
//	if (IsCollision(sphere[0], sphere[1])) {
//		colliderA->OnCollision();
//		colliderB->OnCollision();
//	}*/
//
//
//
//	/*Sphere sphereA = *colliderA->GetSphere();
//	Capsule capsuleA = *colliderA->GetCapsule();
//	Sphere sphereB{};
//	Plane planeB{};
//	AABB aabbB{};
//	OBB obbA = *colliderA->GetOBB();*/
//
//	//uint32_t checker = 0;
//	//if (colliderB->GetCollisionAttribute() == kCollisionAttributeEnemy) {
//	//	sphereB = *colliderB->GetSphere();
//	//}
//	//else if (
//	//	colliderB->GetCollisionAttribute() == kCollisionAttributeFloor || 
//	//	colliderB->GetCollisionAttribute() == kCollisionAttributeMovingFloor ||
//	//	colliderB->GetCollisionAttribute() == kCollisionAttributeGoal
//	//	){
//	//	aabbB = *colliderB->GetAABB();
//	//	checker = 1;
//	//}
//
//	//if (sphereB.radius != 0.0f) {
//	//	if (IsCollision(sphereA, sphereB)) {
//	//		colliderA->OnCollision(colliderB->GetCollisionAttribute());
//	//		//colliderB->OnCollision(colliderA->GetCollisionAttribute());
//	//	}
//
//	//	if (IsCollision(capsuleA, sphereB)) {
//	//		colliderA->OnCollision(colliderB->GetCollisionAttribute());
//	//	}
//
//	//	if (IsCollision(obbA, sphereB)) {
//	//		colliderB->OnCollision(colliderA->GetCollisionAttribute());
//	//	}
//	//}
//	//else if (checker == 1) {
//	//	if (IsCollision(aabbB, sphereA)) {
//
//	//		if (colliderB->GetCollisionAttribute() == kCollisionAttributeMovingFloor) {
//	//			colliderA->OnCollision(colliderB->GetWorldTransform());
//	//			colliderB->OnCollision(colliderA->GetCollisionAttribute());
//	//		}
//	//		else {
//	//			colliderA->OnCollision(colliderB->GetCollisionAttribute());
//	//			colliderB->OnCollision(colliderA->GetCollisionAttribute());
//	//		}
//	//	}
//	//}
//
//	//// 球と球の交差判定
//	//if (sqrtf(distance.x * distance.x) + sqrtf(distance.y * distance.y) +
//	//	sqrtf(distance.z * distance.z) <=
//	//	colliderA->GetRadius() + colliderB->GetRadius()) {
//	//	// コライダーAの衝突時コールバックを呼び出す
//	//	colliderA->OnCollision();
//	//	// コライダーBの衝突時コールバックを呼び出す
//	//	colliderB->OnCollision();
//	//}
//}

//void CollisionManager::Discrimination(HitBoxData dataA, HitBoxData dataB)
//{
//	switch (dataA.shape)
//	{
//	case Collider::Shape::kAABB:
//
//
//		break;
//	default:
//		break;
//	}
//
//}
