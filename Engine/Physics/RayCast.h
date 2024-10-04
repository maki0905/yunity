#pragma once

#include "Collider.h"
#include "MathFunction.h"
#include "World.h"

struct RayCastHit {
	Object3D* collider;
	float distance;
	Vector3 point;
};

bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, float maxDistance, World* world, uint32_t mask = 0);
bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, World* world, uint32_t mask = 0);

Vector3 RayIntersection(Segment segment, AABB aabb);
Vector3 RayIntersection(Ray ray, AABB aabb);