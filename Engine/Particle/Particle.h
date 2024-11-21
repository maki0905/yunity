#pragma once

#include <memory>
#include <list>

#include "Vector3.h"
#include "Vector4.h"
#include "Transform.h"
#include "Matrix4x4.h"
#include "Camera.h"


struct ParticleForCPU {
	Matrix4x4 world;
	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};
};

struct Particle {

	ParticleForCPU particleForCPU;
	EulerTransform transform = { .scale{1.0f, 1.0f, 1.0f},.rotate{0.0f, 0.0f, 0.0f}, .translate{0.0f, 0.0f, 0.0f} };
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	float lifeTime;
	float currentTime;


	void AddVecocity();
	void AffineMatrix();
	void BillboardMatrix(const yunity::Camera& camera);
};



