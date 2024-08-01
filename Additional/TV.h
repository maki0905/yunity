#pragma once

#include "Object3D.h"

class TV :public Object3D{
public:
	
	void InitializeTexture();
	virtual void Update();
	virtual void Draw();
	virtual void OnTriggerEvent(Body* body);

private:
	float time_;
	bool isActive_;
	bool change_;
	uint32_t textureHandle_[2];
};