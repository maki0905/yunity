#include "FollowCamera.h"
#include "GlobalVariables.h"
#include "Externals/nlohmann/json.hpp"
#include "LockOn.h"

void FollowCamera::Initialize(const ViewProjection& viewProjection)
{
	viewProjection_ = viewProjection;
	// シングルトンインスタンスを取得
	input_ = Input::GetInstance();

	delayAmount_ = 0.5f;

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	// グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "DelayAmount", delayAmount_);

}

void FollowCamera::Update()
{
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	
	if (lockOn_) {
		// ロックオン座標
		Vector3 lockOnPos = lockOn_->GetTargetPosition();

		// 追従対象からロックオン対象へのベクトル
		Vector3 sub = Subtract(lockOnPos, target_->translation_);

		/*sub.y = 0.0f;
		sub = Normalize(sub);
		float dot = Dot({ 0.0f, 0.0f, 1.0f }, sub);
		viewProjection_.rotation_.y = std::acosf(dot);*/

		// Y軸回り角度
		viewProjection_.rotation_.y = std::atan2(sub.x, sub.z);

		lockOn_ = nullptr;

	}
	else if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			// 速さ
			const float speed = 0.000001f;

			destinationAngleY_ += (float)joyState.Gamepad.sThumbRX * speed;

			// 右スティック押し込みでリセット
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
				Reset();
			}

			// 最短角度補間
			viewProjection_.rotation_.y =
				LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, delayAmount_);
			
		}
	}

	if (target_) {
		// 追従座標の補間
		//interTarget_ = Lerp(interTarget_, target_->translation_, 0.5f);
		Vector3 translation = { target_->matWorld_.m[3][0], target_->matWorld_.m[3][1], target_->matWorld_.m[3][2] };
		interTarget_ = Lerp(interTarget_, translation, delayAmount_);
	}

	// 追従対象からのオフセット
	Vector3 offset = Offset();

	// カメラ座標
	viewProjection_.translation_ = Add(interTarget_, offset);

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();

	ApplyGlobalVariables();
}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}

Vector3 FollowCamera::Offset() const
{
	// 追従対象からカメラまでのオフセット
	Vector3 offset = { 0.0f, 10.0f, -30.0f };

	Matrix4x4 rotate = MakeRotateYMatrix(viewProjection_.rotation_.y);

	// オフセットをカメラの回転に合わせて回転させる
	offset = TransformNormal(offset, rotate);
	//offset = TransformNormal(offset, rotateXY);
	return offset;
}

void FollowCamera::Reset()
{
	// 追従対象がいれば
	if (target_) {
		// 追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}
	destinationAngleY_ = viewProjection_.rotation_.y;

	// 追従対象からのオフセット
	Vector3 offset = Offset();
	viewProjection_.translation_ = Add(interTarget_, offset);
}

void FollowCamera::ApplyGlobalVariables()
{
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	delayAmount_ = globalVariables->GetFloatValue(groupName, "DelayAmount");
	nlohmann::json json;
	json.clear();
}
