#pragma once

#include <list>
#include <memory>
#include<chrono>

#include "MathFunction.h"
//#include "Body.h"
#include "CollisionManager.h"

class SpringJoint;
class Object3D;
//class CollisionManager;

class World {
public:

	//void Step();
	void Initialize(const Vector3& gravity = {0.0f, 0.0f, 0.0f});

	void Solve();

	void Add(Object3D* collider) { allocator_.push_back(collider); };
	void Take(Object3D* collider);

	void AddJoint(SpringJoint* springJoint) { jointAllocator_.push_back(springJoint); };
	void TakeJoint(SpringJoint* springJoint);

	std::list<Object3D*> GetAllocator() { return allocator_; }

	void SetGravity(const Vector3& gravity) { gravity_ = gravity; };
	Vector3 GetGravity() { return gravity_; };

	void SetFixedTime(bool isFixedTime) { isFixedTime_ = isFixedTime; }
	// デルタタイム
	//float deltaTime_ = 1.0f / 60.0f;
	float deltaTime_;
	const float fixedDeltaTime_ = 1.0f / 60.0f;

private:
	std::chrono::steady_clock::time_point lastTime_;
	std::unique_ptr<CollisionManager> collisionManager_;
	std::list<Object3D*> allocator_;
	std::list<SpringJoint*> jointAllocator_;
	Vector3 gravity_;
	bool isFixedTime_;

};