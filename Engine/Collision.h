#pragma once

#include "Sphere.h"
#include "Plane.h"
#include "Line.h"
#include "Triangle.h"
#include "AABB.h"
#include "OBB.h"
#include "Capsule.h"

bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

bool IsCollision(const Sphere& sphere, const Plane& plane);

bool IsCollision(const Plane& plane, const Sphere& sphere);

bool IsCollision(const Line& line, const Plane& plane);

bool IsCollision(const Plane& plane, const Line& line);

bool IsCollision(const Ray& ray, const Plane& plane);

bool IsCollision(const Plane& plane, const Ray& ray);

bool IsCollision(const Segment& segment, const Plane& plane);

bool IsCollision(const Plane& plane, const Segment& segment);

bool IsCollision(const Triangle& triangle, const Segment& segment);

bool IsCollision(const Segment& segment, const Triangle& triangle);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

bool IsCollision(const AABB& aabb, const Sphere& sphere);

bool IsCollision(const Sphere& sphere, const AABB& aabb);

bool IsCollision(const AABB& aabb, const Segment& segment);

bool IsCollision(const Segment& segment, const AABB& aabb);

bool IsCollision(const AABB& aabb, const Ray& ray);

bool IsCollision(const Ray& ray, const AABB& aabb);

bool IsCollision(const OBB& obb, const Sphere& sphere);

bool IsCollision(const Sphere& sphere, const OBB& obb);

bool IsCollision(const OBB& obb, const Segment& segment);

bool IsCollision(const Segment& segment, const OBB& obb);

bool IsCollision(const Capsule& capsule, const Sphere& sphere);