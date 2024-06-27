#include "Collision.h"

#include "MathFunction.h"
#include <iostream>
#include <algorithm>
bool IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{
	float distance;
	distance = Length(Subtract(sphere2.center, sphere1.center));
	if (distance <= sphere1.radius + sphere2.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	float distance = Dot(plane.normal, sphere.center) - plane.distance;
	// 絶対値にするためdistanceが0以下だったらマイナス1を掛ける
	if (distance < 0) {
		distance *= -1;
	}
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const Plane& plane, const Sphere& sphere)
{
	float distance = Dot(plane.normal, sphere.center) - plane.distance;
	// 絶対値にするためdistanceが0以下だったらマイナス1を掛ける
	if (distance < 0) {
		distance *= -1;
	}
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const Line& line, const Plane& plane)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;


	if (t >= -1.0f && t <= 1) {
		return true;
	}

	return false;
}

bool IsCollision(const Plane& plane, const Line& line)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, line.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;


	if (t >= -1.0f && t <= 1) {
		return true;
	}

	return false;
}

bool IsCollision(const Ray& ray, const Plane& plane)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, ray.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;


	if (t > 0.0f) {
		return true;
	}

	return false;
}

bool IsCollision(const Plane& plane, const Ray& ray)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, ray.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / dot;


	if (t > 0.0f) {
		return true;
	}

	return false;
}


bool IsCollision(const Segment& segment, const Plane& plane)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;


	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}

	return false;
}

bool IsCollision(const Plane& plane, const Segment& segment)
{
	// 垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;


	if (t >= 0.0f && t <= 1.0f) {
		return true;
	}

	return false;
}

bool IsCollision(const Triangle& triangle, const Segment& segment)
{
	Vector3 v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v2 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 normal = Normalize(Cross(v1, v2));
	/*float distance = (normal.x * triangle.vertices[0].x) + (normal.y * triangle.vertices[0].y) + (normal.z * triangle.vertices[0].z);*/
	float distance = Dot(triangle.vertices[0], normal);
	float dot = Dot(normal, segment.diff);
	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (distance - Dot(segment.origin, normal)) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));

	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v1p = Subtract(p, triangle.vertices[1]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v2p = Subtract(p, triangle.vertices[2]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
	Vector3 v0p = Subtract(p, triangle.vertices[0]);

	// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross01, normal) >= 0.0f && Dot(cross12, normal) >= 0.0f && Dot(cross20, normal) >= 0.0f) {
		return true;
	}

	return false;


}

bool IsCollision(const Segment& segment, const Triangle& triangle)
{
	Vector3 v1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v2 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 normal = Normalize(Cross(v1, v2));
	/*float distance = (normal.x * triangle.vertices[0].x) + (normal.y * triangle.vertices[0].y) + (normal.z * triangle.vertices[0].z);*/
	float distance = Dot(triangle.vertices[0], normal);
	float dot = Dot(normal, segment.diff);
	// 垂直=平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}

	float t = (distance - Dot(segment.origin, normal)) / dot;

	if (t < 0.0f || t > 1.0f) {
		return false;
	}

	Vector3 p = Add(segment.origin, Multiply(t, segment.diff));

	Vector3 v01 = Subtract(triangle.vertices[1], triangle.vertices[0]);
	Vector3 v1p = Subtract(p, triangle.vertices[1]);
	Vector3 v12 = Subtract(triangle.vertices[2], triangle.vertices[1]);
	Vector3 v2p = Subtract(p, triangle.vertices[2]);
	Vector3 v20 = Subtract(triangle.vertices[0], triangle.vertices[2]);
	Vector3 v0p = Subtract(p, triangle.vertices[0]);

	// 各辺を結んだベクトルと、頂点と衝突点pを結んだベクトルのクロス積を取る
	Vector3 cross01 = Cross(v01, v1p);
	Vector3 cross12 = Cross(v12, v2p);
	Vector3 cross20 = Cross(v20, v0p);

	// すべての小三角形のクロス積と法線が同じ方向を向いていたら衝突
	if (Dot(cross01, normal) >= 0.0f && Dot(cross12, normal) >= 0.0f && Dot(cross20, normal) >= 0.0f) {
		return true;
	}

	return false;

}

bool IsCollision(const AABB& aabb1, const AABB& aabb2)
{
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && // x軸
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && // y軸
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) { // z軸
		return true;
	}

	return false;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };
	// 最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));
	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const Sphere& sphere, const AABB& aabb)
{
	// 最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center.x, aabb.min.x, aabb.max.x), std::clamp(sphere.center.y, aabb.min.y, aabb.max.y), std::clamp(sphere.center.z, aabb.min.z, aabb.max.z) };
	// 最近接点と球の中心との距離を求める
	float distance = Length(Subtract(closestPoint, sphere.center));
	// 距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

bool IsCollision(const AABB& aabb, const Segment& segment)
{

	float tNearX = std::min((aabb.min.x - segment.origin.x) / segment.diff.x, (aabb.max.x - segment.origin.x) / segment.diff.x);
	float tNearY = std::min((aabb.min.y - segment.origin.y) / segment.diff.y, (aabb.max.y - segment.origin.y) / segment.diff.y);
	float tNearZ = std::min((aabb.min.z - segment.origin.z) / segment.diff.z, (aabb.max.z - segment.origin.z) / segment.diff.z);
	float tFarX = std::max((aabb.min.x - segment.origin.x) / segment.diff.x, (aabb.max.x - segment.origin.x) / segment.diff.x);
	float tFarY = std::max((aabb.min.y - segment.origin.y) / segment.diff.y, (aabb.max.y - segment.origin.y) / segment.diff.y);
	float tFarZ = std::max((aabb.min.z - segment.origin.z) / segment.diff.z, (aabb.max.z - segment.origin.z) / segment.diff.z);
	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = std::min(std::min(tFarX, tFarY), tFarZ);
	//float segmentLength = Length(Subtract(segment.diff, segment.origin));
	if (tmin < tmax && tmin < 1.0f && tmax > 0.0f) {


		return true;

	}
	if (segment.origin.x > aabb.min.x && segment.origin.x < aabb.max.x) {
		if (segment.origin.y > aabb.min.y && segment.origin.y < aabb.max.y) {
			if (segment.origin.z > aabb.min.z && segment.origin.z < aabb.max.z) {
				return true;
			}
		}
	}
	return false;
}

bool IsCollision(const Segment& segment, const AABB& aabb)
{
	float tNearX = std::min((aabb.min.x - segment.origin.x) / segment.diff.x, (aabb.max.x - segment.origin.x) / segment.diff.x);
	float tNearY = std::min((aabb.min.y - segment.origin.y) / segment.diff.y, (aabb.max.y - segment.origin.y) / segment.diff.y);
	float tNearZ = std::min((aabb.min.z - segment.origin.z) / segment.diff.z, (aabb.max.z - segment.origin.z) / segment.diff.z);
	float tFarX = std::max((aabb.min.x - segment.origin.x) / segment.diff.x, (aabb.max.x - segment.origin.x) / segment.diff.x);
	float tFarY = std::max((aabb.min.y - segment.origin.y) / segment.diff.y, (aabb.max.y - segment.origin.y) / segment.diff.y);
	float tFarZ = std::max((aabb.min.z - segment.origin.z) / segment.diff.z, (aabb.max.z - segment.origin.z) / segment.diff.z);
	// AABBとの衝突点(貫通点)のtが小さい方
	float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
	// AABBとの衝突点(貫通点)のtが大きい方
	float tmax = std::min(std::min(tFarX, tFarY), tFarZ);
	//float segmentLength = Length(Subtract(segment.diff, segment.origin));
	if (tmin < tmax && tmin < 1.0f && tmax > 0.0f) {


		return true;

	}
	if (segment.origin.x > aabb.min.x && segment.origin.x < aabb.max.x) {
		if (segment.origin.y > aabb.min.y && segment.origin.y < aabb.max.y) {
			if (segment.origin.z > aabb.min.z && segment.origin.z < aabb.max.z) {
				return true;
			}
		}
	}
	return false;
}

bool IsCollision(const OBB& obb, const Sphere& sphere)
{
	Matrix4x4 W;
	W.m[0][0] = obb.orientations[0].x;
	W.m[0][1] = obb.orientations[0].y;
	W.m[0][2] = obb.orientations[0].z;
	W.m[0][3] = 0;
	W.m[1][0] = obb.orientations[1].x;
	W.m[1][1] = obb.orientations[1].y;
	W.m[1][2] = obb.orientations[1].z;
	W.m[1][3] = 0;
	W.m[2][0] = obb.orientations[2].x;
	W.m[2][1] = obb.orientations[2].y;
	W.m[2][2] = obb.orientations[2].z;
	W.m[2][3] = 0;
	W.m[3][0] = obb.center.x;
	W.m[3][1] = obb.center.y;
	W.m[3][2] = obb.center.z;
	W.m[3][3] = 1;
	Matrix4x4 WInverse = Inverse(W);
	Vector3 centerInOBBLocalSpace = TransformVector3(sphere.center, WInverse);
	AABB aabbOBBLocal{
		.min{Multiply(-1.0f, obb.size)},
		.max{obb.size }
	};
	Sphere sphereOBBLocal{
		.center{centerInOBBLocalSpace},
		.radius{sphere.radius}
	};

	// ローカル空間で衝突判定
	if (IsCollision(aabbOBBLocal, sphereOBBLocal)) {
		return true;
	}

	return false;
}

bool IsCollision(const Sphere& sphere, const OBB& obb)
{
	Matrix4x4 W;
	W.m[0][0] = obb.orientations[0].x;
	W.m[0][1] = obb.orientations[0].y;
	W.m[0][2] = obb.orientations[0].z;
	W.m[0][3] = 0;
	W.m[1][0] = obb.orientations[1].x;
	W.m[1][1] = obb.orientations[1].y;
	W.m[1][2] = obb.orientations[1].z;
	W.m[1][3] = 0;
	W.m[2][0] = obb.orientations[2].x;
	W.m[2][1] = obb.orientations[2].y;
	W.m[2][2] = obb.orientations[2].z;
	W.m[2][3] = 0;
	W.m[3][0] = obb.center.x;
	W.m[3][1] = obb.center.y;
	W.m[3][2] = obb.center.z;
	W.m[3][3] = 1;
	Matrix4x4 WInverse = Inverse(W);
	Vector3 centerInOBBLocalSpace = TransformVector3(sphere.center, WInverse);
	AABB aabbOBBLocal{
		.min{Multiply(-1.0f, obb.size)},
		.max{obb.size }
	};
	Sphere sphereOBBLocal{
		.center{centerInOBBLocalSpace},
		.radius{sphere.radius}
	};

	// ローカル空間で衝突判定
	if (IsCollision(aabbOBBLocal, sphereOBBLocal)) {
		return true;
	}

	return false;
}

bool IsCollision(const OBB& obb, const Segment& segment)
{
	Matrix4x4 W;
	W.m[0][0] = obb.orientations[0].x;
	W.m[0][1] = obb.orientations[0].y;
	W.m[0][2] = obb.orientations[0].z;
	W.m[0][3] = 0;
	W.m[1][0] = obb.orientations[1].x;
	W.m[1][1] = obb.orientations[1].y;
	W.m[1][2] = obb.orientations[1].z;
	W.m[1][3] = 0;
	W.m[2][0] = obb.orientations[2].x;
	W.m[2][1] = obb.orientations[2].y;
	W.m[2][2] = obb.orientations[2].z;
	W.m[2][3] = 0;
	W.m[3][0] = obb.center.x;
	W.m[3][1] = obb.center.y;
	W.m[3][2] = obb.center.z;
	W.m[3][3] = 1;
	Matrix4x4 WInverse = Inverse(W);
	Vector3 localOrigin = TransformVector3(segment.origin, WInverse);
	Vector3 localEnd = TransformVector3(Add(segment.origin, segment.diff), WInverse);

	AABB localAABB{
		{-obb.size.x, -obb.size.y, -obb.size.z},
		{obb.size.x, obb.size.y, obb.size.z}
	};

	Segment localSegment;
	localSegment.origin = localOrigin;
	localSegment.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localSegment);
}

bool IsCollision(const Segment& segment, const OBB& obb)
{
	Matrix4x4 W;
	W.m[0][0] = obb.orientations[0].x;
	W.m[0][1] = obb.orientations[0].y;
	W.m[0][2] = obb.orientations[0].z;
	W.m[0][3] = 0;
	W.m[1][0] = obb.orientations[1].x;
	W.m[1][1] = obb.orientations[1].y;
	W.m[1][2] = obb.orientations[1].z;
	W.m[1][3] = 0;
	W.m[2][0] = obb.orientations[2].x;
	W.m[2][1] = obb.orientations[2].y;
	W.m[2][2] = obb.orientations[2].z;
	W.m[2][3] = 0;
	W.m[3][0] = obb.center.x;
	W.m[3][1] = obb.center.y;
	W.m[3][2] = obb.center.z;
	W.m[3][3] = 1;
	Matrix4x4 WInverse = Inverse(W);
	Vector3 localOrigin = TransformVector3(segment.origin, WInverse);
	Vector3 localEnd = TransformVector3(Add(segment.origin, segment.diff), WInverse);

	AABB localAABB{
		{-obb.size.x, -obb.size.y, -obb.size.z},
		{obb.size.x, obb.size.y, obb.size.z}
	};

	Segment localSegment;
	localSegment.origin = localOrigin;
	localSegment.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localSegment);
}

bool IsCollision(const Capsule& capsule, const Sphere& sphere)
{
	Vector3 d = Subtract(sphere.center, capsule.segment.origin);
	Vector3 ba = Add(capsule.segment.origin, capsule.segment.diff);

	Vector3 e = Normalize(ba);

	float t = Dot(d, e) / Length(ba);

	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 f = Lerp(capsule.segment.origin, Add(capsule.segment.origin, capsule.segment.diff), t);

	float distance = Length(Subtract(sphere.center, f));

	if (distance < capsule.radius + sphere.radius) {
		return true;
	}

	return false;
}