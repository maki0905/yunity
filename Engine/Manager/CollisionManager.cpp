#include "CollisionManager.h"

#include "MathFunction.h"
#include "Collision.h"
#include "CollisionConfig.h"
#include "ImGuiManager.h"
#include "Object3D.h"
#include "Shape.h"

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
	if (colliderA->GetCollisionAttribute() == colliderB->GetCollisionAttribute()) {
		return;
	}

	switch (colliderA->GetShape())
	{
	case Collider::Shape::kSphere:
		switch (colliderB->GetShape())
		{
		case Collider::Shape::kSphere:

			break;
		case Collider::Shape::kAABB:
			if (IsCollision(
				Sphere(colliderA->GetColliderCenter(), colliderA->GetHitBoxSize().x * 2.0f),
				AABB(Subtract(colliderB->GetColliderCenter(), colliderB->GetHitBoxSize()), Add(colliderB->GetColliderCenter(), colliderB->GetHitBoxSize())))) {
				colliderA->SetHitBody(colliderB);
				colliderB->SetHitBody(colliderA);
				if (colliderA->GetIsTrigger() || colliderB->GetIsTrigger()) {
					colliderA->OnTriggerEvent();
					colliderB->OnTriggerEvent();
				}
				else {
					colliderA->OnCollision(colliderB);
					colliderB->OnCollision(colliderA);
					colliderA->OnCollisionEvent();
					colliderB->OnCollisionEvent();
				}
			}

			break;
		}
		break;
	case Collider::Shape::kAABB:
		switch (colliderB->GetShape())
		{
		case Collider::Shape::kSphere:
			if (IsCollision(
				AABB{ Subtract(colliderA->GetColliderCenter(), colliderA->GetHitBoxSize()), Add(colliderA->GetColliderCenter(), colliderA->GetHitBoxSize()) },
				Sphere{ colliderB->GetColliderCenter(), colliderB->GetHitBoxSize().x * 2.0f })) {
				colliderA->SetHitBody(colliderB);
				colliderB->SetHitBody(colliderA);
				if (colliderA->GetIsTrigger() || colliderB->GetIsTrigger()) {
					colliderA->OnTriggerEvent();
					colliderB->OnTriggerEvent();
				}
				else {
					if (colliderA->GetCollisionAttribute() == kCollisionAttributePlayer && colliderB->GetCollisionAttribute() == kCollisionAttributeCoin) {
						colliderA->OnTriggerEvent();
						colliderB->OnTriggerEvent();
					}
					else {
						colliderA->OnCollision(colliderB);
						colliderB->OnCollision(colliderA);
						colliderA->OnCollisionEvent();
						colliderB->OnCollisionEvent();
					}
				}
			}

			break;
		case Collider::Shape::kAABB:
			if (IsCollision(
				AABB(Subtract(colliderA->GetColliderCenter(), colliderA->GetHitBoxSize()), Add(colliderA->GetColliderCenter(), colliderA->GetHitBoxSize())),
				AABB(Subtract(colliderB->GetColliderCenter(), colliderB->GetHitBoxSize()), Add(colliderB->GetColliderCenter(), colliderB->GetHitBoxSize())))) {
				colliderA->SetHitBody(colliderB);
				colliderB->SetHitBody(colliderA);
				if (colliderA->GetIsTrigger() || colliderB->GetIsTrigger()) {
					colliderA->OnTriggerEvent();
					colliderB->OnTriggerEvent();
				}
				else {
					colliderA->OnCollision(colliderB);
					colliderB->OnCollision(colliderA);
					colliderA->OnCollisionEvent();
					colliderB->OnCollisionEvent();
				}
			}

			break;
		case Collider::Shape::kOBB:
			if (IsCollision(CreateOBB(colliderA->GetColliderCenter(), colliderA->GetWorldTransform()->rotation_, colliderA->GetHitBoxSize()),CreateOBB(colliderB->GetColliderCenter(), colliderB->GetWorldTransform()->rotation_, colliderB->GetHitBoxSize()))) {
				colliderA->SetHitBody(colliderB);
				colliderB->SetHitBody(colliderA);
				if (colliderA->GetIsTrigger() || colliderB->GetIsTrigger()) {
					colliderA->OnTriggerEvent();
					colliderB->OnTriggerEvent();
				}
				else {
					colliderA->OnCollision(colliderB);
					colliderB->OnCollision(colliderA);
					colliderA->OnCollisionEvent();
					colliderB->OnCollisionEvent();
				}
			}
			break;
		}
		break;
	case Collider::Shape::kOBB:
		switch (colliderB->GetShape())
		{
		case Collider::Shape::kAABB:
			if (IsCollision(CreateOBB(colliderA->GetColliderCenter(), colliderA->GetWorldTransform()->rotation_, colliderA->GetHitBoxSize()), CreateOBB(colliderB->GetColliderCenter(), colliderB->GetWorldTransform()->rotation_, colliderB->GetHitBoxSize()))) {
				colliderA->SetHitBody(colliderB);
				colliderB->SetHitBody(colliderA);
				if (colliderA->GetIsTrigger() || colliderB->GetIsTrigger()) {
					colliderA->OnTriggerEvent();
					colliderB->OnTriggerEvent();
				}
				else {
					colliderA->OnCollision(colliderB);
					colliderB->OnCollision(colliderA);
					colliderA->OnCollisionEvent();
					colliderB->OnCollisionEvent();
				}
			}
			break;

		case Collider::Shape::kOBB:
			if (IsCollision(CreateOBB(colliderA->GetColliderCenter(), colliderA->GetWorldTransform()->rotation_, colliderA->GetHitBoxSize()), CreateOBB(colliderB->GetColliderCenter(), colliderB->GetWorldTransform()->rotation_, colliderB->GetHitBoxSize()))) {
				colliderA->SetHitBody(colliderB);
				colliderB->SetHitBody(colliderA);
				if (colliderA->GetIsTrigger() || colliderB->GetIsTrigger()) {
					colliderA->OnTriggerEvent();
					colliderB->OnTriggerEvent();
				}
				else {
					colliderA->OnCollision(colliderB);
					colliderB->OnCollision(colliderA);
					colliderA->OnCollisionEvent();
					colliderB->OnCollisionEvent();
				}
			}
			break;
		}
		break;
	}
}


