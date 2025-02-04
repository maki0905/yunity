#include "Wire.h"

#include "ModelManager.h"
#include "Player.h"
#include "WindowsAPI.h"
#include "RayCast.h"
#include "CommonData.h"
#include "GlobalVariables.h"

void Wire::Initialize(yunity::Camera* camera, yunity::World* world, yunity::WorldTransform* worldTransform, Player* player)
{
	ApplyGlobalVariables();

	isWire_ = false;

	// ワールド取得
	world_ = world;

	// プレイヤーのワールドトランスフォームを取得
	playerWorldTransform_ = worldTransform;

	// カメラ取得
	camera_ = camera;

	// 3Dレティクル
	reticle3D_ = std::make_unique<yunity::Model>();
	reticle3D_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "apex"));
	reticle3D_->SetCamera(camera);

	// 2Dレティクル
	reticle_ = std::make_unique<yunity::Sprite>();
	reticle_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("offReticle.png"), { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	landingPoint_ = std::make_unique<yunity::Sprite>();
	landingPoint_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("onReticle.png"), { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	reticleWorldTransform_.Initialize();
	reticleWorldTransform_.parent_ = worldTransform;
	landingPointWorldTrans_.Initialize();

	// ワイヤーの先端
	apex_ = std::make_unique<yunity::Model>();
	apex_.reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "apex"));
	apex_->SetCamera(camera);
	apexWorldTransform_.Initialize();
	apexBody_ = std::make_unique<yunity::Body>();
	apexBody_->CreateBody(world, &apexWorldTransform_);

	// ワイヤーの線
	for (int i = 0; i < maxLines_; i++) {
		std::unique_ptr<yunity::PrimitiveDrawer> line = std::make_unique<yunity::PrimitiveDrawer>();
		line.reset(yunity::PrimitiveDrawer::Create());
		line->SetCamera(camera_);
		lines_.push_back(std::move(line));
	}

	// スプリングジョイント
	springJoint_ = std::make_unique<yunity::SpringJoint>();
	// ジョイント生成
	springJoint_->CreateSpringJoint(player, apexBody_.get());
	// 有効軸設定
	springJoint_->EnableSpring(0, false);
	springJoint_->EnableSpring(1, false);
	// 各軸の自然長
	springJoint_->SetEquilibriumPoint(0, 0.0f);
	springJoint_->SetEquilibriumPoint(1, 0.0f);
	// 各軸の剛性
	springJoint_->SetStiffness(0, stiffness_);
	springJoint_->SetStiffness(1, stiffness_);
	// 各軸の減衰
	springJoint_->SetDamping(0, dampar_);
	springJoint_->SetDamping(1, dampar_);
	// ワールドに追加
	world->AddJoint(springJoint_.get());

	// 固定ジョイント
	fixedJoint_ = std::make_unique<yunity::FixedJoint>();

	pointParticle_ = std::make_unique<PointParticle>();
	pointParticle_->Initialize(camera);
}

void Wire::Update()
{

	// 前回のジョイスティック状態
	prePad_ = pad_;

	if (isWire_) { // ワイヤー時の設置レティクルの位置を設定
		Vector3 landingPoint = MapWorldToScreen(apexWorldTransform_.GetMatWorldTranslation(), camera_->GetViewMatrix(), camera_->GetProjectionMatrix(), yunity::WindowsAPI::kWindowWidth, yunity::WindowsAPI::kWindowHeight);
		landingPoint_->SetPosition({ landingPoint.x, landingPoint.y });
	}

	// レティクルの移動量を初期化
	Vector3 reticleMove = { 0.0f, 0.0f, 0.0f };
	if (yunity::Input::GetInstance()->IsControllerConnected()) {
		if (yunity::Input::GetInstance()->GetJoystickState(0, pad_)) {
			// レティクルの動き
			reticleMove = { (float)pad_.Gamepad.sThumbRX, (float)pad_.Gamepad.sThumbRY, 0.0f };
			reticleMove = Multiply(reticleSpeed_, reticleMove.Normalize());
			reticleWorldTransform_.translation_ = Add(reticleWorldTransform_.translation_, reticleMove);
		
		}
	}

	// レティクル方向に線分を飛ばして設置点を取得
	bool isHit = false;
	RayCastHit hit;
	Vector3 direction = Subtract({ reticleWorldTransform_.matWorld_.m[3][0], reticleWorldTransform_.matWorld_.m[3][1], reticleWorldTransform_.matWorld_.m[3][2] }, playerWorldTransform_->translation_);
	float lenght = Length(direction);
	if (lenght > limitLength_) {
		lenght = limitLength_;
	}
	direction.Normalize();
	// マスク生成
	uint32_t rayMask = ~(kCollisionAttributePlayer | kCollisionAttributeCoin | kCollisionAttributeForceField);
	isHit = RayCast(playerWorldTransform_->translation_, direction, &hit, lenght, world_,rayMask);

	// ワイヤー
	if (CommonData::GetInstance()->scene_ == Scene::kStage) {
		if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			if (!isWire_) { // ワイヤーを発射
				if (isHit && hit.collider->GetCollisionAttribute() != kCollisionAttributeCoin) {
					isWire_ = true;
					springJoint_->EnableSpring(0, true);
					springJoint_->EnableSpring(1, true);
					apexWorldTransform_.translation_ = hit.point;
					apexBody_->SetMatTranslation(hit.point);
					// 動くオブジェクトの場合
					if (hit.collider->GetCollisionAttribute() == kCollisionAttributeMoveFloor || hit.collider->GetCollisionAttribute() == kCollisionAttributeMove|| hit.collider->GetCollisionAttribute() == kCollisionAttributePillar) {
						fixedJoint_->CreateFixedJoint(hit.collider, apexBody_.get());
						world_->AddJoint(fixedJoint_.get());
					}
					// パーティクル生成
					pointParticle_->Spawn(hit.point);
				}
			}
			else { // ワイヤーを解除
				isWire_ = false;
				springJoint_->EnableSpring(0, false);
				springJoint_->EnableSpring(1, false);
				fixedJoint_->Clear();
				world_->TakeJoint(fixedJoint_.get());
				if (apexWorldTransform_.parent_) {
					apexWorldTransform_.parent_ = nullptr;
				}
			}
		}
	}

	if (isHit) { // レティクル方向にオブジェクトがあった場合
		if (!isWire_) {
			Vector3 landingPoint = MapWorldToScreen(hit.point, camera_->GetViewMatrix(), camera_->GetProjectionMatrix(), yunity::WindowsAPI::kWindowWidth, yunity::WindowsAPI::kWindowHeight);
			landingPoint_->SetPosition({ landingPoint.x, landingPoint.y });
		}
		isHitRay_ = true;
	}
	else {
		if (!isWire_) {
			isHitRay_ = false;
		}
	}

	// ワイヤーの紅白描画用の頂点生成
	if (isWire_) {
		lineVertexs_.clear();
		float currentLength = 0.0f;
		direction = Subtract(apexBody_->GetMatWorldTranslation(), playerWorldTransform_->translation_);
		lenght = Length(direction);
		direction.Normalize();

		while (currentLength < lenght) {
			Vector3 startPosition = Add(playerWorldTransform_->translation_, Multiply(currentLength, direction));
			lineVertexs_.push_back(startPosition);

			float nextLength = std::min(currentLength + segmentLength_, lenght);
			Vector3 endPosition = Add(playerWorldTransform_->translation_, Multiply(nextLength, direction));
			lineVertexs_.push_back(endPosition);

			currentLength = nextLength;
		}
	}

	if (Length(reticleWorldTransform_.translation_) > limitLength_) { // レティクルの距離が制限以上の場合
		Vector3 dir = reticleWorldTransform_.translation_.Normalize();
		reticleWorldTransform_.translation_ = Multiply(limitLength_, dir);
	}

	reticleWorldTransform_.UpdateMatrix();
	apexWorldTransform_.UpdateMatrix();
	apexBody_->GetWorldTransform()->UpdateMatrix();

	Vector2 pos = WorldToScreen({ reticleWorldTransform_.matWorld_.m[3][0], reticleWorldTransform_.matWorld_.m[3][1], reticleWorldTransform_.matWorld_.m[3][2] }, camera_->GetViewMatrix(), camera_->GetProjectionMatrix(), yunity::WindowsAPI::kWindowWidth, yunity::WindowsAPI::kWindowHeight);
	reticle_->SetPosition(pos);

	pointParticle_->Update();

}

void Wire::Draw3D()
{
	reticle3D_->Draw(reticleWorldTransform_, yunity::TextureManager::GetInstance()->Load("pink1x1.png"));
	if (isWire_) {
		for (int i = 0; i < lineVertexs_.size() / 2; i++) {
			if (i % 2 == 0) {
				lines_[i]->Draw(lineVertexs_[i * 2], lineVertexs_[i * 2 + 1], { 1.0f, 0.0f, 0.0f, 1.0f });
			}
			else {
				lines_[i]->Draw(lineVertexs_[i * 2], lineVertexs_[i * 2 + 1], { 1.0f, 1.0f, 1.0f, 1.0f });
			}
		}
		apex_->Draw(*apexBody_->GetWorldTransform(), yunity::TextureManager::GetInstance()->Load("purple1x1.png"));
	}

	pointParticle_->Draw();
}

void Wire::DrawUI()
{
	if (isHitRay_ || isWire_) {
		landingPoint_->Draw();
	}
	reticle_->Draw();
}

void Wire::Reset()
{
	apexWorldTransform_.translation_ = playerWorldTransform_->translation_;
	springJoint_->EnableSpring(0, false);
	springJoint_->EnableSpring(1, false);
	fixedJoint_->Clear();
	isWire_ = false;
}

void Wire::ApplyGlobalVariables()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "Wire";

	reticleSpeed_ = globalVariables->GetFloatValue(groupName, "ReticleSpeed");
	stiffness_ = globalVariables->GetFloatValue(groupName, "Stiffness");
	dampar_ = globalVariables->GetFloatValue(groupName, "Dampar");
	limitLength_ = globalVariables->GetFloatValue(groupName, "LimitLength");
	segmentLength_ = globalVariables->GetFloatValue(groupName, "SegmentLength");
	maxLines_ = globalVariables->GetIntValue(groupName, "MaxLines");
}
