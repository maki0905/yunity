#pragma once

#include "Sphere.h"
#include "Plane.h"
#include "Line.h"
#include "Triangle.h"
#include "AABB.h"
#include "OBB.h"
#include "Capsule.h"


/// <summary>
/// 衝突判定
/// </summary>
/// <param name="sphere1"></param>
/// <param name="sphere2"></param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="sphere"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere, const Plane& plane);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="plane"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const Plane& plane, const Sphere& sphere);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="line"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Line& line, const Plane& plane);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="plane"></param>
/// <param name="line"></param>
/// <returns></returns>
bool IsCollision(const Plane& plane, const Line& line);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="ray"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Ray& ray, const Plane& plane);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="plane"></param>
/// <param name="ray"></param>
/// <returns></returns>
bool IsCollision(const Plane& plane, const Ray& ray);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="segment"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Segment& segment, const Plane& plane);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="plane"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const Plane& plane, const Segment& segment);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="triangle"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const Triangle& triangle, const Segment& segment);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="segment"></param>
/// <param name="triangle"></param>
/// <returns></returns>
bool IsCollision(const Segment& segment, const Triangle& triangle);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Sphere& sphere);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="sphere"></param>
/// <param name="aabb"></param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere, const AABB& aabb);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Segment& segment);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="segment"></param>
/// <param name="aabb"></param>
/// <returns></returns>
bool IsCollision(const Segment& segment, const AABB& aabb);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="aabb"></param>
/// <param name="ray"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb, const Ray& ray);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="ray"></param>
/// <param name="aabb"></param>
/// <returns></returns>
bool IsCollision(const Ray& ray, const AABB& aabb);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="obb1"></param>
/// <param name="obb2"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb1, const OBB& obb2);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="obb"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Sphere& sphere);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="sphere"></param>
/// <param name="obb"></param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere, const OBB& obb);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="obb"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const OBB& obb, const Segment& segment);

/// <summary>
/// 衝突判定
/// </summary>
/// <param name="segment"></param>
/// <param name="obb"></param>
/// <returns></returns>
bool IsCollision(const Segment& segment, const OBB& obb);

/// <summary>
///  衝突判定
/// </summary>
/// <param name="capsule"></param>
/// <param name="sphere"></param>
/// <returns></returns>
bool IsCollision(const Capsule& capsule, const Sphere& sphere);