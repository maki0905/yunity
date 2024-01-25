#pragma once

#include <memory>
#include <list>

#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"
#include "Matrix4x4.h"
#include "Camera.h"

struct Particle {
	Matrix4x4 world;
	Transform transform;
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	Vector4 color;
	float lifeTime;
	float currentTime;

	bool isBillboard;

	void RotationMatrix();
	void BillboardMatrix(const Camera& camera);
};


