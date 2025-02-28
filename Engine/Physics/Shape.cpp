#include "Shape.h"

AABB CreateAABB(Vector3 traslation, Vector3 size)
{
	// AABBを生成
    return AABB(Subtract(traslation, size), Add(traslation, size));
}

OBB CreateOBB(Vector3 translation, Vector3 rotation, Vector3 size)
{
	// 回転行列を作成
	Matrix4x4 rotate = MakeRotateXYZMatrix(rotation);
	// 回転行列から各軸の向きを取得
	Vector3 orientations[3] = { {rotate.m[0][0], rotate.m[0][1], rotate.m[0][2]}, {rotate.m[1][0], rotate.m[1][1], rotate.m[1][2]}, {rotate.m[2][0], rotate.m[2][1], rotate.m[2][2]} };
	// OBBを生成
	return OBB(translation, { orientations[0], orientations[1], orientations[2] }, size);
}

yunity::CollisionShape::~CollisionShape()
{
}
