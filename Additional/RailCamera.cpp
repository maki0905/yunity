#include "RailCamera.h"
#include "Vector3.h"
#include "MathFunction.h"


void RailCamera::Initialize(Camera* camera, const Vector3& position, const Vector3& rotation) {
	// ワールドトランスフォームの初期化設定
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;

	camera_ = camera;

	controlPoints_ = {
		/*{0,  0,  0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0,  0},
		{30, 0,  0}*/
		{0,   10, -100},
		{0,   10, -80 },
		{0,   20, -65 },
		{0,  10, -20 },
		{0, 0,  0},
		{0, 0,  10},
		{10, 0,  10  },
		{30, 30, 10  },
		{50, 0, 5},
		{60, -10, 0},
		{55, 0, -30},
		{30, 10, -50},
		{-20, -20, -80},
		{0, 10, -100},
		{0,   10,  -80 }
	};
	eyeIndex_ = 0;
	targetIndex_ = 0;
	rangeManagemant_ = 0;
	targetPoint_ = 5;
}

void RailCamera::Update(/*ViewProjection* viewProjection*/) {
	Vector3 velocity = { 0.0f, 0.0f, 0.1f };
	Vector3 rotation = { 0.0f, 0.0f, 0.0f };


	//// 線分の数
	const size_t segmentCount = 100;

	if (rangeManagemant_ != 999) {
		float t = 1.0f / segmentCount * rangeManagemant_;
		Vector3 eye = CatmullRom(
			controlPoints_[eyeIndex_], controlPoints_[eyeIndex_ + 1], controlPoints_[eyeIndex_ + 2],
			controlPoints_[eyeIndex_ + 3], t);
		t = 1.0f / segmentCount * ((rangeManagemant_ + targetPoint_) % segmentCount);
		if ((rangeManagemant_ + targetPoint_) % segmentCount == 0 &&
			targetIndex_ < controlPoints_.size() / 2 - 1) {
			targetIndex_++;
		}
		Vector3 target = CatmullRom(
			controlPoints_[targetIndex_], controlPoints_[targetIndex_ + 1],
			controlPoints_[targetIndex_ + 2], controlPoints_[targetIndex_ + 3], t);

		rangeManagemant_++;
		if (rangeManagemant_ == segmentCount + 1) {
			rangeManagemant_ = 0;
			if (eyeIndex_ == controlPoints_.size() / 2 - 1) {
				rangeManagemant_ = 999;
			}

			if (eyeIndex_ < controlPoints_.size() / 2 - 1) {
				eyeIndex_++;
			}
		}
		Matrix4x4 eyeMatrix = MakeIdentity4x4();
		eyeMatrix.m[3][0] = eye.x;
		eyeMatrix.m[3][1] = eye.y;
		eyeMatrix.m[3][2] = eye.z;

		worldTransform_.translation_ = eye;

		velocity = Subtract(target, eye);
		velocity = Normalize(velocity);
		worldTransform_.rotation_.y = std::atan2(velocity.x, velocity.z);
		float lenghtXZ = Length(Vector3(velocity.x, 0.0f, velocity.z));
		worldTransform_.rotation_.x = std::atan2(-velocity.y, lenghtXZ);

		// カメラオブジェクトのワールド行列からビュー行列を計算する
		camera_->SetTranslate(worldTransform_.translation_);
		camera_->SetRotate(worldTransform_.rotation_);
	}

	worldTransform_.UpdateMatrix(RotationType::Euler);

	// ビュープロジェクションを転送
	camera_->Update();
}
