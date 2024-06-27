#pragma once

#include "Model.h"
#include "WorldTransform.h"

#include "Body.h"

class Object3D : public Body{
public:
	void Create(Model* model, World* world, Collider::Shape shape);
	void Update();
	void Draw();

	void Event(Body* body = nullptr) override;

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }
	void SetSize(const Vector3& size) { size_ = size; }
	void SetCenter(const Vector3& center) { center_ = center; }

	Vector3 GetSize() { return size_; }
	Vector3 GetCenter() { return center_; }

	Model* GetModel() { return model_.get(); }

private:
	std::unique_ptr<Model>model_;
	WorldTransform worldTransform_;
	Vector3 size_;
	Vector3 center_;
	bool isHit_;
	uint32_t texture_;
};