#include "RayCast.h"

#include "Collision.h"
#include "Body.h"

bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, float maxDistance, World* world)
{
	Segment ray;
	ray.origin = origin;
	ray.diff = Multiply(maxDistance, direction);

	for (auto& obj : world->GetAllocator()) {
		switch (obj->GetShape())
		{
		case Collider::Shape::kSphere:

			break;
		case Collider::Shape::kAABB:
			AABB aabb;
			aabb.min = Subtract(obj->GetWorldTransform().translation_, obj->GetHitBoxSize());
			aabb.max = Add(obj->GetWorldTransform().translation_, obj->GetHitBoxSize());
			if (IsCollision(ray, aabb)) {
				return true;
			}
			break;
		case Collider::Shape::kOBB:

			break;

		}
		
	}
    return false;
}
