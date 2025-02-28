#pragma once
#include "Vector3.h"

/// <param name="normal(Vector3)">法線</param>
/// <param name="distance(float)">距離</param>
struct Plane {
	Vector3 normal; // 法線
	float distance; // 距離
};

Vector3 Perpendicular(const Vector3& vector);

Plane PlaneFromTri(const Vector3& point0, const Vector3& point1, const Vector3& point2);

float DistanceFromPlane(const Plane& plane, const Vector3& in);

Vector3 ProjectPointOntoPlane(const Plane& plane, const Vector3& point);
