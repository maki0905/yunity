#include "Object3D.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

//void Object3D::Create(Model* model, World* world, Collider::Shape shape)
//{
//	model_ = std::make_unique<Model>();
//	model_.reset(model);
//	worldTransform_.Initialize();
//	CreateBody(world, &worldTransform_, 0.0f);
//	CreateCollider(&worldTransform_, shape, model_->GetCamera(), size_);
//
//	if (model_->GetModelName() == "Trampoline") {
//		SetCollisionAttribute(kCollisionAttributeTrampoline);
//	}
//	else {
//		SetCollisionAttribute(kCollisionAttributeFloor);
//	}
//	isHit_ = false;
//	texture_ = TextureManager::GetInstance()->Load("uvChecker.png");
//}

void Object3D::Initialize(Model* model, World* world, Collider::Shape shape)
{
	model_ = std::make_unique<Model>();
	model_.reset(model);
	worldTransform_.Initialize();
	CreateBody(world, &worldTransform_, 0.0f);
	CreateCollider(&worldTransform_, shape, model_->GetCamera(), size_);

	if (model_->GetModelName() == "Trampoline") {
		SetCollisionAttribute(kCollisionAttributeTrampoline);
	}
	else if (model_->GetModelName() == "endBox") {
		SetCollisionAttribute(kCollisionAttributeGoal);
	}
	else {
		SetCollisionAttribute(kCollisionAttributeFloor);
	}
	isHit_ = false;
	texture_ = TextureManager::GetInstance()->Load("uvChecker.png");
}

//void Object3D::Initialize(std::unique_ptr<Model> model, World* world, Collider::Shape shape)
//{
//	model_ = std::make_unique<Model>();
//	model_ = std::move(model);
//	worldTransform_.Initialize();
//	CreateBody(world, &worldTransform_, 0.0f);
//	CreateCollider(&worldTransform_, shape, model_->GetCamera(), size_);
//
//	if (model_->GetModelName() == "Trampoline") {
//		SetCollisionAttribute(kCollisionAttributeTrampoline);
//	}
//	else if (model_->GetModelName() == "endBox") {
//		SetCollisionAttribute(kCollisionAttributeGoal);
//	}
//	else {
//		SetCollisionAttribute(kCollisionAttributeFloor);
//	}
//	isHit_ = false;
//	texture_ = TextureManager::GetInstance()->Load("uvChecker.png");
//}

void Object3D::Initialize(World* world, Collider::Shape shape)
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	worldTransform_.Initialize();
	CreateBody(world, &worldTransform_, 0.0f);
	CreateCollider(&worldTransform_, shape, camera_, size_);
}

void Object3D::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Object3D");
	ImGui::PushID(this);
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
	ImGui::PopID();
	ImGui::End();
#endif
	isHit_ = false;
	worldTransform_.UpdateMatrix();
}

void Object3D::Draw()
{
	/*if (isHit_) {
		model_->Draw(worldTransform_, texture_);
	}
	else {
		model_->Draw(worldTransform_);
	}*/
	/*if (model_->GetModelName() == "Trampoline") {
		model_->Draw(worldTransform_, TextureManager::GetInstance()->Load("trampoline.png"));
	}
	else {
		model_->Draw(worldTransform_);
	}*/
	//model_->Draw(worldTransform_, TextureManager::GetInstance()->Load("trampoline.png"));
	model_->Draw(worldTransform_/*,texture_*/);
	Collider::HitBox(camera_);
}

void Object3D::OnCollisionEvent(Body* body)
{
}

void Object3D::OnTriggerEvent(Body* body)
{
}

void Object3D::SetModel(const std::string& modelName, Model* model)
{
	models_[modelName].reset(model);
}
