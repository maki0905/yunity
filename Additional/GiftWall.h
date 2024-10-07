#pragma once
#include "Object3D.h"
class GiftWall : public Object3D{
public:
	virtual void Update();
private:
	float rotateSpeed_ = 0.5f;
};