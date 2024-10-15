#pragma once
#include "Object3D.h"

class Coin : public Object3D{
public:
	virtual void Update();
	virtual void Draw();
	void OnCollisionEvent() override;
	void OnTriggerEvent() override;
	bool GetIsActive() { return isActive_; }
	void SetIsActive(bool isActive) { isActive_ = isActive; }
private:
	bool isActive_ = true;
};