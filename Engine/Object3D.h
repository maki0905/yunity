#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Body.h"

class Object3D : public Body{
public:
	~Object3D() {
		/*for (auto& pair : models_) {
			delete pair.second;
		}*/
		models_.clear();
	}
	void Initialize(Model* model, World* world, Collider::Shape shape);
	void Initialize(World* world, Collider::Shape shape);
	//void Initialize(std::unique_ptr<Model> model, World* world, Collider::Shape shape);
	virtual void Update();
	virtual void Draw();

	// 衝突時に呼ばれる固有の処理
	virtual void OnCollisionEvent(Body* body);
	virtual void OnTriggerEvent(Body* body);

	void SetPosition(const Vector3& position) { worldTransform_.translation_ = position; }
	void SetRotation(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }
	void SetSize(const Vector3& size) { size_ = size; }
	void SetCenter(const Vector3& center) { center_ = center; }
	void SetCamera(Camera* camera) { 
		camera_ = camera; 
		model_->SetCamera(camera);
	}
	Vector3 GetSize() { return size_; }
	Vector3 GetCenter() { return center_; }
	Vector3 GetTranslation() { return worldTransform_.translation_; }

	Model* GetModel() { return model_.get(); }
	Model* GetModel(const std::string& modelName) { return models_[modelName].get(); }
	void SetModel(const std::string& modelName, Model* model);

protected:
	WorldTransform worldTransform_;
	std::unordered_map<std::string, std::unique_ptr<Model>> models_;
	Camera* camera_;
private:
	//Camera* camera_;
	std::unique_ptr<Model> model_;
	//Model* model_;
	//std::map<std::string, Model*> models_;
	//WorldTransform worldTransform_;
	Vector3 size_;
	Vector3 center_;
	bool isHit_;
	uint32_t texture_;
};