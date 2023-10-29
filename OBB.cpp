#include "OBB.h"
#include "MathFunction.h"
#include "Novice.h"

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 screenPoint[8];
	//Vector3 min = Subtract(obb.center, obb.size);
	//Vector3 max = Add(obb.center, obb.size);
	Vector3 min = { -obb.size.x, -obb.size.y, -obb.size.z };
	Vector3 max = { obb.size.x, obb.size.y, obb.size.z };
	Matrix4x4 R;
	R.m[0][0] = obb.orientations[0].x;
	R.m[0][1] = obb.orientations[0].y;
	R.m[0][2] = obb.orientations[0].z;
	R.m[0][3] = 0;
	R.m[1][0] = obb.orientations[1].x;
	R.m[1][1] = obb.orientations[1].y;
	R.m[1][2] = obb.orientations[1].z;
	R.m[1][3] = 0;
	R.m[2][0] = obb.orientations[2].x;
	R.m[2][1] = obb.orientations[2].y;
	R.m[2][2] = obb.orientations[2].z;
	R.m[2][3] = 0;
	R.m[3][0] = 0;
	R.m[3][1] = 0;
	R.m[3][2] = 0;
	R.m[3][3] = 1;

	Vector3 ndcVertex = Transform(min, R);
	ndcVertex = Add(ndcVertex, obb.center);
	ndcVertex = Transform(ndcVertex, viewProjectionMatrix);
	screenPoint[0] = Transform(ndcVertex, viewportMatrix);

	ndcVertex = Transform(Vector3(max.x, min.y, min.z), R);
	ndcVertex = Add(ndcVertex, obb.center);
	ndcVertex = Transform(ndcVertex, viewProjectionMatrix);
	screenPoint[1] = Transform(ndcVertex, viewportMatrix);

	ndcVertex = Transform(Vector3(max.x, min.y, max.z), R);
	ndcVertex = Add(ndcVertex, obb.center);
	ndcVertex = Transform(ndcVertex, viewProjectionMatrix);
	screenPoint[2] = Transform(ndcVertex, viewportMatrix);

	ndcVertex = Transform(Vector3(min.x, min.y, max.z), R);
	ndcVertex = Add(ndcVertex, obb.center);
	ndcVertex = Transform(ndcVertex, viewProjectionMatrix);
	screenPoint[3] = Transform(ndcVertex, viewportMatrix);

	ndcVertex = Transform(Vector3(max.x, max.y, min.z), R);
	ndcVertex = Add(ndcVertex, obb.center);
	ndcVertex = Transform(ndcVertex, viewProjectionMatrix);
	screenPoint[4] = Transform(ndcVertex, viewportMatrix);

	
	ndcVertex = Transform(Vector3(min.x, max.y, min.z), R);
	ndcVertex = Add(ndcVertex, obb.center);
	ndcVertex = Transform(ndcVertex, viewProjectionMatrix);
	screenPoint[5] = Transform(ndcVertex, viewportMatrix);

	ndcVertex = Transform(Vector3(min.x, max.y, max.z), R);
	ndcVertex = Add(ndcVertex, obb.center);
	ndcVertex = Transform(ndcVertex, viewProjectionMatrix);
	screenPoint[6] = Transform(ndcVertex, viewportMatrix);

	ndcVertex = Transform(max, R);
	ndcVertex = Add(ndcVertex, obb.center);
	ndcVertex = Transform(ndcVertex, viewProjectionMatrix);
	screenPoint[7] = Transform(ndcVertex, viewportMatrix);

	Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[1].x), int(screenPoint[1].y), color);
	Novice::DrawLine(int(screenPoint[1].x), int(screenPoint[1].y), int(screenPoint[2].x), int(screenPoint[2].y), color);
	Novice::DrawLine(int(screenPoint[2].x), int(screenPoint[2].y), int(screenPoint[3].x), int(screenPoint[3].y), color);
	Novice::DrawLine(int(screenPoint[3].x), int(screenPoint[3].y), int(screenPoint[0].x), int(screenPoint[0].y), color);

	Novice::DrawLine(int(screenPoint[4].x), int(screenPoint[4].y), int(screenPoint[5].x), int(screenPoint[5].y), color);
	Novice::DrawLine(int(screenPoint[5].x), int(screenPoint[5].y), int(screenPoint[6].x), int(screenPoint[6].y), color);
	Novice::DrawLine(int(screenPoint[6].x), int(screenPoint[6].y), int(screenPoint[7].x), int(screenPoint[7].y), color);
	Novice::DrawLine(int(screenPoint[7].x), int(screenPoint[7].y), int(screenPoint[4].x), int(screenPoint[4].y), color);

	Novice::DrawLine(int(screenPoint[0].x), int(screenPoint[0].y), int(screenPoint[5].x), int(screenPoint[5].y), color);
	Novice::DrawLine(int(screenPoint[1].x), int(screenPoint[1].y), int(screenPoint[4].x), int(screenPoint[4].y), color);
	Novice::DrawLine(int(screenPoint[2].x), int(screenPoint[2].y), int(screenPoint[7].x), int(screenPoint[7].y), color);
	Novice::DrawLine(int(screenPoint[3].x), int(screenPoint[3].y), int(screenPoint[6].x), int(screenPoint[6].y), color);
}
