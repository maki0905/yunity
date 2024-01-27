#include "Particle.h"

void Particle::AddVecocity()
{
	transform.translate = Add(transform.translate, velocity);
}

void Particle::AffineMatrix()
{
	particleForCPU.world = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
}

void Particle::BillboardMatrix(const Camera& camera)
{
	//Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	//Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix,camera.matView);
	Matrix4x4 billboardMatrix = Inverse(camera.matView);
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
	particleForCPU.world = Multiply(Multiply(Multiply(MakeScaleMatrix(transform.scale), MakeRotateXYZMatrix(transform.rotate)), billboardMatrix), MakeTranslateMatrix(transform.translate));
}