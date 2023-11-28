#include "Player.h"
#include "CollisionConfig.h"
#include "GlobalVariables.h"

#include "Externals/nlohmann/json.hpp"

#include "ImGuiManager.h"
#include "LockOn.h"

const std::array<Player::ConstAttack, Player::ComboNum>Player::kConstAttacks_ = {
	{
	{0, 0, 20, 30, 0.0f, 0.0f, 0.15f},
	{15, 10, 15, 30, 0.2f, 0.0f, 0.0f},
	{15, 10, 15, 30, 0.2f, 0.0f, 0.0f},
	}
};


void Player::Initialize(const std::vector<Model*>& models)
{
	input_ = Input::GetInstance();
	BaseObject::Initialize(models);

	worldTransform_Body_.Initialize();
	worldTransform_Head_.Initialize();
	worldTransform_Larm_.Initialize();
	worldTransform_Rarm_.Initialize();
	worldTransform_Weapon_.Initialize();


	worldTransform_Head_.parent_ = &worldTransform_Body_;
	worldTransform_Head_.translation_.y = 4.4f;
	worldTransform_Head_.rotation_.y = 3.2f;
	worldTransform_Larm_.parent_ = &worldTransform_Body_;
	worldTransform_Larm_.translation_ = Vector3(0.2f, 3.4f, 0.0f);
	worldTransform_Rarm_.parent_ = &worldTransform_Body_;
	worldTransform_Rarm_.translation_ = Vector3(-0.2f, 3.4f, 0.0f);
	worldTransform_Weapon_.parent_ = &worldTransform_Body_;

	sphere_ = {
		.center{worldTransform_Body_.translation_},
		.radius{2.0f}
	};
	capsule_.radius = 2.0f;

	velocity_ = { 0.0f, 0.0f, 0.0f };
	acceleration_ = { 0.0f, gravity_, 0.0f };

	obb_.center = worldTransform_Weapon_.translation_;
	Matrix4x4 rotate = MakeRotateXYZMatrix(worldTransform_Weapon_.rotation_);
	obb_.orientations[0].x = rotate.m[0][0];
	obb_.orientations[0].y = rotate.m[0][1];
	obb_.orientations[0].z = rotate.m[0][2];
	obb_.orientations[1].x = rotate.m[0][0];
	obb_.orientations[1].y = rotate.m[0][1];
	obb_.orientations[1].z = rotate.m[0][2];
	obb_.orientations[2].x = rotate.m[0][0];
	obb_.orientations[2].y = rotate.m[0][1];
	obb_.orientations[2].z = rotate.m[0][2];

	obb_.size = { 1.0f, 5.0f, 1.0f };

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
	isAlive_ = true;

	//workDash_.speed = 10.0f;
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	workAttack_.start_Arm[0] = globalVariables->GetVector3Value(groupName, "StartArm_0");
	workAttack_.start_Arm[1] = globalVariables->GetVector3Value(groupName, "StartArm_1");
	workAttack_.start_Arm[2] = globalVariables->GetVector3Value(groupName, "StartArm_2");
	workAttack_.end_Arm[0] = globalVariables->GetVector3Value(groupName, "EndArm_0");
	workAttack_.end_Arm[1] = globalVariables->GetVector3Value(groupName, "EndArm_1");
	workAttack_.end_Arm[2] = globalVariables->GetVector3Value(groupName, "EndArm_2");
	workAttack_.start_Weapon[0] = globalVariables->GetVector3Value(groupName, "StartWeapon_0");
	workAttack_.start_Weapon[1] = globalVariables->GetVector3Value(groupName, "StartWeapon_1");
	workAttack_.start_Weapon[2] = globalVariables->GetVector3Value(groupName, "StartWeapon_2");
	workAttack_.end_Weapon[0] = globalVariables->GetVector3Value(groupName, "EndWeapon_0");
	workAttack_.end_Weapon[1] = globalVariables->GetVector3Value(groupName, "EndWeapon_1");
	workAttack_.end_Weapon[2] = globalVariables->GetVector3Value(groupName, "EndWeapon_2");
	nlohmann::json json;
	json.clear();
	

}

void Player::Update()
{
	Input::GetInstance()->GetJoystickState(0, joyState_);

	
	for (auto& model : models_) {
		model->SetMaterial(color_);
	}

	capsule_.segment.origin = worldTransform_Body_.translation_;
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		// 各振る舞いごとの初期化を実行
		switch (behavior_) {

		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		}
		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
		// 通常行動
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
		// 攻撃行動
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}
	capsule_.segment.diff = Subtract(worldTransform_Body_.translation_, capsule_.segment.origin);

	worldTransform_Body_.quaternion_ = Slerp(worldTransform_Body_.quaternion_, moveQuaternion_, 0.3f);
	worldTransform_Body_.quaternion_ = Normalize(worldTransform_Body_.quaternion_);

	if (behavior_ != Behavior::kAttack) {
		worldTransform_Body_.UpdateMatrix(RotationType::Quaternion);
		worldTransform_Head_.UpdateMatrix(RotationType::Quaternion);
		worldTransform_Larm_.UpdateMatrix(RotationType::Quaternion);
		worldTransform_Rarm_.UpdateMatrix(RotationType::Quaternion);
		worldTransform_Weapon_.UpdateMatrix(RotationType::Quaternion);
	}
	else {
		worldTransform_Body_.UpdateMatrix(RotationType::Quaternion);
		worldTransform_Head_.UpdateMatrix(RotationType::Euler);
		worldTransform_Larm_.UpdateMatrix(RotationType::Euler);
		worldTransform_Rarm_.UpdateMatrix(RotationType::Euler);
		worldTransform_Weapon_.UpdateMatrix(RotationType::Euler);
	}


	ApplyGlobalVariables();
	joyStatePre_ = joyState_;

}

void Player::Draw(const ViewProjection& viewProjection)
{
	if (isAlive_) {
		if (behavior_ == Behavior::kAttack) {
			models_[4]->Draw(worldTransform_Weapon_, viewProjection);
		}
		models_[0]->Draw(worldTransform_Body_, viewProjection);
		models_[1]->Draw(worldTransform_Head_, viewProjection);
		models_[2]->Draw(worldTransform_Larm_, viewProjection);
		models_[3]->Draw(worldTransform_Rarm_, viewProjection);
	}

}

void Player::Reset()
{

	worldTransform_Body_.translation_ = { 0.0f, 0.0f, 0.0f };

	worldTransform_Head_.parent_ = &worldTransform_Body_;
	worldTransform_Head_.translation_.y = 4.4f;
	worldTransform_Larm_.parent_ = &worldTransform_Body_;
	worldTransform_Larm_.translation_ = Vector3(0.2f, 3.4f, 0.0f);
	worldTransform_Rarm_.parent_ = &worldTransform_Body_;
	worldTransform_Rarm_.translation_ = Vector3(-0.2f, 3.4f, 0.0f);

	worldTransform_Body_.UpdateMatrix(RotationType::Quaternion);
	worldTransform_Head_.UpdateMatrix(RotationType::Quaternion);
	worldTransform_Larm_.UpdateMatrix(RotationType::Quaternion);
	worldTransform_Rarm_.UpdateMatrix(RotationType::Quaternion);
	worldTransform_Weapon_.UpdateMatrix(RotationType::Quaternion);

	sphere_ = {
		.center{worldTransform_Body_.translation_},
		.radius{0.5f}
	};

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
	isAlive_ = true;

	velocity_ = { 0.0f, 0.0f, 0.0f };

}

void Player::OnCollision(uint32_t collisionAttribute)
{
	if (collisionAttribute == kCollisionAttributeEnemy) {
		isAlive_ = false;
	}
	else if (collisionAttribute == kCollisionAttributeFloor) {
		isLanding_ = true;
		worldTransform_Body_.translation_.y = 0.0f;
	}
	else if (collisionAttribute == kCollisionAttributeMovingFloor) {
		isLanding_ = true;
		isMove_ = true;
		worldTransform_Body_.translation_.y = 0.0f;
	}
	else if (collisionAttribute == kCollisionAttributeGoal) {
		isAlive_ = false;
	}

}

void Player::OnCollision(WorldTransform* worldTransform)
{
	isLanding_ = true;
	isMove_ = true;
	worldTransform_Body_.translation_.y = 0.0f;
	if (worldTransform_Body_.parent_ == nullptr) {
		worldTransform_Body_.translation_ = Subtract(worldTransform_Body_.translation_, worldTransform->translation_);
	}
	worldTransform_Body_.parent_ = worldTransform;
}

void Player::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	workDash_.speed = globalVariables->GetFloatValue(groupName, "DashSpeed");
	nlohmann::json json;
	json.clear();
}

void Player::BehaviorRootInitialize()
{

	worldTransform_Larm_.rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransform_Rarm_.rotation_ = { 0.0f, 0.0f, 0.0f };
	worldTransform_Weapon_.rotation_ = { 0.0f, 0.0f, 0.0 };

}

void Player::BehaviorRootUpdate()
{
	velocity_.x = 0.0f;
	velocity_.z = 0.0f;
	acceleration_ = { 0.0f, gravity_, 0.0f };

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	// ジョイスティック状態取得
	if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) {
				behaviorRequest_ = Behavior::kAttack;
			}
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				behaviorRequest_ = Behavior::kDash;
			}

			const float threshold = 0.7f;
			bool isMoving = false;

			// 速さ
			const float speed = 0.3f;

			// 移動量
			Vector3 move = { (float)joyState.Gamepad.sThumbLX, 0, (float)joyState.Gamepad.sThumbLY };

			if (Length(move) > threshold) {
				isMoving = true;
			}

			if (isMoving) {

				// 移動量に速さを反映
				move = Multiply(speed, Normalize(move));

				Matrix4x4 rotate = MakeRotateYMatrix(viewProjection_->rotation_.y);

				move = Transform(move, rotate);

				// 移動成分
				acceleration_ = Add(acceleration_, move);

				move = Normalize(move);
				Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, move));
				float dot = Dot({ 0.0f, 0.0f, 1.0f }, move);
				moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
			}
			else if (lockOn_ && lockOn_->ExistTarget()) {
				// ロックオン座標
				Vector3 lockOnPos = lockOn_->GetTargetPosition();

				Vector3 sub = Subtract(lockOnPos, Vector3{ worldTransform_Body_.matWorld_.m[3][0], worldTransform_Body_.matWorld_.m[3][1], worldTransform_Body_.matWorld_.m[3][2] });


				sub.y = 0.0f;
				sub = Normalize(sub);
				Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, sub));
				float dot = Dot({ 0.0f, 0.0f, 1.0f }, sub);
				moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
			}
		}
	}

	if (isLanding_) {
		acceleration_.y = 0.0f;
		if (Input::GetInstance()->IsControllerConnected()) {
			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
					velocity_.y = jumpValue_;
				}
			}
		}
	}


	if (!isMove_) {
		if (worldTransform_Body_.parent_) {
			Matrix4x4 invers = Inverse(worldTransform_Body_.parent_->matWorld_);
			Matrix4x4 lMat = Multiply(worldTransform_Body_.matWorld_, invers);
			Vector3 t = { lMat.m[3][0], lMat.m[3][1], lMat.m[3][2] };
			Vector3 t2 = { worldTransform_Body_.parent_->matWorld_.m[3][0], worldTransform_Body_.parent_->matWorld_.m[3][1], worldTransform_Body_.parent_->matWorld_.m[3][2] };
			worldTransform_Body_.translation_ = Add(t, t2);
			worldTransform_Body_.parent_ = nullptr;
		}
	}



	velocity_ = Add(velocity_, acceleration_);
	if (velocity_.y < acceleration_.y) {
		velocity_.y = acceleration_.y;
	}
	worldTransform_Body_.translation_ = Add(worldTransform_Body_.translation_, velocity_);

	if (worldTransform_Body_.translation_.y < -20.0f) {
		isAlive_ = false;
	}

	worldTransform_Body_.UpdateMatrix(RotationType::Quaternion);
	if (worldTransform_Body_.parent_) {
		Matrix4x4 invers = Inverse(worldTransform_Body_.parent_->matWorld_);
		Matrix4x4 lMat = Multiply(worldTransform_Body_.matWorld_, invers);
		Vector3 t = { lMat.m[3][0], lMat.m[3][1], lMat.m[3][2] };
		Vector3 t2 = { worldTransform_Body_.parent_->matWorld_.m[3][0], worldTransform_Body_.parent_->matWorld_.m[3][1], worldTransform_Body_.parent_->matWorld_.m[3][2] };
		worldTransform_.translation_ = Add(t, t2);
		worldTransform_.quaternion_ = worldTransform_Body_.quaternion_;
		worldTransform_.UpdateMatrix(RotationType::Quaternion);
		worldTransform_Head_.parent_ = &worldTransform_;
		worldTransform_Larm_.parent_ = &worldTransform_;
		worldTransform_Rarm_.parent_ = &worldTransform_;
		worldTransform_Weapon_.parent_ = &worldTransform_;
	}
	else {
		worldTransform_Head_.parent_ = &worldTransform_Body_;
		worldTransform_Larm_.parent_ = &worldTransform_Body_;
		worldTransform_Rarm_.parent_ = &worldTransform_Body_;
		worldTransform_Weapon_.parent_ = &worldTransform_Body_;
	}

	sphere_.center = { worldTransform_Body_.matWorld_.m[3][0], worldTransform_Body_.matWorld_.m[3][1], worldTransform_Body_.matWorld_.m[3][2] };
	isLanding_ = false;
	isMove_ = false;

}

void Player::BehaviorAttackInitialize()
{
	workAttack_.parameter = 0.0f;
	workAttack_.recoveryTimer = 0;
	workAttack_.comboIndex = 0;

}

void Player::BehaviorAttackUpdate()
{

	worldTransform_Larm_.rotation_ = Lerp(workAttack_.start_Arm[workAttack_.comboIndex], workAttack_.end_Arm[workAttack_.comboIndex], workAttack_.parameter);
	worldTransform_Rarm_.rotation_ = worldTransform_Larm_.rotation_;
	worldTransform_Weapon_.rotation_ = Lerp(workAttack_.start_Weapon[workAttack_.comboIndex], workAttack_.end_Weapon[workAttack_.comboIndex], workAttack_.parameter);

	if (workAttack_.parameter < 1.0f) {
		if (workAttack_.parameter != 0.0f) {
			if (workAttack_.comboIndex < ComboNum) {
				if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && !(joyStatePre_.Gamepad.wButtons & XINPUT_GAMEPAD_Y)) {
					if (workAttack_.comboIndex == 0) {
						workAttack_.comboNext = true;
					}
					else if (workAttack_.comboIndex == 1) {
						workAttack_.comboNext = true;
					}
				}
			}
		}
		workAttack_.parameter += 0.04f;


		Vector3 rotation = worldTransform_Weapon_.rotation_;
		rotation.y = worldTransform_Body_.rotation_.y;
		Matrix4x4 rotate = MakeRotateXYZMatrix(rotation);
		obb_.orientations[0].x = rotate.m[0][0];
		obb_.orientations[0].y = rotate.m[0][1];
		obb_.orientations[0].z = rotate.m[0][2];
		obb_.orientations[1].x = rotate.m[1][0];
		obb_.orientations[1].y = rotate.m[1][1];
		obb_.orientations[1].z = rotate.m[1][2];
		obb_.orientations[2].x = rotate.m[2][0];
		obb_.orientations[2].y = rotate.m[2][1];
		obb_.orientations[2].z = rotate.m[2][2];
	}
	else if (workAttack_.comboNext) {
		workAttack_.comboNext = false;
		workAttack_.comboIndex++;

		workAttack_.parameter = 0.0f;

		workAttack_.anticipationTimer = 0;
		workAttack_.chargeTimer = 0;
		workAttack_.recoveryTimer = 0;
		workAttack_.swingTimer = 0;

		const float threshold = 0.7f;
		bool isMoving = false;

		// 速さ
		float speed = 0.3f;

		// 移動量
		Vector3 move = { (float)joyState_.Gamepad.sThumbLX, 0, (float)joyState_.Gamepad.sThumbLY };

		if (Length(move) > threshold) {
			isMoving = true;
		}

		if (lockOn_ && lockOn_->ExistTarget()) {
			// ロックオン座標
			Vector3 lockOnPos = lockOn_->GetTargetPosition();

			Vector3 sub = Subtract(lockOnPos, Vector3{ worldTransform_Body_.matWorld_.m[3][0], worldTransform_Body_.matWorld_.m[3][1], worldTransform_Body_.matWorld_.m[3][2] });

			// 距離
			float distance = Length(sub);
			// 距離しきい値
			const float threshold = 0.2f;

			// しきい値より離れているときのみ
			if (distance > threshold) {
				// Y軸回り角度
				sub.y = 0.0f;
				sub = Normalize(sub);
				Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, sub));
				float dot = Dot({ 0.0f, 0.0f, 1.0f }, sub);
				moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));

				//worldTransform_Body_.rotation_.y = std::atan2(sub.x, sub.z);

				// しきい値を超える速さはら補正する
				if (speed > distance - threshold) {
					// ロックオン対象へのめり込み防止
					speed = distance - threshold;
				}


			}

		}


		if (isMoving) {

			// 移動量に速さを反映
			move = Multiply(speed, Normalize(move));

			Matrix4x4 rotate = MakeRotateYMatrix(viewProjection_->rotation_.y);

			move = Transform(move, rotate);

			// 移動成分
			acceleration_ = Add(acceleration_, move);

			move = Normalize(move);
			Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, move));
			float dot = Dot({ 0.0f, 0.0f, 1.0f }, move);
			moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acos(dot));
			

			
		}

		sphere_.center = { worldTransform_Body_.matWorld_.m[3][0], worldTransform_Body_.matWorld_.m[3][1], worldTransform_Body_.matWorld_.m[3][2] };

	}
	else if (++workAttack_.recoveryTimer < kConstAttacks_[workAttack_.comboIndex].recoveryTime) {
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) && !(joyStatePre_.Gamepad.wButtons & XINPUT_GAMEPAD_Y)) {
			if (workAttack_.comboIndex == 0) {
				workAttack_.comboNext = true;
			}
			else if (workAttack_.comboIndex == 1) {
				workAttack_.comboNext = true;
			}
		}
	}
	else {
		behaviorRequest_ = Behavior::kRoot;
		obb_.orientations[0].x = 0.0f;
		obb_.orientations[0].y = 0.0f;
		obb_.orientations[0].z = 0.0f;
		obb_.orientations[1].x = 0.0f;
		obb_.orientations[1].y = 0.0f;
		obb_.orientations[1].z = 0.0f;
		obb_.orientations[2].x = 0.0f;
		obb_.orientations[2].y = 0.0f;
		obb_.orientations[2].z = 0.0f;

	}
	obb_.center = { worldTransform_Weapon_.matWorld_.m[3][0], worldTransform_Weapon_.matWorld_.m[3][1], worldTransform_Weapon_.matWorld_.m[3][2] };

	obb_.size = { 1.0f, 5.0f, 1.0f };

}

void Player::BehaviorDashInitialize()
{
	workDash_.parameter = 0.0f;
	workDash_.time = 10.0f;
	worldTransform_Body_.rotation_.y = destinationAngleY_;
}

void Player::BehaviorDashUpdate()
{

	// 移動量に速さを反映
	Vector3 move(0.0f, 2.0f, workDash_.speed);

	move = TransformNormal(move, worldTransform_Body_.matWorld_);

	// 移動
	worldTransform_Body_.translation_ = Add(worldTransform_Body_.translation_, move);

	isLanding_ = false;
	isMove_ = false;

	if (++workDash_.parameter >= workDash_.time) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

