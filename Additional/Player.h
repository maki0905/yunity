#pragma once

#include <memory>

#include "Input.h"
#include "Collider.h"
#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"


class Player : public Collider
{
public:
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	Vector3 GetTranslation() { return worldTransfrom_.translation_; }

	void SetTranslation(const Vector3 translation) { worldTransfrom_.translation_ = translation; }



private:
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	std::unique_ptr<Model> model_;

	WorldTransform worldTransfrom_;

	Camera* camera_;
	


};

