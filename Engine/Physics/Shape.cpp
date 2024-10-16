#include "Shape.h"

AABB CreateAABB(Vector3 traslation, Vector3 size)
{
    return AABB(Subtract(traslation, size), Add(traslation, size));
}

OBB CreateOBB(Vector3 translation, Vector3 rotation, Vector3 size)
{
	Matrix4x4 rotate = MakeRotateXYZMatrix(rotation);
	Vector3 orientations[3] = { {rotate.m[0][0], rotate.m[0][1], rotate.m[0][2]}, {rotate.m[1][0], rotate.m[1][1], rotate.m[1][2]}, {rotate.m[2][0], rotate.m[2][1], rotate.m[2][2]} };
	return OBB(translation, { orientations[0], orientations[1], orientations[2] }, size);
}
