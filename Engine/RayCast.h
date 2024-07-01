#pragma once

#include "Collider.h"
#include "MathFunction.h"
#include "World.h"

struct RayCastHit {
	Collider* collider;
	float distance;
	Vector3 point;
};

bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, float maxDistance, World* world, uint32_t mask = 0);