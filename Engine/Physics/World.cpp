#include "World.h"

#include "Object3D.h"
#include "DirectXCore.h"

void World::Initialize(const Vector3& gravity)
{
	collisionManager_ = std::make_unique<CollisionManager>();
	allocator_.clear();
	gravity_ = gravity;
	lastTime_ = std::chrono::high_resolution_clock::now();
}

void World::Solve()
{
	//deltaTime_ = DirectXCore::GetInstance()->GetDeletTime();
	for (auto& obj : allocator_) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - lastTime_;
		deltaTime_ = deltaTime.count();
		lastTime_ = currentTime;
		deltaTime_ = 1.0f / 60.0f;
		obj->Solve();
	}

	collisionManager_->ClearCollider();
	for (auto& collider : allocator_) {
		collisionManager_->SetCollider(collider);
	}

	collisionManager_->CheckAllCollision();


}
