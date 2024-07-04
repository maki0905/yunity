#include "World.h"

#include "Object3D.h"

void World::Initialize(const Vector3& gravity)
{
	collisionManager_ = std::make_unique<CollisionManager>();
	allocator_.clear();
	gravity_ = gravity;
}

void World::Solve()
{
	for (auto& obj : allocator_) {
		obj->Solve();
	}

	collisionManager_->ClearCollider();
	for (auto& collider : allocator_) {
		collisionManager_->SetCollider(collider);
	}

	collisionManager_->CheckAllCollision();


}
