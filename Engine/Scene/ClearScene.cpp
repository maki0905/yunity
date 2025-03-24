#include "ClearScene.h"

#include "ImGuiManager.h"
#include "ModelManager.h"
#include "LevelEditor.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "Object3D.h"
#include "Collider.h"

void ClearScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	debugCamera_ = std::make_unique<yunity::DebugCamera>();

	world_ = std::make_unique<yunity::World>();
	world_->Initialize({ 0.0f, -9.0f, 0.0f });

	object_ = std::make_unique<yunity::Object3D>();
	object_->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Grasslands"), world_.get(), yunity::Collider::Shape::kAABB);
	object_->SetCamera(camera_);
	object_->SetScale({ 40.0f, 1.0f, 10.0f });
	object_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	world_->Add(object_.get());

	for (int i = 0; i < 2; i++) {
		std::unique_ptr<yunity::Object3D> object = std::make_unique<yunity::Object3D>();
		object->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(),yunity::Collider::Shape::kAABB);
		object->SetCamera(camera_);
		object->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		object->SetMass(0.0f);
		object->SetDrag(0.0f);
		object->SetAngularDrag(0.0f);
		object->SetInertiaMoment(0.0f);
		object->SetPosition({-3.0f + 6.0f * float(i), 5.0f, 0.0f});
		object->GetWorldTransform()->UpdateMatrix();
		object->SetBounciness(0.0f);
		object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		objects_[i] = std::move(object);

		world_->Add(objects_[i].get());
	}

	for (int i = 0; i < 5; i++) {
		std::unique_ptr<yunity::SpringJoint> joint = std::make_unique<yunity::SpringJoint>();
		joint->CreateSpringJoint(objects_[0].get(), objects_[1].get());
	}

	force_ = { 0.0f, 0.0f, 0.0f };
	velocity_ = { 0.0f, 0.0f, 0.0f };

	onoff_ = false;

}

void ClearScene::Update()
{




	ImGui::Begin("ClearScene");
	if (ImGui::Button("onoff")) {
		onoff_ ^= true;
	}
	if (ImGui::Button("Mass")) {
		for (int i = 0; i < 2; i++) {
			objects_[i]->SetMass(1.0f);
		}
	}

	ImGui::DragFloat3("Velocity", &velocity_.x);

	if (ImGui::Button("ResetVelocity")) {
		for (int i = 0; i < 2; i++) {
			objects_[i]->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}
	}


	ImGui::End();


	for (int i = 0; i < 2; i++) {
		std::string name = "Obj" + std::to_string(i);
		ImGui::Begin(name.c_str());

		Vector3 position = objects_[i]->GetMatWorldTranslation();
		ImGui::DragFloat3("position", &position.x);
		objects_[i]->SetPosition(position);

		float mass = objects_[i]->GetMass();
		ImGui::DragFloat("Mass", &mass);
		objects_[i]->SetMass(mass);

		Vector3 velocity = objects_[i]->GetVelocity();
		ImGui::DragFloat3("Velocity", &velocity.x);
		objects_[i]->SetVelocity(velocity);


		if (ImGui::Button("SetVelocity")) {
			objects_[i]->SetVelocity(velocity_);
		}
		if (ImGui::Button("ResetVelocity")) {
			objects_[i]->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}

		float bounciness_ = objects_[i]->GetBounciness();
		ImGui::DragFloat("Bounciness", &bounciness_);
		objects_[i]->SetBounciness(bounciness_);

		if (ImGui::Button("BounceCombine::Average")) {
			objects_[i]->SetBounceCombine(yunity::Body::BounceCombine::kAverage);
		}
		if (ImGui::Button("BounceCombine::Minimum")) {
			objects_[i]->SetBounceCombine(yunity::Body::BounceCombine::kMinimum);
		}
		if (ImGui::Button("BounceCombine::Maximum")) {
			objects_[i]->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		}
		if (ImGui::Button("BounceCombine::Multiply")) {
			objects_[i]->SetBounceCombine(yunity::Body::BounceCombine::kMultiply);
		}


		float miu_ = objects_[i]->GetMiu();
		ImGui::DragFloat("Miu", &miu_);
		objects_[i]->SetMiu(miu_);

		if (ImGui::Button("FrictionCombine::Average")) {
			objects_[i]->SetFirictionCombine(yunity::Body::FrictionCombine::kAverage);
		}
		if (ImGui::Button("FrictionCombine::Minimum")) {
			objects_[i]->SetFirictionCombine(yunity::Body::FrictionCombine::kMinimum);
		}
		if (ImGui::Button("FrictionCombine::Maximum")) {
			objects_[i]->SetFirictionCombine(yunity::Body::FrictionCombine::kMaximum);
		}
		if (ImGui::Button("FrictionCombine::Multiply")) {
			objects_[i]->SetFirictionCombine(yunity::Body::FrictionCombine::kMultiply);
		}
		ImGui::End();
	}

	if (onoff_) {
		world_->Solve();
	}

#ifdef _DEBUG
	if (yunity::Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
	}

	if (isDebug_) {
		debugCamera_->Update(camera_);
	}
	camera_->Update();


#endif

}

void ClearScene::DrawBack()
{
}

void ClearScene::Draw3D()
{
	object_->Draw();
	for (int i = 0; i < 2; i++) {
		objects_[i]->Draw();
		objects_[i]->HitBox();
	}
}

void ClearScene::DrawFront()
{
}
