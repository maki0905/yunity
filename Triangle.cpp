#include "Triangle.h"
#include "MathFunction.h"
#include "Novice.h"

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 screenVertices[3];
	for (uint32_t i = 0; i < 3; ++i) {
		Vector3 ndcVertex = Transform(triangle.vertices[i], viewProjectionMatrix);
		screenVertices[i] = Transform(ndcVertex, viewportMatrix);
	}

	Novice::DrawTriangle(
		int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y),
		int(screenVertices[2].x), int(screenVertices[2].y), color, kFillModeWireFrame);
}
