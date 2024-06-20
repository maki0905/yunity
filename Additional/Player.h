#pragma once

#include <memory>

#include "Camera.h"
#include "Collider.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Body.h"



class Player : public Collider
{
public:
	void Initialize(Camera* camera, World* world);

	void Update();

	void Draw();




private:
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	std::unique_ptr<Model> model_;
	WorldTransform worldTransform_;

	Camera* camera_;

	std::unique_ptr<Body> body_;

	float stiffness_;
	float dampar_;
	float mass_;
	float limitLength_;
	
	bool isCrouching_;

};

