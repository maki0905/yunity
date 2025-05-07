#include "ClearScene.h"

#include "ImGuiManager.h"
#include "ModelManager.h"
#include "LevelEditor.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

void ClearScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();

	world_ = std::make_unique<yunity::World>();
	world_->Initialize({0.0f, -9.0f, 0.0f});

	object_ = std::make_unique<yunity::Object3D>();
	object_->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), 0.0f, yunity::ShapeType::kBox);
	object_->SetCamera(camera_);
	object_->SetScale({ 10.0f, 1.0f, 10.0f });
	object_->SetBoxSize({ 10.0f, 1.0f, 10.0f });

	world_->AddObject(object_.get());

	for(int i = 0; i < 1; i++){
		std::unique_ptr<yunity::Object3D> object = std::make_unique<yunity::Object3D>();
		object->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), 1.0f, yunity::ShapeType::kBox);
		object->SetCamera(camera_);
		object->SetMass(0.0f);
		object->SetDrag(0.005f);
		object->SetAngularDrag(0.005f);
		object->SetInertiaMoment(0.0f);
		object->SetPosition({ 0.0f, 5.0f, 0.0f });
		object->SetTexture(yunity::TextureManager::GetInstance()->Load("uvChecker.png"));
		object->SetBounciness(1.0f);
		object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		objects_[i] = std::move(object);

		world_->AddObject(objects_[i].get());
	}

	onoff_ = false;

}

void ClearScene::Update()
{
	if (onoff_) {
		world_->Solve();
	}

	ImGui::Begin("ClearScene");
	if (ImGui::Button("onoff")) {
		onoff_ ^= true;
	}

	if (ImGui::Button("SetMass_1.0")) {
		objects_[0]->SetMass(10.0f);
		objects_[0]->SetInertiaMoment(1.0f);
	}

	if (ImGui::Button("SetMass_0.0")) {
		objects_[0]->SetMass(0.0f);
		objects_[0]->SetInertiaMoment(0.0f);
	}

	Vector3 position = objects_[0]->GetMatWorldTranslation();
	ImGui::DragFloat3("position", &position.x);
	objects_[0]->SetPosition(position);

	ImGui::End();
	
}

void ClearScene::DrawBack()
{
}

void ClearScene::Draw3D()
{
	object_->Draw();
	object_->HitBox();
	for (int i = 0; i < 1; i++) {
		objects_[i]->Draw();
		objects_[i]->HitBox();
	}
}

void ClearScene::DrawFront()
{
}
