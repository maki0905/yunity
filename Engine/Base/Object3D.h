#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Body.h"

class Object3D : public Body{
public:
	~Object3D();
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
	void SetCamera(Camera* camera) { 
		camera_ = camera; 
		if (model_) {
			model_->SetCamera(camera);
		}
	}
	Vector3 GetTranslation() { return worldTransform_.translation_; }
	Model* GetModel() { return model_.get(); }
	Model* GetModel(const std::string& modelName) { return models_[modelName].get(); }
	void SetModel(const std::string& modelName, Model* model);
	void SetEnableLighting(bool onOff);
	void SetDirectionalLight(Model::DirectionalLight directionalLight);

	void SetFileName(const std::string fileName) { fileName_ = fileName; }

protected:
	WorldTransform worldTransform_;
	std::unordered_map<std::string, std::unique_ptr<Model>> models_;
	std::unique_ptr<Model> model_;
	Camera* camera_;
private:
	//Camera* camera_;
	//Model* model_;
	//std::map<std::string, Model*> models_;
	//WorldTransform worldTransform_;
	bool isHit_;
	uint32_t texture_;
	std::string fileName_;
};