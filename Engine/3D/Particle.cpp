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
	transform.translate = Add(transform.translate, velocity);
	Matrix4x4 billboardMatrix = camera.matView;
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
	/*world = MakeAffineMatrix(scale, rotation, translation);
	world = Multiply(world,billboardMatrix);*/
	particleForCPU.world = Multiply(Multiply(MakeScaleMatrix(transform.scale), billboardMatrix), MakeTranslateMatrix(transform.translate));
}