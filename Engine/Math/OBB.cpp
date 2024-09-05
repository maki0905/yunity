#include "OBB.h"

bool TestAxis(const Vector3& axis, const OBB& obb1, const OBB& obb2) {
	float r1 = obb1.size.x * std::fabsf(Dot(obb1.orientations[0], axis)) + obb1.size.y * std::fabsf(Dot(obb1.orientations[1], axis)) + obb1.size.z * std::fabsf(Dot(obb1.orientations[2], axis));
	float r2 = obb2.size.x * std::fabsf(Dot(obb2.orientations[0], axis)) + obb2.size.y * std::fabsf(Dot(obb2.orientations[1], axis)) + obb2.size.z * std::fabsf(Dot(obb2.orientations[2], axis));
	float distance = std::fabsf(Dot(Subtract(obb2.center, obb1.center), axis));
	return distance <= r1 + r2;
}
