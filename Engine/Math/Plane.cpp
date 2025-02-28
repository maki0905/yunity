#include "Plane.h"

#include "MathFunction.h"

Plane PlaneFromTri(const Vector3& point0, const Vector3& point1, const Vector3& point2)
{
	Vector3 p1p0 = Subtract(point1, point0);
	Vector3 p2p0 = Subtract(point2, point0);

	Vector3 normal = Normalize(Cross(p1p0, p2p0));


	float d = -Dot(point0, normal);
	return Plane(normal, d);
}

float DistanceFromPlane(const Plane& plane, const Vector3& in)
{
	return Dot(in, plane.normal) + plane.distance;
}

Vector3 ProjectPointOntoPlane(const Plane& plane, const Vector3& point)
{
	float distance = DistanceFromPlane(plane, point);
	return Subtract(point, Multiply(distance, plane.normal));
}