#include "Object3D.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

yunity::Object3D::~Object3D()
{
	models_.clear();
}

void yunity::Object3D::Initialize(Model* model, World* world, Collider::Shape shape)
{
	model_ = std::make_unique<Model>();
	model_.reset(model);
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
	CreateBody(world, &worldTransform_, GetMass());
	CreateCollider(&worldTransform_, shape, CameraManager::GetInstance()->GetCamera());
	isHit_ = false;
	texture_ = TextureManager::GetInstance()->Load("uvChecker.png");
}

void yunity::Object3D::Initialize(World* world, Collider::Shape shape)
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	worldTransform_.Initialize();
	worldTransform_.UpdateMatrix();
	CreateBody(world, &worldTransform_, 0.0f);
	CreateCollider(&worldTransform_, shape, camera_);
}

void yunity::Object3D::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Object3D");
	ImGui::PushID(this);
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
	ImGui::PopID();
	ImGui::End();
#endif
	isHit_ = false;
	//worldTransform_.UpdateMatrix();
}

void yunity::Object3D::Draw()
{
	if (model_) {
		model_->Draw(worldTransform_/*,texture_*/);
	}
#ifdef _DEBUG
	Collider::HitBox(camera_);
#endif
}

void yunity::Object3D::OnCollisionEvent()
{
}

void yunity::Object3D::OnTriggerEvent()
{
}



void yunity::Object3D::SetModel(const std::string& modelName, Model* model)
{
	models_[modelName].reset(model);
}

void yunity::Object3D::SetEnableLighting(bool onOff)
{
	if (model_) {
		model_->SetEnableLighting(onOff);
	}
	if (models_.size()) {
		std::vector<std::string> keys;
		keys.reserve(models_.size());

		for (const auto& model : models_) {
			keys.push_back(model.first);
		}

		for (const auto& key : keys) {
			models_[key]->SetEnableLighting(onOff);
		}

	}

}

void yunity::Object3D::SetDirectionalLight(Model::DirectionalLight directionalLight)
{
	
	if (model_) {
		model_->SetDirectionalLight(directionalLight);
		model_->SetEnableLighting(true);
	}
	if (models_.size()) {
		std::vector<std::string> keys;
		keys.reserve(models_.size());

		for (const auto& model : models_) {
			keys.push_back(model.first);
		}

		for (const auto& key : keys) {
			models_[key]->SetDirectionalLight(directionalLight);
			models_[key]->SetEnableLighting(true);
		}


	}
	
}
