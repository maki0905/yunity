#pragma once

#include <memory>

#include "Camera.h"
#include "Collider.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Body.h"



class Player : public Body
{
public:
	void Initialize(Camera* camera, World* world);

	void Update();

	void Draw();

	void Event(Body* body = nullptr) override;

	void ResetPos(const Vector3& pos);

	bool GetActive() { return isActive_; }

private:
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	std::unique_ptr<Model> model_;
	WorldTransform worldTransform_;

	Camera* camera_;

	float stiffness_;
	float dampar_;
	float mass_;
	float limitLength_;
	
	bool isCrouching_;
	bool isHit_;
	bool isActive_;

};

