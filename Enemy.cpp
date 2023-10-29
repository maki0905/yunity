#include "Enemy.h"
#include "CollisionConfig.h"

#define _USE_MATH_DEFINES
#include <math.h>

void Enemy::Initialize(const std::vector<Model*>& models)
{
	
	BaseObject::Initialize(models);

	worldTransform_Body_.Initialize();
	worldTransform_Larm_.Initialize();
	worldTransform_Rarm_.Initialize();

	worldTransform_Larm_.parent_ = &worldTransform_Body_;
	worldTransform_Rarm_.parent_ = &worldTransform_Body_;

	angle_ = 0.0f;
	angularVelocity_ = 1.0f;
	sphere_ = {
		.center{worldTransform_Body_.translation_},
		.radius{2.0f}
	};
	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
}

void Enemy::Update()
{
	Move();
	worldTransform_Body_.translation_.z += 90.0f;
	// 1フレームでのパラメータ加算値
	const float step = 2.0f * float(M_PI) / period;
	// パラメータを1ステップ分加算
	flightingParameter_ += step;
	// 2πを超えたらθに戻す
	flightingParameter_ = std::fmod(flightingParameter_, float(2.0f * M_PI));
	// 浮遊を座標に反映
	worldTransform_Larm_.rotation_.z = std::sin(flightingParameter_) * (amplitude / 2.0f);
	worldTransform_Rarm_.rotation_.z = -worldTransform_Larm_.rotation_.z;


	worldTransform_Body_.UpdateMatrix();
	worldTransform_Larm_.UpdateMatrix();
	worldTransform_Rarm_.UpdateMatrix();
	sphere_.center = worldTransform_Body_.translation_;
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	models_[1]->Draw(worldTransform_Larm_, viewProjection);
	models_[2]->Draw(worldTransform_Rarm_, viewProjection);
	models_[0]->Draw(worldTransform_Body_, viewProjection);

}

void Enemy::Move()
{
	int period = 60;
	float radius = 10.0f;
	angle_ -= angularVelocity_ * (1.0f / float(period));

	Vector3 move = {
		angularVelocity_ * std::sin(angle_), 0.0f,
		-angularVelocity_ * std::cos(angle_)
	};

	// Y軸周り角度(θy)
	worldTransform_Body_.rotation_.y = std::atan2(move.x, move.z);

	worldTransform_Body_.translation_.x = std::cos(angle_) * radius;
	worldTransform_Body_.translation_.z = radius + std::sin(angle_) * radius;

}
