#include "Player.h"

#include "ImGuiManager.h"
#include "ModelManager.h"
#include "RayCast.h"
#include "SceneManager.h"
#include "CommonData.h"
#include "Sprite.h"
#include "Tradition.h"
#include "EngineTimeStep.h"
#include "RenderTexture.h"

void Player::Initialize(yunity::Camera* camera, yunity::World* world)
{
	SetHitBoxSize({ 2.0f, 6.0f, 2.0f });
	Object3D::Initialize(world, yunity::Collider::Shape::kAABB);
	worldTransform_.rotateType_ = yunity::RotationType::Quaternion;
	worldTransform_.translation_.y = 40.0f;
	SetMass(2.0f);
	SetFirictionCombine(FrictionCombine::kAverage);
	SetMiu(2.0f);
	SetBounceCombine(BounceCombine::kMaximum);
	SetBounciness(0.0f);
	SetFixedMove(2, true);
	SetDrag(0.0f);

	models_["player"] = std::make_unique<yunity::Model>();
	models_["player"].reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "Player"));
	models_["player"]->SetCamera(camera);

	camera_ = camera;

	isCrouching_ = false;

	world->Add(this);

	isHit_ = false;
	SetCollisionAttribute(kCollisionAttributePlayer);
	isActive_ = true;

	line_ = std::make_unique<yunity::PrimitiveDrawer>();
	line_.reset(yunity::PrimitiveDrawer::Create());
	line_->SetCamera(camera_);

	reticle3D_ = std::make_unique<yunity::Model>();
	reticle3D_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "apex"));
	reticle3D_->SetCamera(camera_);

	onReticle_ = yunity::TextureManager::GetInstance()->Load("onReticle.png");
	offReticle_ = yunity::TextureManager::GetInstance()->Load("offReticle.png");
	reticle_ = std::make_unique<yunity::Sprite>();
	reticle_.reset(yunity::Sprite::Create(offReticle_, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	landingPoint_ = std::make_unique<yunity::Sprite>();
	landingPoint_.reset(yunity::Sprite::Create(onReticle_, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	isHitRay_ = false;

	reticleWorldTransform_.Initialize();
	reticleWorldTransform_.translation_.x = 3.0f;
	reticleWorldTransform_.parent_ = &worldTransform_;
	landingPointWorldTrans_.Initialize();
	


	apex_ = std::make_unique<yunity::Model>();
	apex_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "apex"));
	apex_->SetCamera(camera_);
	apexWorldTransform_.Initialize();
	apexBody_ = std::make_unique<Body>();
	apexBody_->CreateBody(world, &apexWorldTransform_);

	springJoint_ = std::make_unique<yunity::SpringJoint>();
	springJoint_->CreateSpringJoint(this, apexBody_.get());
	springJoint_->EnableSpring(0, false);
	springJoint_->EnableSpring(1, false);
	springJoint_->SetEquilibriumPoint(0, 0.0f);
	springJoint_->SetEquilibriumPoint(1, 0.0f);
	springJoint_->SetStiffness(0, stiffness_);
	springJoint_->SetStiffness(1, stiffness_);
	springJoint_->SetDamping(0, dampar_);
	springJoint_->SetDamping(1, dampar_);

	world->AddJoint(springJoint_.get());


	isWire_ = false;
	isJump_ = true;
	isMoving_ = false;
	isSelect_ = false;


	yunity::Input::GetInstance()->GetJoystickState(0, pad_);

	scoreUI_ = std::make_unique<Score>();
	scoreUI_->Initialize();
	isScore_ = false;
	isReticle_ = false;
	lerpTime_ = 0.0f;
	displayTime_ = 0.0f;

	fixedJoint_ = std::make_unique<yunity::FixedJoint>();
	playerFixedJoint_ = std::make_unique<yunity::FixedJoint>();

	pointParticle_ = std::make_unique<PointParticle>();
	pointParticle_->Initialize(camera);
	smokeParticle_ = std::make_unique<SmokeParticle>();
	smokeParticle_->Initialize(camera);

	for (int i = 0; i < 10; i++) {
		fireworksParticles_[i] = std::make_unique<FireworksParticle>();
		fireworksParticles_[i]->Initialize(camera_);
	}

}

void Player::Update()
{
	prePad_ = pad_;

	Vector3 move = { 0.0f, 0.0f, 0.0f };
	Vector3 reticleMove = { 0.0f, 0.0f, 0.0f };

	InGameProduction();
	ResetProduction();
	DeathProduction();
	GoalProduction();

	scoreUI_->Update();

	if (std::abs(GetVelocity().x) > limitSpeed_) {
		Vector3 velocity = GetVelocity();
		float pandm = velocity.x / std::abs(velocity.x);
		SetVelocity({ limitSpeed_ * pandm, velocity.y, velocity.z });
	}

	if (CommonData::GetInstance()->isGoal_) {
		isReticle_ = false;
		return;
	}

	// ジョイスティック状態取得
	if (yunity::Input::GetInstance()->IsControllerConnected()) {
		if (yunity::Input::GetInstance()->GetJoystickState(0, pad_)) {

			// プレイヤーの動き
			const float threshold = 0.7f;
			bool isMoving = false;
			// 速さ
			float speed = fixedSpeed_;
			if (!isHit_ && !isWire_) {
				speed = floatSpeed_;
			}

			// 移動量
			if (isWire_) {
				move = { (float)pad_.Gamepad.sThumbLX, (float)pad_.Gamepad.sThumbLY,0 };
				Vector3 landingPoint = MapWorldToScreen(apexWorldTransform_.GetMatWorldTranslation(), camera_->GetViewMatrix(), camera_->GetProjectionMatrix(), 1280.0f, 720.0f);
				landingPoint_->SetPosition({ landingPoint.x, landingPoint.y });
			}
			else {
				move = { (float)pad_.Gamepad.sThumbLX, 0,0 };
			}

			if (Length(move) > threshold) {
				isMoving = true;
			}

			if (isMoving) {
				if (isHit_) {
					smokeParticle_->Spawn({ worldTransform_.GetMatWorldTranslation().x, worldTransform_.GetMatWorldTranslation().y - 2.5f, worldTransform_.GetMatWorldTranslation().z });
				}

				move.Normalize();
				// 移動量に速さを反映
				move = Multiply(speed, move);

				AddForce(move, Body::ForceMode::kImpulse);
			}

			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !isSelect_) {
				if (!isJump_) {
					isJump_ = true;
					AddForce({ 0.0f, 35.0f, 0.0f }, Body::ForceMode::kImpulse);
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
			uint32_t rayMask = ~(kCollisionAttributePlayer | kCollisionAttributeCoin);
			isHit = RayCast(worldTransform_.translation_, direction, &hit, lenght, GetWorld(), rayMask);


			// ワイヤー
			if (CommonData::GetInstance()->scene_ == Scene::kStage) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
					if (!isWire_) {
						if (isHit && hit.collider->GetCollisionAttribute() != kCollisionAttributeCoin) {
							isWire_ = true;
							springJoint_->EnableSpring(0, true);
							springJoint_->EnableSpring(1, true);
							point_ = hit.point;
							apexWorldTransform_.translation_ = hit.point;
							apexBody_->SetMatTranslation(hit.point);
							if (hit.collider->GetCollisionAttribute() == kCollisionAttributeMoveFloor || hit.collider->GetCollisionAttribute() == kCollisionAttributeTrampoline || hit.collider->GetCollisionAttribute() == kCollisionAttributePillar) {
								fixedJoint_->CreateFixedJoint(hit.collider, apexBody_.get());
								GetWorld()->AddJoint(fixedJoint_.get());
							}
							pointParticle_->Spawn(hit.point);
						}
					}
					else {
						isWire_ = false;
						springJoint_->EnableSpring(0, false);
						springJoint_->EnableSpring(1, false);
						fixedJoint_->Clear();
						GetWorld()->TakeJoint(fixedJoint_.get());
						if (apexWorldTransform_.parent_) {
							apexWorldTransform_.parent_ = nullptr;
						}
					}
				}
			}

			if (isHit) {
				if (!isWire_) {
					Vector3 landingPoint = MapWorldToScreen(hit.point, camera_->GetViewMatrix(), camera_->GetProjectionMatrix(), 1280.0f, 720.0f);
					landingPoint_->SetPosition({ landingPoint.x, landingPoint.y });
				}
				isHitRay_ = true;
			}
			else {
				if (!isWire_) {
					isHitRay_ = false;
				}
			}


		}
	}

	if (Length(reticleWorldTransform_.translation_) > limitLength_) {
		Vector3 dir = reticleWorldTransform_.translation_.Normalize();
		reticleWorldTransform_.translation_ = Multiply(limitLength_, dir);
	}

	reticleWorldTransform_.UpdateMatrix();
	apexWorldTransform_.UpdateMatrix();
	apexBody_->GetWorldTransform()->UpdateMatrix();

	Vector2 pos = WorldToScreen({ reticleWorldTransform_.matWorld_.m[3][0], reticleWorldTransform_.matWorld_.m[3][1], reticleWorldTransform_.matWorld_.m[3][2] }, camera_->GetViewMatrix(), camera_->GetProjectionMatrix(), 1280.0f, 720.0f);
	reticle_->SetPosition(pos);

	if (worldTransform_.translation_.y < -12.0f) {
		InitializeDeth();
	}

	if (!isMoving_) {
		GetWorld()->TakeJoint(playerFixedJoint_.get());
	}

	isHit_ = false;
	isSelect_ = false;
	isMoving_ = false;

	smokeParticle_->Update();
	pointParticle_->Update();

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
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
	if (isReticle_) {
		if (CommonData::GetInstance()->scene_ == Scene::kStage) {
			reticle3D_->Draw(reticleWorldTransform_, yunity::TextureManager::GetInstance()->Load("pink1x1.png"));
			if (isWire_) {
				line_->Draw(worldTransform_.translation_, apexBody_->GetMatWorldTranslation(), { 0.0f, 0.0f, 0.0f, 1.0f });
				apex_->Draw(*apexBody_->GetWorldTransform(), yunity::TextureManager::GetInstance()->Load("purple1x1.png"));
			}
		}
	}

	smokeParticle_->Draw();
	pointParticle_->Draw();
	for (int i = 0; i < 10; i++) {
		fireworksParticles_[i]->Draw();
	}

#ifdef _DEBUG
	HitBox();
#endif // _DEBUG

}

void Player::DrawUI()
{
	if (isReticle_) {
		if (isHitRay_ || isWire_) {
			landingPoint_->Draw();
		}
		reticle_->Draw();
		
	}
	if (isScore_) {
		scoreUI_->Draw();
	}
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
		InitializeDeth();
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
		isJump_ = false;
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
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeGoal && CommonData::GetInstance()->flagState_ < FlagState::kGoal) {
		CommonData::GetInstance()->isGoal_ = true;
		CommonData::GetInstance()->flagState_ = FlagState::kGoal;
		SetVelocity({ 0.0f, 0.0f, 0.0f });
		goalPoint_ = GetHitBody()->GetMatWorldTranslation();
		oldPlayerPosition_ = GetMatWorldTranslation();
		time_ = 0.0f;

	}
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeSelect) {
		isSelect_ = true;

	}
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeCoin) {
		scoreUI_->AddScore(10);
	}
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeCheckPoint) {
		spawnPoint_ = GetHitBody()->GetMatWorldTranslation();
		spawnPoint_.z = 0.0f;
	}
}

void Player::Reset()
{
	worldTransform_.translation_ = spawnPoint_;
	Body::Reset();
	point_ = worldTransform_.translation_;
	apexWorldTransform_.translation_ = worldTransform_.translation_;
	springJoint_->EnableSpring(0, false);
	springJoint_->EnableSpring(1, false);
	isWire_ = false;
	isActive_ = true;
}

void Player::Reset(const Vector3& pos)
{
	worldTransform_.translation_ = pos;
	Body::Reset();
	point_ = worldTransform_.translation_;
	apexWorldTransform_.translation_ = worldTransform_.translation_;
	isWire_ = false;
	isActive_ = true;
}

bool Player::Result()
{
	if (lerpTime_ != 1.0f) {
		lerpTime_ += 1.0f / 30.0f;
		lerpTime_ = std::clamp(lerpTime_, 0.0f, 1.0f);
		scoreUI_->SetPosition(Lerp(Vector2(84.0f, 84.0f), Vector2(640.0f, 360.0f), lerpTime_));
	}
	else if (displayTime_ != 1.0f) {
		displayTime_ += 1.0f / 180.0f;
		displayTime_ = std::clamp(displayTime_, 0.0f, 1.0f);
		if (scoreUI_->GetScore() > CommonData::GetInstance()->hiScore_) {
			CommonData::GetInstance()->hiScore_ = scoreUI_->GetScore();
			scoreUI_->SetDisplayHiScore(true);

		}

		int num = 0;
		for (int i = 0; i < 10; i++) {
			if (!fireworksParticles_[i]->GetIsActive() && num < 5) {
				fireworksParticles_[i]->Spawn({ worldTransform_.GetMatWorldTranslation().x + 4.0f * rng.NextFloatRange(-5.0f, 5.0f), worldTransform_.GetMatWorldTranslation().y + 4.0f * rng.NextFloatRange(5.0f, 10.0f), 0.0f });
				num++;
			}

			fireworksParticles_[i]->Update();
		}
	}
	else {
		scoreUI_->SetDisplayHiScore(false);
		isScore_ = false;
		return true;
	}
	return false;

}

void Player::InGameProduction()
{
	if (CommonData::GetInstance()->flagState_ == FlagState::kInStage) {
		if (!Tradition::GetInstance()->GetOut()) {
			CommonData::GetInstance()->flagState_ = FlagState::kPlay;
		}
		Tradition::GetInstance()->Update();
		camera_->SetOffset(Lerp({ 0.0f, 0.0f, 0.0f }, setCameraPos_, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
		SetScale(Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, std::clamp(1.0f - Tradition::GetInstance()->GetTime(), 0.0f, 1.0f)));
		if(worldTransform_.scale_.x == 1.0f){
				isScore_ = true;
				isReticle_ = true;
		}
	}
}

void Player::ResetProduction()
{
	if (CommonData::GetInstance()->flagState_ == FlagState::kReset) {
		if (time_ >= 1.0f) {
			time_ += yunity::fixedTimeStep_;
			float t = std::clamp(time_ - 1.0f, 0.0f, 1.0f);
			SetScale(Lerp({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f },t));
			if (t == 1.0f) {
				CommonData::GetInstance()->flagState_ = FlagState::kPlay;
				time_ = 0.0f;
			}
		}
		else {
			time_ += yunity::fixedTimeStep_ * resetTime_;
			time_ = std::clamp(time_, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(dieCameraPosition_, { spawnPoint_.x, spawnPoint_.y + camera_->GetOffset().y, dieCameraPosition_.z }, time_));
			SetTranslation(Lerp(diePlayerPosition_, spawnPoint_, time_));
			if (time_ == 1.0f) {
				Reset();
				SetIsTrigger(false);
				camera_->SetTarget(GetWorldTransform());
			}
		}
	}

}

void Player::DeathProduction()
{
	if (CommonData::GetInstance()->flagState_ == FlagState::kDeth) {
		if (time_ < 1.0f) {
			time_ += yunity::fixedTimeStep_ * dieUpTime_;
			time_ = std::clamp(time_, 0.0f, 1.0f);
			float t = 1.0f - std::powf(1.0f - time_, 5);
			Vector3 pos = Slerp(diePlayerPosition_, Add(diePlayerPosition_, dieUp_), t);
			pos.x = diePlayerPosition_.x;
			SetTranslation(pos);
			if (time_ == 1.0f) {
				topPos_ = GetTranslation();
			}
		}
		else if (time_ - 1.0f < 1.0f) {
			time_ += yunity::fixedTimeStep_ * dieDownTime_;
			float t = std::clamp(time_ - 1.0f, 0.0f, 1.0f);
			Vector3 pos = Slerp(topPos_, Add(topPos_, dieDown_), t);
			pos.x = topPos_.x;
			SetTranslation(pos);
		}
		else {
			CommonData::GetInstance()->flagState_ = FlagState::kReset;
			diePlayerPosition_ = GetTranslation();
			SetIsTrigger(false);
			SetScale({ 0.0f, 0.0f, 0.0f });
			time_ = 0.0f;
		}
	}

}

void Player::GoalProduction()
{
	if (CommonData::GetInstance()->flagState_ == FlagState::kGoal) {
		time_ += yunity::fixedTimeStep_ * goalTime_;
		time_ = std::clamp(time_, 0.0f, 1.0f);
		SetPosition(Lerp(oldPlayerPosition_, { goalPoint_.x, GetMatWorldTranslation().y, 0.0f }, time_));

		if (time_ == 1.0f) {
			CommonData::GetInstance()->flagState_ = FlagState::kClear;
			clearCameraPosition_ = camera_->GetTranslate();
			camera_->SetTarget(nullptr);
			Tradition::GetInstance()->Initialize();
			Tradition::GetInstance()->Start();
			time_ = 0.0f;
		}
	}
	else if (CommonData::GetInstance()->flagState_ == FlagState::kClear) {
		if (time_ < 1.0f) {
			if (Result()) {
				time_ += yunity::fixedTimeStep_ * clearTime_;
				time_ = std::clamp(time_, 0.0f, 1.0f);
				camera_->SetTranslate(Lerp(clearCameraPosition_, { clearCameraPosition_.x, goalPoint_.y, zeemOutPositionZ }, time_));
				if (time_ == 1.0f) {
					clearCameraPosition_ = camera_->GetTranslate();
					yunity::RenderTexture::GetInstance()->SelectPostEffect(yunity::PostEffects::kRadialBlur, true);
				}
			}
		}
		else {
			time_ += yunity::fixedTimeStep_;
			float t = std::clamp(time_ - 1.0f, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(clearCameraPosition_, GetGoalPoint(), t));
			Tradition::GetInstance()->Update();
			if (!Tradition::GetInstance()->GetIn()) {
				CommonData::GetInstance()->isGoal_ = false;
				yunity::SceneManager::GetInstance()->ChangeScene("TITLE");
				isSelect_ = false;
				Tradition::GetInstance()->Initialize();
			}
		}
		
	}

}

void Player::InitializeDeth()
{
	isActive_ = false;
	dieCameraPosition_ = camera_->GetTranslate();
	diePlayerPosition_ = GetMatWorldTranslation();
	camera_->SetTarget(nullptr);
	CommonData::GetInstance()->flagState_ = FlagState::kDeth;
	AddForce({ 0.0f, 10.0f, -1.0f }, yunity::Body::ForceMode::kImpulse);
	SetIsTrigger(true);
	time_ = 0.0f;
}

void Player::SetDisplayUI(bool flag, UI ui)
{
	switch (ui)
	{
	case Player::UI::kScore:
		isScore_ = flag;
		break;
	case Player::UI::kReticle:
		isReticle_ = flag;
		break;
	default:
		break;
	}
}
