#include "ClearScene.h"

#include "ImGuiManager.h"
#include "ModelManager.h"
#include "LevelEditor.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "RenderTexture.h"

void ClearScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();

	/*yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, true);
	yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kOutline, true);*/

	world_ = std::make_unique<yunity::World>();
	world_->Initialize({0.0f, -9.0f, 0.0f});

	object_ = std::make_unique<yunity::Object3D>();
	object_->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), 0.0f, yunity::ShapeType::kBox);
	object_->SetCamera(camera_);
	object_->SetScale({ 10.0f, 1.0f, 10.0f });
	object_->SetBoxSize({ 10.0f, 1.0f, 10.0f });
	object_->GetWorldTransform()->UpdateMatrix();

	world_->AddObject(object_.get());

	for(int i = 0; i < 1; i++){
		std::unique_ptr<yunity::Object3D> object = std::make_unique<yunity::Object3D>();
		object->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), 1.0f, yunity::ShapeType::kBox);
		object->SetCamera(camera_);
		object->SetMass(0.0f);
		object->SetDrag(0.005f);
		object->SetAngularDrag(20.0f);
		object->SetPosition({ 0.0f, 5.0f, 0.0f });
		object->SetTexture(yunity::TextureManager::GetInstance()->Load("uvChecker.png"));
		object->SetBounciness(0.5f);
		object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		object->GetWorldTransform()->UpdateMatrix();
		objects_[i] = std::move(object);

		world_->AddObject(objects_[i].get());
	}

	onoff_ = false;

}

void ClearScene::Update()
{
	ImGui::Begin("ClearScene");
	if (ImGui::Button("onoff")) {
		onoff_ ^= true;
	}
	ImGui::Checkbox("FrameMode", &frameMode_);

	if (ImGui::Button("SetMass_1.0")) {
		objects_[0]->SetMass(1.0f);
		objects_[0]->SetInertiaMoment(1.0f);
	}

	if (ImGui::Button("SetMass_0.0")) {
		objects_[0]->SetMass(0.0f);
		objects_[0]->SetInertiaMoment(0.0f);
	}

	if (ImGui::Button("Reset")) {
		objects_[0]->SetPosition(Vector3(0.0f, 5.0f, 0.0f));
		objects_[0]->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
		objects_[0]->SetMass(0.0f);
		objects_[0]->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
		objects_[0]->SetAngularVelocity(Vector3(0.0f, 0.0f, 0.0f));
		objects_[0]->GetWorldTransform()->UpdateMatrix();
	}

	float mass = objects_[0]->GetMass();
	ImGui::DragFloat("Mass", &mass);
	objects_[0]->SetMass(mass);

	Vector3 position = objects_[0]->GetMatWorldTranslation();
	ImGui::DragFloat3("position", &position.x);
	objects_[0]->SetPosition(position);

	Vector3 rotation = objects_[0]->GetWorldTransform()->rotation_;
	ImGui::DragFloat3("rotation", &rotation.x, 0.01f);
	objects_[0]->SetRotation(rotation);


	float angularDrag = objects_[0]->GetAngularDrag();
	ImGui::DragFloat("AngularDrag", &angularDrag);
	objects_[0]->SetAngularDrag(angularDrag);

	float bounciness = objects_[0]->GetBounciness();
	ImGui::DragFloat("Bounciness", &bounciness, 0.1f);
	objects_[0]->SetBounciness(bounciness);

	ImGui::End();

	
	ImGui::Begin("Parameter");
	Vector3 velocity = objects_[0]->GetVelocity();
	ImGui::DragFloat3("Velocity", &velocity.x);

	Vector3 angularVelocity = objects_[0]->GetAngularVelocity();
	ImGui::DragFloat3("AngularVelocity", &angularVelocity.x);

	ImGui::End();

	if (frameMode_) {
		if (!yunity::Input::GetInstance()->TriggerKey(DIK_P)) {
			return;
		}
	}

	if (onoff_) {
		world_->Solve();
	}
	
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
