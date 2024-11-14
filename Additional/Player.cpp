#include "Player.h"

#include "ImGuiManager.h"
#include "ModelManager.h"
#include "RayCast.h"
#include "SceneManager.h"
#include "CommonData.h"

void Player::Initialize(Camera* camera, World* world)
{
	SetHitBoxSize({ 2.0f, 6.0f, 2.0f });
	Object3D::Initialize(world, Collider::Shape::kAABB);
	worldTransform_.rotateType_ = RotationType::Quaternion;
	worldTransform_.translation_.y = 40.0f;
	SetMass(2.0f);
	SetFirictionCombine(FrictionCombine::kAverage);
	SetMiu(2.0f);
	SetBounceCombine(BounceCombine::kMaximum);
	SetBounciness(0.0f);
	/*worldTransform_.Initialize(RotationType::Quaternion);
	worldTransform_.translation_.y = 3.0f;*/
	models_["player"] = std::make_unique<Model>();
	models_["player"].reset(ModelManager::GetInstance()->CreateModel(obj, "Player"));
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
	isActive_ = true;

	line_ = std::make_unique<PrimitiveDrawer>();
	line_.reset(PrimitiveDrawer::Create());
	line_->SetCamera(camera_);

	reticle3D_ = std::make_unique<Model>();
	reticle3D_.reset(ModelManager::GetInstance()->CreateModel(obj, "apex"));
	reticle3D_->SetCamera(camera_);

	onReticle_ = TextureManager::GetInstance()->Load("onReticle.png");
	offReticle_ = TextureManager::GetInstance()->Load("offReticle.png");
	reticle_ = std::make_unique<Sprite>();
	reticle_.reset(Sprite::Create(offReticle_, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));

	reticleWorldTransform_.Initialize();
	reticleWorldTransform_.translation_.x = 3.0f;
	reticleWorldTransform_.parent_ = &worldTransform_;

	apex_ = std::make_unique<Model>();
	apex_.reset(ModelManager::GetInstance()->CreateModel(obj, "apex"));
	apex_->SetCamera(camera_);
	apexWorldTransform_.Initialize();
	apexBody_ = std::make_unique<Body>();
	apexBody_->CreateBody(world, &apexWorldTransform_);

	springJoint_ = std::make_unique<SpringJoint>();
	springJoint_->CreateSpringJoint(this, apexBody_.get());
	springJoint_->EnableSpring(0, false);
	springJoint_->EnableSpring(1, false);
	springJoint_->SetEquilibriumPoint(0, 0.0f);
	springJoint_->SetEquilibriumPoint(1, 0.0f);
	springJoint_->SetStiffness(0, stiffness_);
	springJoint_->SetStiffness(1, stiffness_);
	springJoint_->SetDamping(0, dampar_);
	springJoint_->SetDamping(1, dampar_);


	isWire_ = false;
	isJunp_ = true;
	isFloot_ = true;
	isMoving_ = false;
	isSelect_ = false;


	Input::GetInstance()->GetJoystickState(0, pad_);

	scoreUI_ = std::make_unique<Score>();
	scoreUI_->Initialize();

	fixedJoint_ = std::make_unique<FixedJoint>();
	playerFixedJoint_ = std::make_unique<FixedJoint>();

}

void Player::Update()
{
	prePad_ = pad_;

	Vector3 move = { 0.0f, 0.0f, 0.0f };
	Vector3 reticleMove = { 0.0f, 0.0f, 0.0f };
	// ジョイスティック状態取得
	if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, pad_)) {

			// プレイヤーの動き
			const float threshold = 0.7f;
			bool isMoving = false;
			// 速さ
			float speed = 1.0f;
			if (!isHit_ && !isWire_) {
				speed = 0.1f;
			}

			// 移動量
			move = { (float)pad_.Gamepad.sThumbLX, 0,(float)pad_.Gamepad.sThumbLY };

			if (Length(move) > threshold) {
				isMoving = true;
			}

			if (isMoving) {

				move.Normalize();
				// 移動量に速さを反映
				move = Multiply(speed, move);

				AddForce(move, Body::ForceMode::kImpulse);
			}

			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				if (!isJunp_) {
					isJunp_ = true;
					AddForce({ 0.0f, 28.0f, 0.0f }, Body::ForceMode::kImpulse);
					GetWorld()->TakeJoint(playerFixedJoint_.get());
				}
			}

			// レティクルの動き
			reticleMove = { (float)pad_.Gamepad.sThumbRX, (float)pad_.Gamepad.sThumbRY, 0.0f };
			reticleMove = Multiply(0.8f, reticleMove.Normalize());
			reticleWorldTransform_.translation_ = Add(reticleWorldTransform_.translation_, reticleMove);

			bool isHit = false;
			RayCastHit hit;
			Vector3 direction = Subtract({ reticleWorldTransform_.matWorld_.m[3][0], reticleWorldTransform_.matWorld_.m[3][1], reticleWorldTransform_.matWorld_.m[3][2] }, worldTransform_.translation_);
			float lenght = Length(direction);
			if (lenght > limitLength_) {
				lenght = limitLength_;
			}
			direction.Normalize();
			isHit = RayCast(worldTransform_.translation_, direction, &hit, lenght, GetWorld(), kCollisionAttributePlayer);


			// ワイヤー
			if (CommonData::GetInstance()->scene_ == Scene::kStage) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
					if (!isWire_) {
						if (isHit && hit.collider->GetCollisionAttribute() != kCollisionAttributeCoin) {
							isWire_ = true;
							//GetWorld()->AddJoint(springJoint_.get());
							//point_ = hit.collider->GetTranslation();
							point_ = hit.point;
							apexWorldTransform_.translation_ = hit.point;
							apexBody_->SetMatTranslation(hit.point);
							if (hit.collider->GetCollisionAttribute() == kCollisionAttributeMoveFloor || hit.collider->GetCollisionAttribute() == kCollisionAttributeTrampoline) {
								fixedJoint_->CreateFixedJoint(hit.collider, apexBody_.get());
							}
						}
					}
					else {
						isWire_ = false;
						fixedJoint_->Clear();
						if (apexWorldTransform_.parent_) {
							apexWorldTransform_.parent_ = nullptr;
						}
					}
				}
			}

			if (isHit) {
				reticle_->SetTextureHandle(onReticle_);
			}
			else {
				reticle_->SetTextureHandle(offReticle_);
			}


		}
	}

	if (Length(reticleWorldTransform_.translation_) > limitLength_) {
		Vector3 dir = reticleWorldTransform_.translation_.Normalize();
		reticleWorldTransform_.translation_ = Multiply(limitLength_, dir);
	}

	//camera_->SetTranslate({ GetMatWorldTranslation().x, GetMatWorldTranslation().y, camera_->GetTranslate().z });

	if (!isWire_) {
		apexWorldTransform_.translation_ = worldTransform_.translation_;
		apexBody_->SetTranslation(worldTransform_.translation_);
	}
	else {
		AddForce(Spring(apexWorldTransform_.GetMatWorldTranslation(), GetMatWorldTranslation(), 0.0f, stiffness_, dampar_), Body::ForceMode::kForce);
		AddForce(RubberMovement(GetMatWorldTranslation(), apexWorldTransform_.GetMatWorldTranslation(), limitLength_, stiffness_, dampar_), Body::ForceMode::kForce);
		fixedJoint_->Solve();
	}

	reticleWorldTransform_.UpdateMatrix();
	apexWorldTransform_.UpdateMatrix();
	apexBody_->GetWorldTransform()->UpdateMatrix();

	Vector2 pos = WorldToScreen({ reticleWorldTransform_.matWorld_.m[3][0], reticleWorldTransform_.matWorld_.m[3][1], reticleWorldTransform_.matWorld_.m[3][2] }, camera_->GetViewMatrix(), camera_->GetProjectionMatrix(), 1280.0f, 720.0f);
	reticle_->SetPosition(pos);

	if (worldTransform_.translation_.y < -12.0f) {
		isActive_ = false;
	}

	if (!isMoving_) {
		GetWorld()->TakeJoint(playerFixedJoint_.get());
	}

	isHit_ = false;
	isFloot_ = true;
	isSelect_ = false;
	isMoving_ = false;

	scoreUI_->Update();

	if (std::abs(GetVelocity().x) > limitSpeed_) {
		Vector3 velocity = GetVelocity();
		float pandm = velocity.x / std::abs(velocity.x);
		SetVelocity({ limitSpeed_ * pandm, velocity.y, velocity.z });
	}

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
	ImGui::DragFloat("mass", &mass_);
	ImGui::DragFloat("stiffness", &stiffness_);
	ImGui::DragFloat("damping", &dampar_);
	Vector3 velocity = GetVelocity();
	ImGui::DragFloat3("velocity", &velocity.x);
	if (ImGui::Button("reset")) {
		isActive_ = false;

	}

	ImGui::End();

#endif
}

void Player::Draw()
{
	models_["player"]->Draw(worldTransform_);
	//model_->Draw(worldTransform_);
	//raticle_->Draw(reticleWorldTransform_, TextureManager::GetInstance()->Load("uvChecker.png"));
	//model_->Draw(worldTransform_, TextureManager::GetInstance()->Load("uvChecker.png"));
	if (CommonData::GetInstance()->scene_ == Scene::kStage) {
		reticle3D_->Draw(reticleWorldTransform_, TextureManager::GetInstance()->Load("pink1x1.png"));
		if (isWire_) {
			line_->Draw(worldTransform_.translation_,apexBody_->GetMatWorldTranslation()/* apexWorldTransform_.GetMatWorldTranslation()*/, { 0.0f, 0.0f, 0.0f, 1.0f });
			apex_->Draw(*apexBody_->GetWorldTransform()/*apexWorldTransform_*/, TextureManager::GetInstance()->Load("purple1x1.png"));
		}
	}
#ifdef _DEBUG
	HitBox();
#endif // _DEBUG

}

void Player::DrawUI()
{
	reticle_->Draw();
	scoreUI_->Draw();
}

void Player::OnCollisionEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeTrampoline) {
		AABB aabb = GetAABB();
		AABB other = GetHitBody()->GetAABB();
		if (aabb.min.y >= other.max.y) {
			if (aabb.min.x < other.max.x && aabb.max.x > other.min.x) {
				AddForce({ 0.0f, 30.0f, 0.0f }, Body::ForceMode::kImpulse);
			}

		}
	}

	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeSpike) {
		isActive_ = false;
	}

	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeMoveFloor) {
		AABB aabb = GetAABB();
		AABB other = GetHitBody()->GetAABB();
		if (aabb.min.y >= (other.max.y + other.min.y) / 2.0f) {
			if (aabb.min.x + 1.0f < other.max.x && aabb.max.x + 1.0f > other.min.x) {
				playerFixedJoint_->CreateFixedJoint(GetHitBody(), this);
				GetWorld()->AddJoint(playerFixedJoint_.get());
				isMoving_ = true;
			}

		}
	}


	if (GetNormalVector().y > 0.0f) {
		isJunp_ = false;
	}

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::Text("Hit");
	ImGui::End();

#endif
	isHit_ = true;
}

void Player::OnTriggerEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeGoal) {
		CommonData::GetInstance()->isGoal_ = true;
		SetVelocity({ 0.0f, 0.0f, 0.0f });
	}
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeSelect) {
		isSelect_ = true;

	}
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeCoin) {
		scoreUI_->AddScore(10);
	}
}

void Player::ResetPos(const Vector3& pos)
{
	worldTransform_.translation_ = pos;
	Reset();
	point_ = worldTransform_.translation_;
	apexWorldTransform_.translation_ = worldTransform_.translation_;
	isWire_ = false;
	isActive_ = true;
	scoreUI_->Reset();
}
