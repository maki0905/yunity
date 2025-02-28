#include "RayCast.h"

#include "Collision.h"
#include "CollisionConfig.h"
#include "Shape.h"

#include <algorithm>

bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, float maxDistance, yunity::World* world, uint32_t mask)
{
	Segment ray;
	ray.origin = origin;
	ray.diff = Multiply(maxDistance, direction);

	for (auto& obj : world->GetAllocator()) {
		if ((mask & obj->GetCollisionAttribute()) == 0) {
			continue;
		}

		switch (obj->GetShape())
		{
		case yunity::ShapeType::kSphere:

			break;
		case yunity::ShapeType::kBox:

			if (obj->GetWorldTransform()->rotation_.Length() == 0.0f) {
				AABB aabb;
				aabb.min = Subtract(obj->GetTranslation(), obj->GetHitBoxSize());
				aabb.max = Add(obj->GetTranslation(), obj->GetHitBoxSize());
				if (IsCollision(ray, aabb)) {
					hitInfo->collider = obj;
					hitInfo->point = RayIntersection(ray, aabb);
					return true;
				}
			}
			else {
				OBB obb;
				obb = CreateOBB(obj->GetMatWorldTranslation(), obj->GetWorldTransform()->rotation_, obj->GetHitBoxSize());
				if (IsCollision(ray, obb)) {
					hitInfo->collider = obj;
					hitInfo->point = RayIntersection(ray, obb);
					return true;
				}
				break;
			}
		}
		
	}
    return false;
}

bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, yunity::World* world, uint32_t mask)
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
		case yunity::ShapeType::kSphere:

			break;
		case yunity::ShapeType::kBox:
			AABB aabb;
			aabb.min = Subtract(obj->GetTranslation(), obj->GetHitBoxSize());
			aabb.max = Add(obj->GetTranslation(), obj->GetHitBoxSize());
			if (IsCollision(ray, aabb)) {
				hitInfo->collider = obj;
				hitInfo->point = RayIntersection(ray, aabb);
				return true;
			}
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

Vector3 RayIntersection(Segment segment, OBB obb)
{
	Matrix4x4 W;
	W.m[0][0] = obb.orientations[0].x;
	W.m[0][1] = obb.orientations[0].y;
	W.m[0][2] = obb.orientations[0].z;
	W.m[0][3] = 0;
	W.m[1][0] = obb.orientations[1].x;
	W.m[1][1] = obb.orientations[1].y;
	W.m[1][2] = obb.orientations[1].z;
	W.m[1][3] = 0;
	W.m[2][0] = obb.orientations[2].x;
	W.m[2][1] = obb.orientations[2].y;
	W.m[2][2] = obb.orientations[2].z;
	W.m[2][3] = 0;
	W.m[3][0] = obb.center.x;
	W.m[3][1] = obb.center.y;
	W.m[3][2] = obb.center.z;
	W.m[3][3] = 1;
	Matrix4x4 WInverse = Inverse(W);
	Vector3 localOrigin = TransformVector3(segment.origin, WInverse);
	Vector3 localEnd = TransformVector3(Add(segment.origin, segment.diff), WInverse);

	AABB localAABB{
		{-obb.size.x, -obb.size.y, -obb.size.z},
		{obb.size.x, obb.size.y, obb.size.z}
	};

	Segment localSegment;
	localSegment.origin = localOrigin;
	localSegment.diff = Subtract(localEnd, localOrigin);

	Vector3 result;


	float tNearX = std::min((localAABB.min.x - localSegment.origin.x) / localSegment.diff.x, (localAABB.max.x - localSegment.origin.x) / localSegment.diff.x);
	float tNearY = std::min((localAABB.min.y - localSegment.origin.y) / localSegment.diff.y, (localAABB.max.y - localSegment.origin.y) / localSegment.diff.y);
	float tNearZ = std::min((localAABB.min.z - localSegment.origin.z) / localSegment.diff.z, (localAABB.max.z - localSegment.origin.z) / localSegment.diff.z);
	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = max(max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	result = Add(localSegment.origin, Multiply(tmin, localSegment.diff));
	result = Add(obb.center, result);
	return result;
}
