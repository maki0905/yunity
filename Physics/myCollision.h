#pragma once

#include <limits.h>
#include <cfloat>

#include "MathFunction.h"
#include "Setting.h"

class Shape;
class CircleShape;
class EdgeShape;
class PolygonShape;

const uint32_t nullFeature = UCHAR_MAX;

struct Rot {
	Rot() {};

	explicit Rot(float angle) {
		s = sinf(angle);
		c = cosf(angle);
	}

	void Set(float angle) {
		s = sinf(angle);
		c = cosf(angle);
	}

	void SetIdentity() {
		s = 0.0f;
		c = 1.0f;
	}

	float GetAngle() const {
		return atan2(s, c);
	}

	Vector2 GetXAxis() const {
		return Vector2(c, s);

	}

	Vector2 GetYAxis()const {
		return Vector2(-s, c);
	}

	float s, c;
};

struct myTransform {

	myTransform(){}

	myTransform(const Vector2& position, const Rot& rotation) : p(position), q(rotation){}

	void SetIndentity() {
		p.SetZero();
		q.SetIdentity();
	}

	void Set(const Vector2& postion, float angle) {
		p = postion;
		q.Set(angle);
	}

	Vector2 p;
	Rot q;

};

struct ContactFeature {
	enum Type {
		e_vertex = 0,
		e_face = 1,
	};

	uint32_t indexA;
	uint32_t indexB;
	uint32_t typeA;
	uint32_t typeB;

};

union ContactID {
	ContactFeature cf;
	uint32_t key;
};

struct ManifoldPoint {
	Vector2 localPoint;
	float normalImpulse;
	float tangentImpulse;
	ContactID id;
};

struct Manifold {
	enum Type {
		e_circels,
		e_faceA,
		e_faceB,
	};

	ManifoldPoint points[maxManifoldPoints];
	Vector2 localNormal;
	Vector2 localPoint;
	Type type;
	int32_t pointCount;
};

struct WorldManifold {
	void Initialize(const Manifold* manifold,
		const myTransform& xfA, float radiusA,
		const myTransform& xfB, float radiusB);

	Vector2 normal;
	Vector2 points[maxManifoldPoints];
	float separations[maxManifoldPoints];
};

enum PointState {
	nullState,
	addState,
	persistState,
	removeState
};

void GetPointStates(PointState state1[maxManifoldPoints], PointState state2[maxManifoldPoints],
	const Manifold* manifold1, const Manifold* manifold2);

struct ClipVertex {
	Vector2 v;
	ContactID id;
};

struct RayCastInput {
	Vector2 p1, p2;
	float maxFraction;
};

struct RayCastOutput {
	Vector2 normal;
	float fraction;
};

struct AABB {
	bool IsValid() const;

	Vector2 GetCenter()const {
		return Multiply(0.5f, Add(lowerBound, upperBound));
	}

	float GetPerimeter()const {
		float wx = upperBound.x - lowerBound.x;
		float wy = upperBound.y - lowerBound.y;
		return 2.0f * (wx + wy);
	}

	void Combine(const AABB& aabb) {
		lowerBound = Min(lowerBound, aabb.lowerBound);
		upperBound = Max(upperBound, aabb.upperBound);
	}

	void Combine(const AABB& aabb1, const AABB& aabb2) {
		lowerBound = Min(aabb1.lowerBound, aabb2.lowerBound);
		upperBound = Max(aabb1.upperBound, aabb2.upperBound);
	}

	bool Contains(const AABB& aabb)const {
		bool result = true;
		result = result && lowerBound.x <= aabb.lowerBound.x;
		result = result && lowerBound.y <= aabb.lowerBound.y;
		result = result && aabb.upperBound.x <= upperBound.x;
		result = result && aabb.upperBound.y <= upperBound.y;
		return result;
	}

	bool RayCast(RayCastOutput* output, const RayCastInput& input) const;

	Vector2 lowerBound;
	Vector2 upperBound;
};

void CollideCircles(
	Manifold* manifold, 
	const CircleShape* circleA, const myTransform& xfA, 
	const CircleShape* circleB, const myTransform& xfB);

void CollidePolygonAndCircle(
	Manifold* manifold, 
	const PolygonShape* polygonA, const myTransform& xfA, 
	const CircleShape* circleB, const myTransform& xfB);

void CollidePolygons(
	Manifold* manifold,
	const PolygonShape* polygonA, const myTransform& xfA, 
	const PolygonShape* polygonB, const myTransform& xfB);

void CollideEdgeAndCircle(
	Manifold* manifold, 
	const EdgeShape* edgeA, const myTransform& xfA, 
	const CircleShape* circleB, const myTransform& xfB);

void CollideEdgeAndPolygon(Manifold* manifold,
	const EdgeShape* edgeA, const myTransform& xfA,
	const PolygonShape* polygonB, const myTransform& xfB);

int32_t ClipSegmentToLine(ClipVertex vOut[2], const ClipVertex vIn[2], const Vector2& normal, float offset, int32_t vertexIndexA);

bool TestOverlap(const Shape* shapeA, int32_t indexA, const Shape* shapeB, int32_t indexB, const myTransform& xfA, const myTransform& xfB);

inline bool AABB::IsValid()const {
	Vector2 d = Subtract(upperBound, lowerBound);
	bool valid = d.x >= 0.0f && d.y >= 0.0f;
	valid = valid && lowerBound.IsValid() && upperBound.IsValid();
	return valid;
}

inline bool TestOverlap(const AABB& a, const AABB& b) {
	Vector2 d1, d2;
	d1 = Subtract(b.lowerBound, a.upperBound);
	d2 = Subtract(a.lowerBound, b.upperBound);

	if (d1.x > 0.0f || d1.y > 0.0f) {
		return false;
	}
	if (d2.x > 0.0f || d2.y > 0.0f) {
		return false;
	}
	return true;
}