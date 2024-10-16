#pragma once

#include "MathFunction.h"
#include "Sphere.h"
#include "AABB.h"
#include "OBB.h"

AABB CreateAABB(Vector3 traslation, Vector3 size);

OBB CreateOBB(Vector3 translation, Vector3 rotation, Vector3 size);
