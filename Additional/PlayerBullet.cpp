#include "PlayerBullet.h"
#include <assert.h>
#include "CollisionConfig.h"
#include "Enemy.h"

void PlayerBullet::Initialize(Camera* camera, const Vector3& positino, const Vector3 velocity) {
	
	model_.reset(Model::Create("startBox"));
	model_->SetCamera(camera);

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = positino;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	Create(&worldTransform_, Type::kAABB, RotationType::Euler, camera);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void PlayerBullet::Initialize(Camera* camera, const Vector3& position, const Vector3 velocity, Enemy* enemy)
{
	model_.reset(Model::Create("startBox"));
	model_->SetCamera(camera);

	worldTransform_.Initialize();

	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	// 引数で受け取った初期座標をセット
	enemy_ = enemy;

	Create(&worldTransform_, Type::kAABB, RotationType::Euler, camera);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);
}

void PlayerBullet::Update() {

	if (enemy_ == nullptr) {
		// 座標を移動させる(1フレーム分の移動量を足しこむ)
		worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
		worldTransform_.UpdateMatrix(RotationType::Euler);

	}
	else {
		float BulletSpeed = 0.5f;
		// 敵弾から自キャラらへのベクトルを計算
		Vector3 toPlayer = Subtract(enemy_->GetWorldPosition(), GetWorldPosition());
		// ベクトルを正規化する
		Normalize(toPlayer);
		Normalize(velocity_);
		// 球面線形補間により、今の速度と自キャラへのベクトルを内挿し、新たな速度とする
		velocity_ = Multiply(BulletSpeed, Slerp(velocity_, toPlayer, 0.5f));
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
	}
	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw() {
	model_->Draw(worldTransform_);
}

void PlayerBullet::OnCollision() {
	//isDead_ = true;
}

