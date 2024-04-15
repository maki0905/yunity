#include "Enemy.h"
#include <cassert>
#include "Player.h"
#include <iostream>
#include "TimedCall.h"
#include "CollisionConfig.h"
#include "StageScene.h"

void EnemyPhaseApproach::Update(Enemy* pEnemy) {
	Vector3 move = { 0.0f, 0.0f, -0.5f };
	// 移動(ベクトルを加算)
	pEnemy->Move(move);
	// 規定の一に到達したら離脱
	if (pEnemy->GetPosition().z < 0.0f) {
		pEnemy->chageState(new EnemyPhaseLeave());
		pEnemy->TimedCallClear();
	}
}

void EnemyPhaseLeave::Update(Enemy* pEnemy) {
	Vector3 move = { -0.2f, 0.1f, -0.4f };
	// 移動(ベクトルを加算)
	pEnemy->Move(move);
}


Enemy::Enemy() {
	state_ = new EnemyPhaseApproach();
}

// bullet_の解放
Enemy::~Enemy() {
	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}

}

void Enemy::Initialize(Camera* camera, Vector3 position) {
	camera_ = camera;

	model_.reset(Model::Create("endBox"));
	model_->SetCamera(camera);

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	Create(&worldTransform_, Type::kAABB, RotationType::Euler, camera);

	isLockon_ = false;

	sprite2DReticle_ = std::make_unique<Sprite>();
	sprite2DReticle_.reset(Sprite::Create(TextureManager::GetInstance()->Load("lockOn.png"), { 0.0f, 0.0f }));
	sprite2DReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	sprite2DReticle_->SetSize(Vector2(50.0f, 50.0f));

	// 接近フェーズ初期化
	ApproachPhaseInitialize();

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeEnemy);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributeEnemy);
}

// staticで宣言したメンバ関数ポインタテーブルの実体
void (Enemy::* Enemy::phaseFunc[])() = {
	&Enemy::ApproachPhase, // 接近
	&Enemy::LeavePhase     // 離脱
};

void Enemy::Update() {

	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };

	// メンバ関数ポインタに入っている関数を呼び出す
	//(this->*phaseFunc[static_cast<size_t>(phase_)])();
	state_->Update(this);



	// 終了したタイマーを削除
	timedCalls_.remove_if([](TimedCall* timedCall) {
		if (timedCall->IsFinished()) {
			delete timedCall;
			return true;
		}
		return false;
		});

	// 範囲forでリストの全要素について回す
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Updata();
	}

	// 移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 15.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix(RotationType::Euler);

	Vector3 positionReticle = GetWorldPosition();
	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
		Multiply(Multiply(camera_->GetViewMatrix(), camera_->GetProjectionMatrix()), matViewport);

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = TransformVector3(positionReticle, matViewProjectionViewport);
	if (!isLockon_) {
		float distance = sqrtf(DistanceSquared(player_->GetReticle(), Vector2(positionReticle.x, positionReticle.y)));

		if (distance <= 50.0f) {
			isLockon_ = true;
		}
	}
	else {
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

}

void Enemy::Draw() {
	model_->Draw(worldTransform_);
}

void Enemy::ApproachPhase() {
	Vector3 move = { 0.0f, 0.0f, -0.5f };
	// 移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// 規定の一に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Enemy::Phase::Leave;
	}
}


void Enemy::LeavePhase() {
	Vector3 move = { -0.2f, 0.1f, -0.4f };
	// 移動(ベクトルを加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::Fire() {

	assert(player_);


	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	Vector3 enemyPos = GetWorldPosition();
	// 敵キャラ->自キャラの差分ベクトルを求める
	Vector3 velocity = Subtract(playerPos, enemyPos);
	// ベクトルの正規化
	velocity = Normalize(velocity);
	// ベクトルの長さを、早さに合わせる
	velocity = Multiply(kBulletSpeed, velocity);


	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(camera_, worldTransform_.translation_, velocity);

	// 弾を登録する
	stageScene_->AddEnemyBullet(newBullet);
}

void Enemy::FireReset() {
	// 弾を発射する
	Fire();
	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireReset, this), 60));
}

void Enemy::ApproachPhaseInitialize() {
	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireReset, this), 60));

}

void Enemy::chageState(EnemyState* newState) {
	state_ = newState;
}

void Enemy::Move(const Vector3 move) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}

void Enemy::TimedCallClear() {
	timedCalls_.clear();

}

void Enemy::DrawLockon()
{
	if (isLockon_) {
		sprite2DReticle_->Draw();
	}
}


