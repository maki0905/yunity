#include "Player.h"

#include "ImGuiManager.h"
#include "ModelManager.h"

void Player::Initialize(Camera* camera, World* world)
{
	worldTransform_.Initialize(RotationType::Quaternion);
	worldTransform_.scale_ = { 10.0f, 10.0f, 10.0f };
	worldTransform_.quaternion_ = { 0.0f, 0.0f, 0.0f, 1.0f };

	model_.reset(ModelManager::GetInstance()->CreateModel(gltf, "human", "sneakWalk", ModelType::kSkin));
	model_->SetCamera(camera);
	model_->SetAnimation("walk", ModelManager::GetInstance()->GetAnimation(gltf, "human", "walk"));
	model_->SetAnimation("sneakWalk", ModelManager::GetInstance()->GetAnimation(gltf, "human", "sneakWalk"));
	camera_ = camera;

	isCrouching_ = false;
	model_->PlayAnimation("walk", AnimationCommon::kLooping);

	body_ = std::make_unique<Body>();
	body_->CreateBody(world, &worldTransform_, 1.0f);
	world->Add(body_.get());

	stiffness_ = 1.0f;
	dampar_ = 0.1f;
	mass_ = 1.0f;
	limitLength_ = 10.0f;
}

void Player::Update()
{
	Vector3 move = { 0.0f, 0.0f, 0.0f };
	Quaternion moveQuaternion = worldTransform_.quaternion_;

	if (Input::GetInstance()->PushKey(DIK_A)) {
		move.x = -1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		move.x = 1.0f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_P)) {
		if (!isCrouching_) {
			model_->TransitionAnimation("walk", "sneakWalk", 0.5f);
			model_->PlayAnimation("sneakWalk", AnimationCommon::kLooping);
		}
		else {
			model_->TransitionAnimation("sneakWalk", "walk", 0.5f);
			model_->PlayAnimation("walk", AnimationCommon::kLooping);
		}
		isCrouching_ ^= true;
	}

	move = Normalize(move);

	if (move.x != 0.0f) {
		Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, move));
		float dot = Dot({ 0.0f, 0.0f, 1.0f }, move);
		moveQuaternion = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
	}

	/*if (move.x != 0.0f) {
		if (isCrouching_) {
			model_->PlayAnimation("sneakWalk", AnimationCommon::kLooping);
			model_->StopAnimation("walk");
		}
		else {
			model_->PlayAnimation("walk", AnimationCommon::kLooping);
			model_->StopAnimation("sneakWalk");
		}
	}
	else {
		model_->StopAnimation();
	}*/

	// 移動量に速さを反映
	move = Multiply(0.3f, move);

	// ジョイスティック状態取得
	if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {

			const float threshold = 0.7f;
			bool isMoving = false;

			// 速さ
			const float speed = 0.3f;

			// 移動量
			move = { (float)pad_.Gamepad.sThumbLX, 0,0};

			if (Length(move) > threshold) {
				isMoving = true;
			}

			if (isMoving) {

				move = Normalize(move);
				Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, move));
				float dot = Dot({ 0.0f, 0.0f, 1.0f }, move);
				moveQuaternion = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

				// 移動量に速さを反映
				move = Multiply(speed, move);
			}

			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				isCrouching_ ^= true;
			}

			if (move.x != 0.0f) {
				if (isCrouching_) {
					model_->PlayAnimation("sneakWalk", AnimationCommon::kLooping);
					model_->StopAnimation("walk");
				}
				else {
					model_->PlayAnimation("walk", AnimationCommon::kLooping);
					model_->StopAnimation("sneakWalk");
				}
			}
			else {
				model_->StopAnimation();
			}

		}
	}



	
	/*worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.quaternion_ = Slerp(worldTransform_.quaternion_, moveQuaternion, 1.0f);
	worldTransform_.quaternion_ = Normalize(worldTransform_.quaternion_);*/

	//worldTransform_.UpdateMatrix();

	Vector3 cameraTranslation = camera_->GetTranslate();
	cameraTranslation = Add(cameraTranslation, move);
	camera_->SetTranslate(cameraTranslation);

	prePad_ = pad_;
	/*Vector3 move = { 0.0f, 0.0f, 0.0f };
	Quaternion moveQuaternion = worldTransform_.quaternion_;
	Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, move));
	float dot = Dot({ 0.0f, 0.0f, 1.0f }, move);
	moveQuaternion = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));*/
	//worldTransform_.quaternion_ = Slerp(worldTransform_.quaternion_, moveQuaternion, 1.0f);
	body_->SetMass(mass_);
	body_->AddForce(body_->RubberMovement(worldTransform_.translation_, { 0.0f, 0.0f, 0.0f }, limitLength_, stiffness_, dampar_), 0);

	ImGui::Begin("Player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
	ImGui::DragFloat("mass", &mass_);
	ImGui::DragFloat("stiffness", &stiffness_);
	ImGui::DragFloat("damping", &dampar_);
	if (ImGui::Button("reset")) {
		body_->Reset();
		worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };

	}

	ImGui::End();

}

void Player::Draw()
{
	model_->Draw(worldTransform_);
	//Collider::HitBox();
}
