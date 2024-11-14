#include "RayCast.h"

#include "Collision.h"
#include "CollisionConfig.h"
#include "Object3D.h"

#include <algorithm>

bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, float maxDistance, World* world, uint32_t mask)
{
	Segment ray;
	ray.origin = origin;
	ray.diff = Multiply(maxDistance, direction);

	for (auto& obj : world->GetAllocator()) {
		if ((mask ^ obj->GetCollisionAttribute()) == 0) {
			continue;
		}

		switch (obj->GetShape())
		{
		case Collider::Shape::kSphere:

			break;
		case Collider::Shape::kAABB:
			AABB aabb;
			aabb.min = Subtract(obj->GetTranslation(), obj->GetHitBoxSize());
			aabb.max = Add(obj->GetTranslation(), obj->GetHitBoxSize());
			if (IsCollision(ray, aabb)) {
				hitInfo->collider = obj;
				hitInfo->point = RayIntersection(ray, aabb);
				return true;
			}
			break;
		case Collider::Shape::kOBB:

			break;

		}
		
	}
    return false;
}

bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, World* world, uint32_t mask)
{
	Ray ray;
	ray.origin = origin;
	ray.diff = direction;

	for (auto& obj : world->GetAllocator()) {
		if ((mask ^ obj->GetCollisionAttribute()) == 0) {
			continue;
		}

		switch (obj->GetShape())
		{
		case Collider::Shape::kSphere:

			break;
		case Collider::Shape::kAABB:
			AABB aabb;
			aabb.min = Subtract(obj->GetTranslation(), obj->GetHitBoxSize());
			aabb.max = Add(obj->GetTranslation(), obj->GetHitBoxSize());
			if (IsCollision(ray, aabb)) {
				hitInfo->collider = obj;
				hitInfo->point = RayIntersection(ray, aabb);
				return true;
			}
			break;
		case Collider::Shape::kOBB:

			break;

		}

	}
	return false;
}

Vector3 RayIntersection(Segment segment, AABB aabb)
{
	Vector3 result;


	float tNearX = std::min((aabb.min.x - segment.origin.x) / segment.diff.x, (aabb.max.x - segment.origin.x) / segment.diff.x);
	float tNearY = std::min((aabb.min.y - segment.origin.y) / segment.diff.y, (aabb.max.y - segment.origin.y) / segment.diff.y);
	float tNearZ = std::min((aabb.min.z - segment.origin.z) / segment.diff.z, (aabb.max.z - segment.origin.z) / segment.diff.z);
	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	result = Add(segment.origin, Multiply(tmin, segment.diff));

	return result;
}

Vector3 RayIntersection(Ray ray, AABB aabb)
{
	Vector3 result;

	float tNearX = std::min((aabb.min.x - ray.origin.x) / ray.diff.x, (aabb.max.x - ray.origin.x) / ray.diff.x);
	float tNearY = std::min((aabb.min.y - ray.origin.y) / ray.diff.y, (aabb.max.y - ray.origin.y) / ray.diff.y);
	float tNearZ = std::min((aabb.min.z - ray.origin.z) / ray.diff.z, (aabb.max.z - ray.origin.z) / ray.diff.z);
	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方

	result = Add(ray.origin, Multiply(tmin, ray.diff));

	return result;
}
