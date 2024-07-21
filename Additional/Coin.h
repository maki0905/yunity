#pragma once
#include "Object3D.h"

class Coin : public Object3D{
public:
	void Initialize();
	virtual void OnCollisionEvent(Body* body);

private:
	bool isActive_;
};