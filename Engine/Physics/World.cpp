#include "World.h"

void World::Initialize()
{
	collisionManager_ = std::make_unique<CollisionManager>();
	allocator_.clear();

}

void World::Solve()
{
	collisionManager_->ClearCollider();
	for (auto& collider : allocator_) {
		collisionManager_->SetCollider(collider);
	}

	collisionManager_->CheckAllCollision();

	for (auto& obj : allocator_) {
		obj->Solve();
	}

	allocator_.clear();
}
