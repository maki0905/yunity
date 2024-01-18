#include "myCollision.h"

#include <cassert>

void WorldManifold::Initialize(const Manifold* manifold, const myTransform& xfA, float radiusA, const myTransform& xfB, float radiusB)
{
	if (manifold->pointCount == 0) {
		return;
	}

	switch (manifold->type) {
	case Manifold::e_circels:
		normal.Set(1.0f, 0.0f);
		Vector2 pointA = Multiply(xfA, manifold->localPoint);
		Vector2 pointB = Multiply(xfB, manifold->points[0].localPoint);
		if (DistanceSquared(pointA, pointB) > FLT_EPSILON * FLT_EPSILON) {
			normal = Subtract(pointB, pointA);
			normal.Normalize();
		}

		Vector2 cA = Add(pointA, Multiply(radiusA, normal));
		Vector2 cB = Subtract(pointB, Multiply(radiusB, normal));
		points[0] = Multiply(0.5f, Add(cA, cB));
		separations[0] = Dot(Subtract(cB, cA), normal);

		break;
	case Manifold::e_faceA:
		normal = Multiply(xfA.q, manifold->localNormal);
		Vector2 planePoint = Multiply(xfA, manifold->localPoint);

		for (uint32_t i = 0; i < manifold->pointCount; ++i) {
			Vector2 clipPoint = Multiply(xfB, manifold->points[i].localPoint);
			Vector2 cA = Add(clipPoint, Multiply(radiusA - Dot(Subtract(clipPoint, planePoint), normal), normal));
			Vector2 cB = Subtract(clipPoint, Multiply(radiusB, normal));
			points[i] = Multiply(0.5f, Add(cA, cB));
			separations[i] = Dot(Subtract(cB, cA), normal);
		}

		break;

	case Manifold::e_faceB:
		normal = Multiply(xfB.q, manifold->localNormal);
		Vector2 planePoint = Multiply(xfB, manifold->localPoint);

		for (uint32_t i = 0; i < manifold->pointCount; i++) {
			Vector2 clipPoint = Multiply(xfA, manifold->points[i].localPoint);
			Vector2 cB = Add(clipPoint, Multiply(radiusB - Dot(Subtract(clipPoint, planePoint), normal), normal));
			Vector2 cA = Subtract(clipPoint, Multiply(radiusA, normal));
			points[i] = Multiply(0.5f, Add(cA, cB));
			separations[i] = Dot(Subtract(cA, cB), normal);
		}
		normal = Multiply(-1.0f, normal);

		break;
	}

}

void GetPointStates(PointState state1[maxManifoldPoints], PointState state2[maxManifoldPoints], const Manifold* manifold1, const Manifold* manifold2)
{
	for (uint32_t i = 0; i < maxManifoldPoints; ++i) {
		state1[i] = nullState;
		state2[i] = nullState;
	}

	for (uint32_t i = 0; i < manifold1->pointCount; ++i) {
		ContactID id = manifold1->points[i].id;

		state1[i] = removeState;

		for (uint32_t j = 0; j < manifold2->pointCount; ++j) {
			if (manifold2->points[j].id.key == id.key) {
				state1[i] = persistState;
				break;
			}
		}
	}

	for (uint32_t i = 0; i < manifold2->pointCount; ++i) {
		ContactID id = manifold2->points[i].id;
		
		state2[i] = addState;

		for (uint32_t j = 0; j < manifold1->pointCount; j++) {
			if (manifold1->points[j].id.key == id.key) {
				state2[i] = persistState;
				break;
			}
		}
	}

}

bool AABB::RayCast(RayCastOutput* output, const RayCastInput& input) const {
	float tmin = -FLT_MAX;
	float tmax = FLT_MAX;

	Vector2 p = input.p1;
	Vector2 d = Subtract(input.p2, input.p1);
	Vector2 absD = Abs(d);

	Vector2 normal;

	if (absD.x < FLT_EPSILON) {
		if (p.x < lowerBound.x || upperBound.x < p.x) {
			return false;
		}
	}
	else {
		float inv_d = 1.0f / d.x;
		float t1 = (lowerBound.x - p.x) * inv_d;
		float t2 = (upperBound.x - p.x) * inv_d;

		float s = -1.0f;

		if (t1 > t2) {
			Swap(t1, t2);
			s = 1.0f;
		}

		if (t1 > tmin) {
			normal.SetZero();
			normal.x = s;
			tmin = t1;
		}

		tmax = Min(tmax, t2);
		if (tmin > tmax) {
			return false;
		}

	}


	if (absD.y < FLT_EPSILON) {
		if (p.y < lowerBound.y || upperBound.y < p.y) {
			return false;
		}
	}
	else {
		float inv_d = 1.0f / d.y;
		float t1 = (lowerBound.y - p.y) * inv_d;
		float t2 = (upperBound.y - p.y) * inv_d;

		float s = -1.0f;

		if (t1 > t2) {
			Swap(t1, t2);
			s = 1.0f;
		}

		if (t1 > tmin) {
			normal.SetZero();
			normal.y = s;
			tmin = t1;
		}

		tmax = Min(tmax, t2);
		if (tmin > tmax) {
			return false;
		}

	}


	if (tmin < 0.0f || input.maxFraction < tmin) {
		return false;
	}

	output->fraction = tmin;
	output->normal = normal;
	return true;
	

}

int32_t ClipSegmentToLine(ClipVertex vOut[2], const ClipVertex vIn[2], const Vector2& normal, float offset, int32_t vertexIndexA)
{
	uint32_t count = 0;

	float distance0 = Dot(normal, vIn[0].v) - offset;
	float distance1 = Dot(normal, vIn[1].v) - offset;

	if (distance0 <= 0.0f) {
		vOut[count++] = vIn[0];
	}
	if (distance1 <= 0.0f) {
		vOut[count++] = vIn[1];
	}

	if (distance0 * distance1 < 0.0f) {
		float interp = distance0 / (distance0 - distance1);
		vOut[count].v = vIn[0].v + interp * (vIn[1].v - vIn[0].v);

		vOut[count].id.cf.indexA = static_cast<uint8_t>(vertexIndexA);
		vOut[count].id.cf.indexB = vIn[0].id.cf.indexB;
		vOut[count].id.cf.typeA = ContactFeature::e_vertex;
		vOut[count].id.cf.typeB = ContactFeature::e_face;

		++count;

		assert(count == 2);
	}

	return 0;
}

bool TestOverlap(const Shape* shapeA, int32_t indexA, const Shape* shapeB, int32_t indexB, const myTransform& xfA, const myTransform& xfB)
{
	DistanceInput

	return false;
}
