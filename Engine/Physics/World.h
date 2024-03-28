#pragma once

#include <list>

#include "Collider.h"
#include "CollisionManager.h"

class World {
public:

	//void Step();
	void Initialize();

	void Solve();

	void Add(Collider* collider) { allocator_.push_back(collider); }

private:
	//void Solve();

	std::unique_ptr<CollisionManager> collisionManager_;

	std::list<Collider*> allocator_;

};