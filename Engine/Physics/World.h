#pragma once

#include <list>
#include <memory>

#include "MathFunction.h"
//#include "Body.h"
#include "CollisionManager.h"

class Body;
//class CollisionManager;

class World {
public:

	//void Step();
	void Initialize(const Vector3& gravity = {0.0f, 0.0f, 0.0f});

	void Solve();

	void Add(Body* collider) { allocator_.push_back(collider); };

	void SetGravity(const Vector3& gravity) { gravity_ = gravity; };
	Vector3 GetGravity() { return gravity_; };
	// デルタタイム
	float deltaTime_ = 1.0f / 60.0f;

private:

	std::unique_ptr<CollisionManager> collisionManager_;
	std::list<Body*> allocator_;
	Vector3 gravity_;

};