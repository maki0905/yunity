#pragma once

#include "Object3D.h"
#include "SpringJoint.h"

class Cloth{
public:
	void Initialize(World* world);
	void Update();
	void Draw();


private:
	std::array<std::unique_ptr<Object3D>, 16> points_;
	std::array<std::unique_ptr<PrimitiveDrawer>, 24> lines_;
	std::array<std::unique_ptr<SpringJoint>, 24> joints_;
	float stiffness_ = 1.0f;
	float dampar_ = 0.5f;


};