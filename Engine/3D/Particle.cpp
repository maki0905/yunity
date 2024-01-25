#include "Particle.h"

void Particle::RotationMatrix()
{
	transform.translate = Add(transform.translate, velocity);
	world = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
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
	world = Multiply(Multiply(MakeScaleMatrix(transform.scale), billboardMatrix), MakeTranslateMatrix(transform.translate));
}