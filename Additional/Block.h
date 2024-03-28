#pragma once

#include "Collider.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Model.h"

class Block :public Collider
{
public:
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	WorldTransform GetWordTransform() { return worldTransfrom_; }
	void SetTranslate(const Vector3& translate) { worldTransfrom_.translation_ = translate; }
	void SetScale(const Vector3& scale) { worldTransfrom_.scale_ = scale; }
	void SetRotation(const Vector3& rotation) { worldTransfrom_.rotation_ = rotation; }

private:
	std::unique_ptr<Model> model_;

	WorldTransform worldTransfrom_;
	

};

