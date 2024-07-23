#include "World.h"

#include "Object3D.h"
#include "DirectXCore.h"
#include "ImGuiManager.h"

void World::Initialize(const Vector3& gravity)
{
	collisionManager_ = std::make_unique<CollisionManager>();
	allocator_.clear();
	gravity_ = gravity;
	lastTime_ = std::chrono::high_resolution_clock::now();
	isFixedTime_ = true;
}

void World::Solve()
{
	//deltaTime_ = DirectXCore::GetInstance()->GetDeletTime();
	float time = 0.0f;
	if (!isFixedTime_) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - lastTime_;
		deltaTime_ = deltaTime.count();
		lastTime_ = currentTime;
	}
	else {
		time = fixedDeltaTime_;
	}

	for (auto& obj : allocator_) {
		obj->Solve(time);
	}

	collisionManager_->ClearCollider();
	for (auto& collider : allocator_) {
		collisionManager_->SetCollider(collider);
	}

	collisionManager_->CheckAllCollision();


}

void World::Take(Object3D* collider)
{
	for (std::list<Object3D*>::iterator iterator = allocator_.begin(); iterator != allocator_.end();) {
		if (*iterator == collider) {
			iterator = allocator_.erase(iterator);
			continue;
		}

		iterator++;
	}

}
