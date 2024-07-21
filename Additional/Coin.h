#pragma once
#include "Object3D.h"

class Coin : public Object3D{
public:

	virtual void OnCollisionEvent(Body* body);
	virtual void OnTriggerEvent(Body* body);
	bool GetIsActive() { return isActive_; }
	void SetIsActive(bool isActive) { isActive_ = isActive; }
private:
	bool isActive_;
};