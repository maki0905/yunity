#include "AABB.h"
#include "MathFunction.h"
#include "Novice.h"
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 screenPoint[8];
	Vector3 ndcVertex = Transform(aabb.min, viewProjectionMatrix);
	screenPoint[0] = Transform(ndcVertex, viewportMatrix);
	
	ndcVertex = Transform(Vector3(aabb.max.x, aabb.min.y, aabb.min.z), viewProjectionMatrix);
	screenPoint[1] = Transform(ndcVertex, viewportMatrix);
	
	ndcVertex = Transform(Vector3(aabb.max.x, aabb.min.y, aabb.max.z), viewProjectionMatrix);
	screenPoint[2] = Transform(ndcVertex, viewportMatrix);
	
	ndcVertex = Transform(Vector3(aabb.min.x, aabb.min.y, aabb.max.z), viewProjectionMatrix);
	screenPoint[3] = Transform(ndcVertex, viewportMatrix);
	
	ndcVertex = Transform(Vector3(aabb.max.x, aabb.max.y, aabb.min.z), viewProjectionMatrix);
	screenPoint[4] = Transform(ndcVertex, viewportMatrix);
	
	ndcVertex = Transform(Vector3(aabb.min.x, aabb.max.y, aabb.min.z), viewProjectionMatrix);
	screenPoint[5] = Transform(ndcVertex, viewportMatrix);
	
	ndcVertex = Transform(Vector3(aabb.min.x, aabb.max.y, aabb.max.z), viewProjectionMatrix);
	screenPoint[6] = Transform(ndcVertex, viewportMatrix);
	
	ndcVertex = Transform(aabb.max, viewProjectionMatrix);
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
