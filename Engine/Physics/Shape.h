#pragma once

#include "MathFunction.h"
#include "Sphere.h"
#include "AABB.h"
#include "OBB.h"

/// <summary>
/// AABB生成
/// </summary>
/// <param name="traslation"></param>
/// <param name="size"></param>
/// <returns></returns>
AABB CreateAABB(Vector3 traslation, Vector3 size);

/// <summary>
/// OBB生成
/// </summary>
/// <param name="translation"></param>
/// <param name="rotation"></param>
/// <param name="size"></param>
/// <returns></returns>
OBB CreateOBB(Vector3 translation, Vector3 rotation, Vector3 size);
