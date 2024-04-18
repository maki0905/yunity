#include "Player.h"

#include "ImGuiManager.h"

void Player::Initialize(Camera* camera)
{
	worldTransfrom_.Initialize();

	model_.reset(Model::Create("startBox", "obj"));
	model_->SetCamera(camera);
	camera_ = camera;


	Create(&worldTransfrom_, Type::kAABB, RotationType::Euler, camera);
	Collider::SetMass(1.0f);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);

}

void Player::Update()
{
	float x = worldTransfrom_.translation_.x;
	camera_->SetTranslate(Vector3(x, camera_->GetTranslate().y, camera_->GetTranslate().z));
	if (Input::GetInstance()->PushKey(DIK_A)) {
		worldTransfrom_.translation_.x -= 0.1f;
		//SetVelocity(Vector3{ -0.1f, 0.0f, 0.0f });
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		worldTransfrom_.translation_.x += 0.1f;
		//SetVelocity(Vector3{ 0.1f, 0.0f, 0.0f });
	}
	if (Input::GetInstance()->TriggerKey(DIK_W)) {
		//worldTransfrom_.translation_.x += 0.1f;
		SetVelocity(Vector3{ 0.0f, 10.0f, 0.0f });
	}

	ImGui::Begin("Player");
	ImGui::SliderFloat3("pos", &worldTransfrom_.translation_.x, -100.0f, 100.9f);
	ImGui::End();
	
}

void Player::Draw()
{
	model_->Draw(worldTransfrom_, TextureManager::GetInstance()->Load("Black1x1.png"));
	Collider::HitBox();
}
