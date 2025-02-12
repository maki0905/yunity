#include "World.h"

#include "DirectXCore.h"
#include "ImGuiManager.h"
#include "Joint.h"
#include "Object3D.h"

void yunity::World::Initialize(const Vector3& gravity)
{
	collisionManager_ = std::make_unique<CollisionManager>();
	objectList_.clear();
	jointList_.clear();
	gravity_ = gravity;
	lastTime_ = std::chrono::high_resolution_clock::now();
	isFixedTime_ = true;
}

void yunity::World::Solve()
{
	float time = 0.0f;

	// 時間計測
	if (!isFixedTime_) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> deltaTime = currentTime - lastTime_;
		deltaTime_ = deltaTime.count();
		lastTime_ = currentTime;
	}
	else {
		time = fixedDeltaTime_;
	}

	// ジョイント解決
	for (auto& joint : jointList_) {
		joint->Solve();
	}

	// 物理演算
	for (auto& obj : objectList_) {
		//obj->Solve(time);

		for (int i = 0; i < 8; i++) {
			obj->SolveVelocity(time / 8.0f);
		}
		for (int i = 0; i < 5; i++) {
			obj->SolvePosition(time / 5.0f);
		}
	}

	// 衝突判定
	collisionManager_->ClearCollider();
	for (auto& collider : objectList_) {
		collisionManager_->SetCollider(collider);
	}
	collisionManager_->CheckAllCollision();

	// 衝突解決
	for (auto& obj : objectList_) {
		obj->SolveConstraints();
	}


}

void yunity::World::Take(Object3D* collider)
{
	for (std::list<Object3D*>::iterator iterator = objectList_.begin(); iterator != objectList_.end();) {
		if (*iterator == collider) {
			iterator = objectList_.erase(iterator);
			continue;
		}

		iterator++;
	}

}

void yunity::World::AddJoint(Joint* joint)
{
	bool duplicationCheck = true;
	for (std::list<Joint*>::iterator iterator = jointList_.begin(); iterator != jointList_.end();) {
		if (*iterator == joint) {
			duplicationCheck = false;
		}
		iterator++;
	}
	if (duplicationCheck) {
		jointList_.emplace_back(joint);
	}
}

void yunity::World::TakeJoint(Joint* joint)
{
	for (std::list<Joint*>::iterator iterator = jointList_.begin(); iterator != jointList_.end();) {
		if (*iterator == joint) {
			iterator = jointList_.erase(iterator);
			continue;
		}

		iterator++;
	}
}
