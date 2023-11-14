#include "PlayerWeapon.h"
#include "Input.h"

void PlayerWeapon::Initialize(const std::vector<Model*>& models)
{
	BaseObject::Initialize(models);
	isActive_ = false;
	parameter_ = 0.0f;

	obb_.center = worldTransform_.translation_;
	Matrix4x4 rotate = MakeRotateXYZMatrix(worldTransform_.rotation_);
	obb_.orientations[0].x = rotate.m[0][0];
	obb_.orientations[0].x = rotate.m[0][1];
	obb_.orientations[0].x = rotate.m[0][2];
	obb_.orientations[1].x = rotate.m[0][0];
	obb_.orientations[1].x = rotate.m[0][1];
	obb_.orientations[1].x = rotate.m[0][2];
	obb_.orientations[2].x = rotate.m[0][0];
	obb_.orientations[2].x = rotate.m[0][1];
	obb_.orientations[2].x = rotate.m[0][2];

	obb_.size = { 1.0f, 5.0f, 1.0f };

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void PlayerWeapon::Update()
{
	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;
	if (Input::GetInstance()->IsControllerConnected()) {
		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
				isActive_ = true;
			}
		}
	}

	if (isActive_) {
		const float weapon_start = -0.2f;
		const float weapon_end = 1.574f;
		worldTransform_.rotation_.x = weapon_start + (weapon_end - weapon_start) * parameter_;
		if (parameter_ < 1.0f) {
			parameter_ += 0.04f;
		}
		else {
			isActive_ = false;
			parameter_ = 0.0f;
		}
		obb_.center = worldTransform_.translation_;
		Matrix4x4 rotate = MakeRotateXYZMatrix(worldTransform_.rotation_);
		obb_.orientations[0].x = rotate.m[0][0];
		obb_.orientations[0].x = rotate.m[0][1];
		obb_.orientations[0].x = rotate.m[0][2];
		obb_.orientations[1].x = rotate.m[0][0];
		obb_.orientations[1].x = rotate.m[0][1];
		obb_.orientations[1].x = rotate.m[0][2];
		obb_.orientations[2].x = rotate.m[0][0];
		obb_.orientations[2].x = rotate.m[0][1];
		obb_.orientations[2].x = rotate.m[0][2];

		obb_.size = { 1.0f, 5.0f, 1.0f };
	}


}

void PlayerWeapon::Draw(const ViewProjection& viewProjection)
{
	if (isActive_) {
		models_[0]->Draw(worldTransform_, viewProjection);
	}
}
