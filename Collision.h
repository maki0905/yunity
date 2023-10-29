#pragma once
#include "Sphere.h"
#include "Plane.h"
#include "Line.h"
#include "Triangle.h"
#include "AABB.h"
#include "OBB.h"

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

bool IsCollision(const Sphere& sphere, const Plane& plane);

bool IsCollision(const Line& line, const Plane& plane);

bool IsCollision(const Ray& ray, const Plane& plane);

bool IsCollision(const Segment& segment, const Plane& plane);

bool IsCollision(const Triangle& triangle, const Segment& segment);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

bool IsCollision(const AABB& aabb, const Sphere& sphere);

bool IsCollision(const AABB& aabb, const Segment& segment);

bool IsCollision(const OBB& obb, const Sphere& sphere);

bool IsCollision(const OBB& obb, const Segment& segment);