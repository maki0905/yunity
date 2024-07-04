#pragma once
#include "Object3D.h"

class StartObject : public Object3D {
public:
	Vector3 GetTranslation() { Object3D::GetWorldTransform().translation_; }
	//void OnCollisionEvent() override;
private:


};