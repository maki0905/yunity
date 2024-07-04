#pragma once

#include <memory>

#include "Camera.h"
#include "Collider.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Object3D.h"
#include "Sprite.h"



class Player : public Object3D
{
public:
	void Initialize(Camera* camera, World* world);

	void Update() override;

	void Draw() override;

	void DrawUI();

	//void OnCollisionEvent(Body* body = nullptr) override;
	virtual void OnCollisionEvent(Body* body);

	void ResetPos(const Vector3& pos);

	bool GetActive() { return isActive_; }

private:
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	//std::unique_ptr<Model> model_;
	//WorldTransform worldTransform_;

	//Camera* camera_;


	float stiffness_;
	float dampar_;
	float mass_;
	float limitLength_;
	
	bool isCrouching_;
	bool isHit_;
	bool isActive_;

	std::unique_ptr<PrimitiveDrawer> line_;
	Vector3 point_;
	bool isWire_;

	bool isJunp_;

	bool isFloot_;

	std::unique_ptr<Model> reticle3D_;
	//WorldTransform reticle3DWorldTransform_;
	std::unique_ptr<Model> apex_;
	WorldTransform apexWorldTransform_;


	std::unique_ptr<Sprite> reticle_;
	WorldTransform reticleWorldTransform_;
	uint32_t onReticle_;
	uint32_t offReticle_;

};

