#include "Player.h"

#include "ImGuiManager.h"
#include "ModelManager.h"
#include "RayCast.h"

void Player::Initialize(Camera* camera, World* world)
{
	SetSize({ 2.5f, 2.5f, 2.5f });
	Object3D::Initialize(world, Collider::Shape::kAABB);
	worldTransform_.rotateType_ = RotationType::Quaternion;
	worldTransform_.translation_.y = 3.0f;
	SetMass(2.0f);
	/*worldTransform_.Initialize(RotationType::Quaternion);
	worldTransform_.translation_.y = 3.0f;*/

	models_["player"] = ModelManager::GetInstance()->CreateModel(obj, "startBox");
	models_["player"]->SetCamera(camera);
	/*model_.reset(ModelManager::GetInstance()->CreateModel(obj, "startBox"));
	model_->SetCamera(camera);*/
	/*model_.reset(ModelManager::GetInstance()->CreateModel(gltf, "human", "sneakWalk", ModelType::kSkin));
	model_->SetCamera(camera);*/
	/*model_.reset(ModelManager::GetInstance()->CreateModel(gltf, "human", "sneakWalk", ModelType::kSkin));
	model_->SetCamera(camera);
	model_->SetAnimation("walk", ModelManager::GetInstance()->GetAnimation(gltf, "human", "walk"));
	model_->SetAnimation("sneakWalk", ModelManager::GetInstance()->GetAnimation(gltf, "human", "sneakWalk"));*/
	camera_ = camera;

	isCrouching_ = false;
	//model_->PlayAnimation("walk", AnimationCommon::kLooping);

	stiffness_ = 1.0f;
	dampar_ = 0.1f;
	mass_ = 1.0f;
	limitLength_ = 15.0f;

	/*CreateBody(world, &worldTransform_, 2.0f);*/

	world->Add(this);

	//CreateCollider(&worldTransform_, Collider::Shape::kAABB, camera_, { 2.5f, 2.5f, 2.5f });
	isHit_ = false;
	SetCollisionAttribute(kCollisionAttributePlayer);
	isActive_ = false;

	line_ = std::make_unique<PrimitiveDrawer>();
	line_.reset(PrimitiveDrawer::Create());
	line_->SetCamera(camera_);

	reticle3D_ = std::make_unique<Model>();
	reticle3D_.reset(ModelManager::GetInstance()->CreateModel(obj, "apex"));
	reticle3D_->SetCamera(camera_);

	onReticle_ = TextureManager::GetInstance()->Load("onReticle.png");
	offReticle_ = TextureManager::GetInstance()->Load("offReticle.png");
	reticle_ = std::make_unique<Sprite>();
	reticle_.reset(Sprite::Create(offReticle_, { 0.0f, 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	reticleWorldTransform_.Initialize();
	reticleWorldTransform_.translation_.x = 3.0f;
	reticleWorldTransform_.parent_ = &worldTransform_;

	apex_ = std::make_unique<Model>();
	apex_.reset(ModelManager::GetInstance()->CreateModel(obj, "apex"));
	apex_->SetCamera(camera_);
	apexWorldTransform_.Initialize();

	isWire_ = false;
	isJunp_ = false;
	isFloot_ = true;

	Input::GetInstance()->GetJoystickState(0, pad_);

}

void Player::Update()
{
	/*Vector3 move = { 0.0f, 0.0f, 0.0f };
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
	}*/

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
	/*move = Multiply(0.3f, move);*/

	// ジョイスティック状態取得
	//if (Input::GetInstance()->IsControllerConnected()) {
	//	if (Input::GetInstance()->GetJoystickState(0, pad_)) {

	//		const float threshold = 0.7f;
	//		bool isMoving = false;

	//		// 速さ
	//		const float speed = 0.3f;

	//		// 移動量
	//		move = { (float)pad_.Gamepad.sThumbLX, 0,0};

	//		if (Length(move) > threshold) {
	//			isMoving = true;
	//		}

	//		if (isMoving) {

	//			move = Normalize(move);
	//			Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, move));
	//			float dot = Dot({ 0.0f, 0.0f, 1.0f }, move);
	//			moveQuaternion = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

	//			// 移動量に速さを反映
	//			move = Multiply(speed, move);
	//		}

	//		if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
	//			isCrouching_ ^= true;
	//		}

	//		if (move.x != 0.0f) {
	//			if (isCrouching_) {
	//				model_->PlayAnimation("sneakWalk", AnimationCommon::kLooping);
	//				model_->StopAnimation("walk");
	//			}
	//			else {
	//				model_->PlayAnimation("walk", AnimationCommon::kLooping);
	//				model_->StopAnimation("sneakWalk");
	//			}
	//		}
	//		else {
	//			model_->StopAnimation();
	//		}

	//	}
	//}




	/*worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.quaternion_ = Slerp(worldTransform_.quaternion_, moveQuaternion, 1.0f);
	worldTransform_.quaternion_ = Normalize(worldTransform_.quaternion_);*/

	//worldTransform_.UpdateMatrix();

	/*Vector3 cameraTranslation = camera_->GetTranslate();
	cameraTranslation = Add(cameraTranslation, move);
	camera_->SetTranslate(cameraTranslation);*/

	prePad_ = pad_;
	/*Vector3 move = { 0.0f, 0.0f, 0.0f };
	Quaternion moveQuaternion = worldTransform_.quaternion_;
	Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, move));
	float dot = Dot({ 0.0f, 0.0f, 1.0f }, move);
	moveQuaternion = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));*/
	//worldTransform_.quaternion_ = Slerp(worldTransform_.quaternion_, moveQuaternion, 1.0f);
	//body_->AddForce(body_->RubberMovement(worldTransform_.translation_, { 0.0f, 0.0f, 0.0f }, limitLength_, stiffness_, dampar_), 0);

	/*if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		AddForce({ 0.0f, 15.0f, 0.0f }, 1);
		
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		worldTransform_.translation_.x += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		worldTransform_.translation_.x -= 0.1f;
	}

	if (worldTransform_.translation_.y < -2.0f) {
		isActive_ = false;
	}*/

	/*if (Input::GetInstance()->PushKey(DIK_W)) {
		worldTransform_.translation_.y += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		worldTransform_.translation_.x += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= 0.1f;
	}

	if (Input::GetInstance()->PushKey(DIK_UP)) {
		reticleWorldTransform_.translation_.y += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		reticleWorldTransform_.translation_.y -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		reticleWorldTransform_.translation_.x += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		reticleWorldTransform_.translation_.x -= 0.1f;
	}*/

	Vector3 move = { 0.0f, 0.0f, 0.0f };
	Vector3 reticleMove = { 0.0f, 0.0f, 0.0f };
	// ジョイスティック状態取得
	if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {

			// プレイヤーの動き
			const float threshold = 0.7f; 
			bool isMoving = false;
			// 速さ
			float speed = 0.3f;
			if (isFloot_) {
				speed = 0.1f;
			}

			// 移動量
			move = { (float)pad_.Gamepad.sThumbLX, 0,0};

			if (Length(move) > threshold) {
				isMoving = true;
			}

			if (isMoving) {

				move.Normalize();
				// 移動量に速さを反映
				move = Multiply(speed, move);

				AddForce(move, 1);
			}

			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				if (!isJunp_) {
					isJunp_ = true;
					AddForce({ 0.0f, 20.0f, 0.0f }, 1);
				}
			}

			// レティクルの動き
			reticleMove = { (float)pad_.Gamepad.sThumbRX, (float)pad_.Gamepad.sThumbRY, 0.0f };
			reticleMove = Multiply(0.8f, reticleMove.Normalize());
			reticleWorldTransform_.translation_ = Add(reticleWorldTransform_.translation_, reticleMove);

			// ワイヤー
			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
				if (!isWire_) {
					Vector3 direction = Subtract({ reticleWorldTransform_.matWorld_.m[3][0], reticleWorldTransform_.matWorld_.m[3][1], reticleWorldTransform_.matWorld_.m[3][2] }, worldTransform_.translation_);
					direction.Normalize();
					RayCastHit hit;
					if (RayCast(worldTransform_.translation_, direction, &hit, limitLength_, GetWorld(), kCollisionAttributePlayer)) {
						isWire_ = true;
						//point_ = hit.collider->GetTranslation();
						point_ = hit.point;
						apexWorldTransform_.translation_ = point_;
					}
					else {
						isWire_ = false;
					}
				}
				else {
					isWire_ = false;
				}
			}


		}
	}

	if (Length(reticleWorldTransform_.translation_) > limitLength_) {
		Vector3 dir = reticleWorldTransform_.translation_.Normalize();
		reticleWorldTransform_.translation_ = Multiply(limitLength_, dir);
	}

	camera_->SetTranslate({worldTransform_.translation_.x, worldTransform_.translation_.y, camera_->GetTranslate().z});

	ImGui::Begin("Player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
	ImGui::DragFloat("mass", &mass_);
	ImGui::DragFloat("stiffness", &stiffness_);
	ImGui::DragFloat("damping", &dampar_);
	if (ImGui::Button("reset")) {
		Reset();
		worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };

	}

	ImGui::End();

	isHit_ = false;

	/*if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		isWire_ ^= true;
		point_ = Add(worldTransform_.translation_, { 5.0f, 0.0f, 0.0f });
	}*/

	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		Vector3 direction = Subtract({ reticleWorldTransform_.matWorld_.m[3][0], reticleWorldTransform_.matWorld_.m[3][1], reticleWorldTransform_.matWorld_.m[3][2] }, worldTransform_.translation_);
		direction.Normalize();
		RayCastHit hit;
		if (RayCast(worldTransform_.translation_, direction, &hit, 10.0f, GetWorld(), kCollisionAttributePlayer)) {
			isWire_ = true;
			//point_ = hit.collider->GetTranslation();
			point_ = hit.point;
			apexWorldTransform_.translation_ = point_;
		}
		else {
			isWire_ = false;
		}

	}

	if (!isWire_) {
		point_ = worldTransform_.translation_;
	}
	else {
		AddForce(RubberMovement(worldTransform_.translation_, point_, limitLength_, stiffness_, dampar_), 0);
	}

	reticleWorldTransform_.UpdateMatrix();
	apexWorldTransform_.UpdateMatrix();

	Vector2 pos = WorldToScreen({ reticleWorldTransform_.matWorld_.m[3][0], reticleWorldTransform_.matWorld_.m[3][1], reticleWorldTransform_.matWorld_.m[3][2] }, camera_->GetViewMatrix(), camera_->GetProjectionMatrix(), 1280.0f, 720.0f);
	reticle_->SetPosition(pos);

	if (worldTransform_.translation_.y < -6.0f) {
		isActive_ = false;
	}

	isFloot_ = true;

}

void Player::Draw()
{
	/*if (RayCast({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, nullptr, 5.0f, GetWorld())) {
		model_->Draw(worldTransform_, TextureManager::GetInstance()->Load("uvChecker.png"));
	}
	else {
		model_->Draw(worldTransform_);
	}*/
	models_["player"]->Draw(worldTransform_);
	//model_->Draw(worldTransform_);
	//raticle_->Draw(reticleWorldTransform_, TextureManager::GetInstance()->Load("uvChecker.png"));
	//model_->Draw(worldTransform_, TextureManager::GetInstance()->Load("uvChecker.png"));
	HitBox();
	line_->Draw(worldTransform_.translation_, point_, { 0.0f, 0.0f, 0.0f, 1.0f });
	reticle3D_->Draw(reticleWorldTransform_, TextureManager::GetInstance()->Load("pink1x1.png"));
	if (isWire_) {
		apex_->Draw(apexWorldTransform_, TextureManager::GetInstance()->Load("purple1x1.png"));
	}
	//reticle_->Draw();
	//Collider::HitBox();
}

void Player::DrawUI()
{
	//reticle_->Draw();
}

void Player::OnCollisionEvent(Body* body)
{
	if (body->GetCollisionAttribute() == kCollisionAttributeTrampoline) {
		AABB aabb = GetAABB();
		AABB other = body->GetAABB();
		if (aabb.min.y >= other.max.y) {
			if (aabb.min.x < other.max.x && aabb.max.x > other.min.x) {
				AddForce({ 0.0f, 30.0f, 0.0f }, 1);
			}

		}
	}

	if (GetVertical().y > 0.0f) {
		isJunp_ = false;
		isFloot_ = false;
	}

	ImGui::Begin("Player");
	ImGui::Text("Hit");
	ImGui::End();
	isHit_ = true;
}

void Player::ResetPos(const Vector3& pos)
{
	worldTransform_.translation_ = pos;
	Reset();
	point_ = worldTransform_.translation_;
	isWire_ = false;
	isActive_ = true;
}
