#pragma once
#include "Object3D.h"
class MoveFloor : public Object3D
{
	virtual void Update();
	float amplitude_ = 0.1f;
	float time_ = 0.0f;
	float cycle_ = 2.0f;
};

