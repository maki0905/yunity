#include "Physics.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Novice.h"
void DrawBall(const Ball& ball, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 15; // 分割数
	const float kLonEvery = float(M_PI / float(kSubdivision)); // 経度分割1つ分の角度
	const float kLatEvery = float(2.0f * M_PI / float(kSubdivision)); // 緯度分割1つ分の角度
	Vector3 center = ball.position;
	float radius = ball.radius;
	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = float(-M_PI / 2.0f) + float(kLatEvery * float(latIndex)); // 現在の緯度
		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = float(float(lonIndex) * kLonEvery); // 現在の経度
			// world座標系でのa,b,cを求める
			Vector3 a, b, c;
			a = Multiply(radius, { std::cos(lat) * std::cos(lon), std::sin(lat), std::cos(lat) * std::sin(lon) });
			b = Multiply(radius, { std::cos(lat + kLatEvery) * std::cos(lon), std::sin(lat + kLatEvery), std::cos(lat + kLatEvery) * std::sin(lon) });
			c = Multiply(radius, { std::cos(lat) * std::cos(lon + kLonEvery), std::sin(lat), std::cos(lat) * std::sin(lon + kLonEvery) });
			// a, b, c, をScreen座標系まで変換
			Vector3 ndcVertex = Transform(Add(a, center), viewProjectionMatrix);
			a = Transform(ndcVertex, viewportMatrix);
			ndcVertex = Transform(Add(b, center), viewProjectionMatrix);
			b = Transform(ndcVertex, viewportMatrix);
			ndcVertex = Transform(Add(c, center), viewProjectionMatrix);
			c = Transform(ndcVertex, viewportMatrix);
			// ab, acで線を引く
			Novice::DrawLine(int(a.x), int(a.y), int(b.x), int(b.y), color);
			Novice::DrawLine(int(a.x), int(a.y), int(c.x), int(c.y), color);
		}

	}
}

Vector3 Reflect(const Vector3& input, const Vector3& normal)
{
	Vector3 result;
	result = Subtract(input, Multiply(2 * Dot(input, normal), normal));
	return result;
}
