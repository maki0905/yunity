#pragma once

#include "Collider.h"
#include "MathFunction.h"
#include "World.h"
#include "Object3D.h"

struct RayCastHit {
	yunity::Object3D* collider;
	float distance;
	Vector3 point;
};

/// <summary>
/// レイキャスト
/// </summary>
/// <param name="origin"></param>
/// <param name="direction"></param>
/// <param name="hitInfo"></param>
/// <param name="maxDistance"></param>
/// <param name="world"></param>
/// <param name="mask"></param>
/// <returns></returns>
bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, float maxDistance, yunity::World* world, uint32_t mask = 0);

/// <summary>
/// レイキャスト
/// </summary>
/// <param name="origin"></param>
/// <param name="direction"></param>
/// <param name="hitInfo"></param>
/// <param name="world"></param>
/// <param name="mask"></param>
/// <returns></returns>
bool RayCast(const Vector3& origin, const Vector3& direction, RayCastHit* hitInfo, yunity::World* world, uint32_t mask = 0);

/// <summary>
/// レイ交点
/// </summary>
/// <param name="segment"></param>
/// <param name="aabb"></param>
/// <returns></returns>
Vector3 RayIntersection(Segment segment, AABB aabb);

/// <summary>
///　レイ交点
/// </summary>
/// <param name="ray"></param>
/// <param name="aabb"></param>
/// <returns></returns>
Vector3 RayIntersection(Ray ray, AABB aabb);