#pragma once
#include "Object3D.h"
class MoveFloor : public Object3D
{
	virtual void Update();
	float amplitude_ = 0.05f;
	float time_ = 0.0f;
	float cycle_ = 5.0f;
};

