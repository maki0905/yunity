#include "FollowCamera.h"

void FollowCamera::Initialize(const ViewProjection& viewProjection)
{
	viewProjection_ = viewProjection;
	// シングルトンインスタンスを取得
	input_ = Input::GetInstance();
}

void FollowCamera::Update()
{
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	// ジョイスティック状態取得
	if (Input::GetInstance()->IsControllerConnected()) {
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
				LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.5f);
		}
	}

	if (target_) {
		// 追従座標の補間
		//interTarget_ = Lerp(interTarget_, target_->translation_, 0.5f);
		Vector3 translation = { target_->matWorld_.m[3][0], target_->matWorld_.m[3][1], target_->matWorld_.m[3][2] };
		interTarget_ = Lerp(interTarget_, translation, 0.5f);
	}

	// 追従対象からのおふセット
	Vector3 offset = Offset();

	// カメラ座標
	viewProjection_.translation_ = Add(interTarget_, offset);

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
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
