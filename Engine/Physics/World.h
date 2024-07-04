#pragma once

#include <list>
#include <memory>

#include "MathFunction.h"
//#include "Body.h"
#include "CollisionManager.h"

class Object3D;
//class CollisionManager;

class World {
public:

	//void Step();
	void Initialize(const Vector3& gravity = {0.0f, 0.0f, 0.0f});

	void Solve();

	void Add(Object3D* collider) { allocator_.push_back(collider); };

	std::list<Object3D*> GetAllocator() { return allocator_; }

	void SetGravity(const Vector3& gravity) { gravity_ = gravity; };
	Vector3 GetGravity() { return gravity_; };
	// デルタタイム
	float deltaTime_ = 1.0f / 60.0f;

private:

	std::unique_ptr<CollisionManager> collisionManager_;
	std::list<Object3D*> allocator_;
	Vector3 gravity_;

};