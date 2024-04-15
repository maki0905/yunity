#include "EnemyBullet.h"

#include "Player.h"

void EnemyBullet::Initialize(Camera* camera, const Vector3& position, const Vector3& velocity)
{
	model_.reset(Model::Create("startBox"));
	model_->SetCamera(camera);

	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	// 横軸方向の長さを求める	
	float lenghtXZ = Length(Vector3(velocity_.x, 0.0f, velocity_.z));
	// X軸周りの角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, lenghtXZ);

	Create(&worldTransform_, Type::kAABB, RotationType::Euler, camera);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);

}

void EnemyBullet::Update()
{
	float BulletSpeed = 0.5f;
	// 敵弾から自キャラらへのベクトルを計算
	Vector3 toPlayer = Subtract(player_->GetWorldPosition(), GetWorldPosition());
	// ベクトルを正規化する
	Normalize(toPlayer);
	Normalize(velocity_);
	// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
	velocity_ = Multiply(BulletSpeed, Slerp(velocity_, toPlayer, 0.03f));
	// 進行方向に見た目の回転を合わせる
	// Y軸周り角度(θy)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	// 横軸方向の長さを求める
	float lenghtXZ = Length(Vector3(velocity_.x, 0.0f, velocity_.z));
	// X軸周りの角度(θx)
	worldTransform_.rotation_.x = std::atan2(-velocity_.y, lenghtXZ);

	// 座標を移動させる
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix(RotationType::Euler);
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;

	}
}

void EnemyBullet::Draw()
{
	model_->Draw(worldTransform_, TextureManager::GetInstance()->Load("Red1x1.png"));
}

void EnemyBullet::OnCollision() {
	isDead_ = true;
}