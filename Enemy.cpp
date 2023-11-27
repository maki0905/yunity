#include "Enemy.h"
#include "CollisionConfig.h"
#include "ImGuiManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

void Enemy::Initialize(const std::vector<Model*>& models)
{
	
	BaseObject::Initialize(models);

	worldTransform_Body_.Initialize();
	worldTransform_Larm_.Initialize();
	worldTransform_Rarm_.Initialize();
	worldTransform_Body_.translation_.y = 2.0f;
	worldTransform_Body_.translation_.z = 115.0f;
	worldTransform_Body_.translation_.x = -15.0f;
	worldTransform_Larm_.parent_ = &worldTransform_Body_;
	worldTransform_Rarm_.parent_ = &worldTransform_Body_;

	angle_ = 0.0f;
	angularVelocity_ = 1.0f;
	sphere_ = {
		.center{worldTransform_Body_.translation_},
		.radius{5.0f}
	};
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
	isActive_ = true;
	isDestroy_ = false;
	isHit_ = false;
}

void Enemy::Update()
{
	
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(M_PI) / period;
	// パラメータを1ステップ分加算
	flightingParameter_ += step;
	// 2πを超えたらθに戻す
	flightingParameter_ = std::fmod(flightingParameter_, float(2.0f * M_PI));
	// 浮遊を座標に反映
	worldTransform_Larm_.quaternion_.z = std::sin(flightingParameter_) * (amplitude / 2.0f);
	worldTransform_Rarm_.quaternion_.z = -worldTransform_Larm_.quaternion_.z;

	if (isDestroy_) {
		if (++workDestroy_.parameter >= effectTime) {
			isActive_ = false;
		}
		else {
			movePre_ = Multiply(1.0f, Normalize(movePre_));
			workDestroy_.angle += 1.0f;
			//worldTransform_Body_.translation_ = Add(worldTransform_Body_.translation_, movePre_);
			//moveQuaternion_ = MakeRotateAxisAngleQuaternion({ 0.0f, 1.0f, 0.0f }, workDestroy_.angle);
			color.w = (effectTime - workDestroy_.parameter) / effectTime;
		}
	}
	else {
		Move();
	}

	worldTransform_Body_.quaternion_ = Slerp(worldTransform_Body_.quaternion_, moveQuaternion_, 0.3f);
	worldTransform_Body_.quaternion_ = Normalize(worldTransform_Body_.quaternion_);

	worldTransform_Body_.UpdateMatrix(RotationType::Quaternion);
	worldTransform_Larm_.UpdateMatrix(RotationType::Quaternion);
	worldTransform_Rarm_.UpdateMatrix(RotationType::Quaternion);
	sphere_.center = worldTransform_Body_.translation_;

	if (isHit_) {
		coolTime_--;
		if (coolTime_ == 0) {
			isHit_ = false;
			coolTime_ = 20;
		}
	}

	for (auto& model : models_) {
		model->SetMaterial(color);
	}

	ImGui::Begin("Enemy");
	ImGui::Text("HP : %o", HP_);
	ImGui::Text("coolTime : %d", coolTime_);
	ImGui::DragFloat4("color", &color.x, 0.01f);
	ImGui::End();

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	if (isActive_) {
		models_[0]->Draw(worldTransform_Body_, viewProjection);
		models_[1]->Draw(worldTransform_Rarm_, viewProjection);
		models_[2]->Draw(worldTransform_Larm_, viewProjection);
	}

}

void Enemy::OnCollision(uint32_t collisionMask)
{
	//isActive_ = false;
	//isDestroy_ = true;
	if (!isHit_) {
		HP_--;
		isHit_ = true;
	}
	if (HP_ <= 0) {
		isDestroy_ = true;
	}
	workDestroy_.parameter = 0;
}

void Enemy::Move()
{
	int period = 60;
	float radius = 10.0f;
	angle_ += angularVelocity_ * (1.0f / float(period));

	Vector3 move = { 0.05f, 0.0f, 0.0f };
	
	Matrix4x4 roatateMatrix = MakeRotateYMatrix(angle_);
	move = TransformNormal(move, roatateMatrix);
	worldTransform_Body_.translation_ = Add(worldTransform_Body_.translation_, move);
	movePre_ = move;

	move = Normalize(move);
	Vector3 cross = Normalize(Cross({ 0.0f, 0.0f, 1.0f }, move));
	float dot = Dot({ 0.0f, 0.0f, 1.0f }, move);
	moveQuaternion_ = MakeRotateAxisAngleQuaternion(cross, std::acosf(dot));

}

void Enemy::Reset()
{
	isActive_ = true;
	isDestroy_ = false;
	worldTransform_Body_.translation_ = startPos_;
	HP_ = 3;
	coolTime_ = 20;
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void Enemy::SetTranslation(const Vector3& translation)
{
	worldTransform_Body_.translation_ = translation;
	startPos_ = translation;
}

Vector3 Enemy::GetModelCenter() const
{
	// 見た目上の中心点オフセット(モデル座標系)
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_Body_.matWorld_);
	return worldPos;
}
