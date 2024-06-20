#pragma once

#include "Model.h"
#include "WorldTransform.h"

class Object3D {
public:
	void Create(Model* model);
	void Update();
	void Draw();
	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }
private:
	std::unique_ptr<Model>model_;
	WorldTransform worldTransform_;
};