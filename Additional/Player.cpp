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
#include "WindowsAPI.h"
#include "GlobalVariables.h"

void Player::Initialize(yunity::Camera* camera, yunity::World* world)
{
	// 外部ファイルからデータ読み込み
	ApplyGlobalVariables();

	// ヒットボックスのサイズ設定
	SetHitBoxSize(hitBoxSize_);

	// 基本データ初期化
	Object3D::Initialize(world, yunity::Collider::Shape::kAABB);
	worldTransform_.rotateType_ = yunity::RotationType::Quaternion;
	worldTransform_.translation_ = Add(worldTransform_.translation_, startPosition_);

	// 物理パラメータ設定
	SetMass(mass_);
	SetFirictionCombine(FrictionCombine::kAverage);
	SetMiu(miu_);
	SetBounceCombine(BounceCombine::kMaximum);
	SetBounciness(0.0f);
	SetFixedMove(2, true);
	SetDrag(0.0f);

	// モデル設定
	models_["player"] = std::make_unique<yunity::Model>();
	models_["player"].reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "Player"));
	models_["player"]->SetCamera(camera);

	camera_ = camera;

	isCrouching_ = false;

	// 物理空間に自分を追加
	world->Add(this);

	isHit_ = false;

	// 衝突属性設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突マスク設定
	SetCollisionMask(~kCollisionAttributePlayer);

	isMoving_ = false;
	isSelect_ = false;

	// ワイヤー初期化
	wire_ = std::make_unique<Wire>();
	wire_->Initialize(camera, world, &worldTransform_, this);
	isReticle_ = false;

	// ガイド初期化
	guideUI_ = std::make_unique<GuideUI>();
	guideUI_->Initialize();

	yunity::Input::GetInstance()->GetJoystickState(0, pad_);

	scoreUI_ = std::make_unique<Score>();
	scoreUI_->Initialize();
	isScore_ = false;
	lerpTime_ = 0.0f;
	displayTime_ = 0.0f;

	playerFixedJoint_ = std::make_unique<yunity::FixedJoint>();

	smokeParticle_ = std::make_unique<SmokeParticle>();
	smokeParticle_->Initialize(camera);

	for (int i = 0; i < fireworksParticleQuantity_; i++) {
		fireworksParticles_[i] = std::make_unique<FireworksParticle>();
		fireworksParticles_[i]->Initialize(camera_);
	}

	pose_ = std::make_unique<Pose>();
	pose_->Initialize(this);

}

void Player::Update()
{
	// 前回のジョイスティック状態
	prePad_ = pad_;

	// プレイヤーとレティクルの移動量を初期化
	Vector3 move = { 0.0f, 0.0f, 0.0f };
	Vector3 reticleMove = { 0.0f, 0.0f, 0.0f };

	// 各種演出
	if (productionState_.get() != nullptr) {
		productionState_->Update(this);
	}

	// スコアUIの更新
	scoreUI_->Update();

	// ガイドのテクスチャを設定
	guideUI_->SetTexrture(GuideUI::GuideType::ABotton, 0);
	guideUI_->SetTexrture(GuideUI::GuideType::RBBotton, 0);

	// x軸方向の制限速度
	if (std::abs(GetVelocity().x) > limitSpeed_) {
		Vector3 velocity = GetVelocity();
		float pandm = velocity.x / std::abs(velocity.x);
		SetVelocity({ limitSpeed_ * pandm, velocity.y, velocity.z });
	}

	if (CommonData::GetInstance()->isGoal_ || CommonData::GetInstance()->flagState_ == FlagState::kCount) { // 早期リターン
		isReticle_ = false;
		return;
	}

	if (pose_->Update()) {
		return;
	}

	// ジョイスティック状態取得
	if (yunity::Input::GetInstance()->IsControllerConnected()) {
		if (yunity::Input::GetInstance()->GetJoystickState(0, pad_)) {

			// プレイヤーの動き
			bool isMoving = false;
			// 速さ
			float speed = fixedSpeed_;
			if (!isHit_ && /*!isWire_*/ !wire_->GetIsWire()) { // 空中時の速さ
				speed = floatSpeed_;
			}

			// 移動量
			if (wire_->GetIsWire()/*isWire_*/) { // ワイヤー使用時
				move = { (float)pad_.Gamepad.sThumbLX, (float)pad_.Gamepad.sThumbLY,0 };
				guideUI_->SetTexrture(GuideUI::GuideType::Wire, 1);
			}
			else {
				move = { (float)pad_.Gamepad.sThumbLX, 0,0 };
				guideUI_->SetTexrture(GuideUI::GuideType::Wire, 0);
			}

			if (Length(move) > threshold_) { // しきい値以上なら
				isMoving = true;
			}

			if (isMoving) { // 動いている時
				if (isHit_) { // 何かに触れていたら
					// 足元に煙のパーティクルを生成
					smokeParticle_->Spawn({ worldTransform_.GetMatWorldTranslation().x, worldTransform_.GetMatWorldTranslation().y, worldTransform_.GetMatWorldTranslation().z });
				}

				move.Normalize();
				// 移動量に速さを反映
				move = Multiply(speed, move);

				AddForce(move, Body::ForceMode::kImpulse);
			}

			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !isSelect_) {
				if (isHit_) {
					//guideA_->SetTextureHandle(guideATexture_[1]);
					guideUI_->SetTexrture(GuideUI::GuideType::ABotton, 1);
					AddForce(jumpPower_, Body::ForceMode::kImpulse);
					GetWorld()->TakeJoint(playerFixedJoint_.get());
				}
			}
		}
	}

	

	wire_->Update();

	if (worldTransform_.translation_.y < deatLine_) {
		InitializeDeth();

	}

	if (!isMoving_) { // 動く床に乗っていない時
		GetWorld()->TakeJoint(playerFixedJoint_.get());
	}

	isHit_ = false;
	isSelect_ = false;
	isMoving_ = false;

	smokeParticle_->Update();

#ifdef _DEBUG
	ImGui::Begin("Player");
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x);
	Vector3 velocity = GetVelocity();
	ImGui::DragFloat3("velocity", &velocity.x);
	ImGui::End();

#endif
}

void Player::Draw()
{
	// プレイヤーモデル描画
	models_["player"]->Draw(worldTransform_);
	if (isReticle_) {
		if (CommonData::GetInstance()->scene_ == Scene::kStage) {
			wire_->Draw3D();
		}
	}

	smokeParticle_->Draw();
	for (int i = 0; i < fireworksParticleQuantity_; i++) {
		fireworksParticles_[i]->Draw();
	}

#ifdef _DEBUG
	HitBox();
#endif // _DEBUG

}

void Player::DrawUI()
{
	if (isReticle_) {

		// ワイヤー関係のUI描画
		wire_->DrawUI();

		// ガイド描画
		guideUI_->Draw();
		
	}
	if (isScore_) {
		// スコア描画
		scoreUI_->Draw();
	}

	pose_->Draw();

}

void Player::OnCollisionEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeSpike) { // トゲに当たった時
		InitializeDeth();
	}

	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeMoveFloor) { // 動く床に当たった時
		AABB aabb = GetAABB();
		AABB other = GetHitBody()->GetAABB();
		if (aabb.min.y >= (other.max.y + other.min.y) / 2.0f) { // プレイヤーの方が上
			if (aabb.min.x + toleranceLevel_ < other.max.x && aabb.max.x + toleranceLevel_ > other.min.x) { // 床の範囲内
				// 固定する
				playerFixedJoint_->CreateFixedJoint(GetHitBody(), this);
				GetWorld()->AddJoint(playerFixedJoint_.get());
				isMoving_ = true;
			}

		}
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
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeGoal && CommonData::GetInstance()->flagState_ < FlagState::kGoal) { // ゴールした時
		CommonData::GetInstance()->isGoal_ = true;
		CommonData::GetInstance()->flagState_ = FlagState::kGoal;
		ChangeProductionState(std::make_unique<GoalProduction>());
		SetVelocity({ 0.0f, 0.0f, 0.0f });
		goalPoint_ = GetHitBody()->GetMatWorldTranslation();
		oldPlayerPosition_ = GetMatWorldTranslation();
		time_ = 0.0f;

	}
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeSelect) { // セレクトTV
		isSelect_ = true;

	}
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeCoin) { // コイン
		scoreUI_->AddScore(coinValue_);
	}
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributeCheckPoint) { // チェックポイント
		spawnPoint_ = GetHitBody()->GetMatWorldTranslation();
		spawnPoint_.z = 0.0f;
	}
}

void Player::Reset()
{
	worldTransform_.translation_ = spawnPoint_;
	Body::Reset();
	wire_->Reset();
	isReticle_ = true;
}

void Player::Reset(const Vector3& pos)
{
	worldTransform_.translation_ = pos;
	Body::Reset();
	wire_->Reset();
}

bool Player::Result()
{
	if (lerpTime_ / limitLerpTime_ != 1.0f) {
		lerpTime_ += yunity::fixedTimeStep_;
		lerpTime_ = std::clamp(lerpTime_, 0.0f, limitLerpTime_);
		scoreUI_->SetPosition(Lerp(scoreStartPosition_, scoreEndPosition_, lerpTime_ / limitLerpTime_));
	}
	else if (displayTime_ / limitDisplayeTime_ != 1.0f) {
		displayTime_ += yunity::fixedTimeStep_;
		displayTime_ = std::clamp(displayTime_, 0.0f, limitDisplayeTime_);
		if (scoreUI_->GetScore() > CommonData::GetInstance()->hiScore_) {
			CommonData::GetInstance()->hiScore_ = scoreUI_->GetScore();
			scoreUI_->SetDisplayHiScore(true);

		}

		int num = 0;
		for (int i = 0; i < fireworksParticleQuantity_; i++) {
			if (!fireworksParticles_[i]->GetIsActive() && num < fireworksParticleQuantity_ / 2) {
				fireworksParticles_[i]->Spawn({ worldTransform_.GetMatWorldTranslation().x, worldTransform_.GetMatWorldTranslation().y, 0.0f });
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

void Player::InitializeDeth()
{
	deathCameraPosition_ = camera_->GetTranslate();
	deathPosition_ = GetMatWorldTranslation();
	camera_->SetTarget(nullptr);
	CommonData::GetInstance()->flagState_ = FlagState::kDeth;
	AddForce({ 0.0f, 10.0f, -1.0f }, yunity::Body::ForceMode::kImpulse);
	SetIsTrigger(true);
	time_ = 0.0f;
	wire_->SetIsWire(false);
	isReticle_ = false;
	ChangeProductionState(std::make_unique<DeathProduction>());
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

void Player::ApplyGlobalVariables()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "Player";

	threshold_ = globalVariables->GetFloatValue(groupName, "Threshold");
	mass_ = globalVariables->GetFloatValue(groupName, "Mass");
	miu_ = globalVariables->GetFloatValue(groupName, "Miu");
	limitLerpTime_ = globalVariables->GetFloatValue(groupName, "LimitLerpTime");
	limitDisplayeTime_ = globalVariables->GetFloatValue(groupName, "LimitDisplayeTime");
	scoreStartPosition_ = globalVariables->GetVector2Value(groupName, "ScoreStartPosition");
	scoreEndPosition_ = globalVariables->GetVector2Value(groupName, "ScoreEndPosition");
	fixedSpeed_ = globalVariables->GetFloatValue(groupName, "FixedSpeed");
	floatSpeed_ = globalVariables->GetFloatValue(groupName, "FloatSpeed");
	jumpPower_ = globalVariables->GetVector3Value(groupName, "JumpPower");
	limitSpeed_ = globalVariables->GetFloatValue(groupName, "LimitSpeed");
	hitBoxSize_ = globalVariables->GetVector3Value(groupName, "HitBoxSize");
	startPosition_ = globalVariables->GetVector3Value(groupName, "StartPosition");
	deatLine_ = globalVariables->GetFloatValue(groupName, "DeatLine");
	toleranceLevel_ = globalVariables->GetFloatValue(groupName, "ToleranceLevel");
	coinValue_ = globalVariables->GetIntValue(groupName, "CoinValue");
}

void Player::ChangeProductionState(std::unique_ptr<PlayerProduction> newState)
{
	if (newState == nullptr) {
		productionState_.reset(nullptr);
	}
	else {
		productionState_ = std::move(newState);
		productionState_->Initialize();
	}
}

