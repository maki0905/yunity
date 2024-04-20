#include "Player.h"

#include "ImGuiManager.h"
#include "MathFunction.h"
#include "Enemy.h"

void Player::Initialize(Camera* camera)
{
	model_.reset(Model::Create("startBox"));
	model_->SetCamera(camera);
	camera_ = camera;

	worldTransfrom_.Initialize();
	worldTransfrom_.translation_.z = 0.0f;

	attackTime_ = 0;

	worldTransform3DReticle_.Initialize();

	sprite2DReticle_ = std::make_unique<Sprite>();
	sprite2DReticle_.reset(Sprite::Create(TextureManager::GetInstance()->Load("lockOn.png"), { 0.0f, 0.0f }));
	sprite2DReticle_->SetAnchorPoint(Vector2(0.5f, 0.5f));
	sprite2DReticle_->SetSize(Vector2(50.0f, 50.0f));


	worldTransform3DReticle_.translation_ = Add(Vector3(0.0f, 0.0f, 1.0f), Vector3(worldTransfrom_.matWorld_.m[3][0], worldTransfrom_.matWorld_.m[3][1], worldTransfrom_.matWorld_.m[3][2]));
	worldTransform3DReticle_.UpdateMatrix(RotationType::Euler);

	Vector3 positionReticle = {
		worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
		worldTransform3DReticle_.matWorld_.m[3][2] };
	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
		Multiply(Multiply(camera_->GetViewMatrix(), camera_->GetProjectionMatrix()), matViewport);
	

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = TransformVector3(positionReticle, matViewProjectionViewport);

	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	enemys_.clear();
		

	Create(&worldTransfrom_, Type::kAABB, RotationType::Euler, camera);
	Collider::SetMass(0.0f);

	// 衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	// 衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayer);

}

void Player::Update()
{
	// 自機の旋回処理
	Rotate();

	// キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0.0f };

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// ゲームパッドの状態を得る変数(XINPUT)
	XINPUT_STATE joyState;

	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;

	}

	// キャラクター攻撃処理
	Attack();

	for (std::list<PlayerBullet*>::iterator playerBulletIterator = bullets_.begin(); playerBulletIterator != bullets_.end();) {
		if ((*playerBulletIterator)->IsDead()) {
			playerBulletIterator = bullets_.erase(playerBulletIterator);
			continue;
		}
		(*playerBulletIterator)->Update();
		// 次のイテレーターへ
		++playerBulletIterator;
	}
	// 弾更新
	/*for (PlayerBullet* bullet : bullets_) {
		bullet->Update();

	}*/

	// デスフラグの立った弾を削除
	/*bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
		});*/

	// 座標移動(ベクトルの加算)
	worldTransfrom_.translation_ = Add(worldTransfrom_.translation_, move);

	// 移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 15.0f;

	// 範囲を超えない処理
	worldTransfrom_.translation_.x = max(worldTransfrom_.translation_.x, -kMoveLimitX);
	worldTransfrom_.translation_.x = min(worldTransfrom_.translation_.x, +kMoveLimitX);
	worldTransfrom_.translation_.y = max(worldTransfrom_.translation_.y, -kMoveLimitY);
	worldTransfrom_.translation_.y = min(worldTransfrom_.translation_.y, +kMoveLimitY);

	// ワールドトランスフォームの更新
	worldTransfrom_.UpdateMatrix(RotationType::Euler);

	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 30.0f;
	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0, 0, 1.0f };
	// 自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransfrom_.matWorld_);
	// ベクトルの長さを整える
	offset = Multiply(kDistancePlayerTo3DReticle, Normalize(offset));
	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ =
		Add(offset, Vector3{
						worldTransfrom_.matWorld_.m[3][0],
						worldTransfrom_.matWorld_.m[3][1],
						worldTransfrom_.matWorld_.m[3][2] });
	worldTransform3DReticle_.UpdateMatrix(RotationType::Euler);

	Vector3 positionReticle = {
		worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1],
		worldTransform3DReticle_.matWorld_.m[3][2]
	};
	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
		Multiply(Multiply(camera_->GetViewMatrix(), camera_->GetProjectionMatrix()), matViewport);

	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = TransformVector3(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	//sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	Reticle(matViewport);




	enemys_.clear();

	float x = worldTransfrom_.translation_.x;
	//camera_->SetTranslate(Vector3(x, camera_->GetTranslate().y, camera_->GetTranslate().z));
	if (Input::GetInstance()->PushKey(DIK_A)) {
		worldTransfrom_.translation_.x -= 0.1f;
		//SetVelocity(Vector3{ -0.1f, 0.0f, 0.0f });
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		worldTransfrom_.translation_.x += 0.1f;
		//SetVelocity(Vector3{ 0.1f, 0.0f, 0.0f });
	}
	if (Input::GetInstance()->TriggerKey(DIK_W)) {
		//worldTransfrom_.translation_.x += 0.1f;
		SetVelocity(Vector3{ 0.0f, 10.0f, 0.0f });
	}

	
}

void Player::Draw()
{
	model_->Draw(worldTransfrom_, TextureManager::GetInstance()->Load("Black1x1.png"));
	model_->Draw(worldTransform3DReticle_);
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();

	}
	//Collider::HitBox();
}

void Player::DrawBullet()
{
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw();

	}
}

void Player::Rotate()
{
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (Input::GetInstance()->PushKey(DIK_A)) {
		worldTransfrom_.rotation_.y -= kRotSpeed;

	}
	else if (Input::GetInstance()->PushKey(DIK_D)) {
		worldTransfrom_.rotation_.y += kRotSpeed;

	}
}

void Player::Attack()
{
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;

	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && attackTime_ == 0) {

		attackTime_ = 10;
		//自キャラの座標をコピー
		//DirectX::XMFLOAT3 position = worldTransform_.translation_;

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		Vector3 position = {
				worldTransfrom_.matWorld_.m[3][0], worldTransfrom_.matWorld_.m[3][1],
				worldTransfrom_.matWorld_.m[3][2] };

		if (enemys_.size() == 0) {
			// 弾を生成し、初期化
			PlayerBullet* newBullet = new PlayerBullet();
			// 自機から照準オブジェクトへのベクトル
			velocity = Subtract(
				Vector3{
					worldTransform3DReticle_.matWorld_.m[3][0],
					worldTransform3DReticle_.matWorld_.m[3][1],
					worldTransform3DReticle_.matWorld_.m[3][2] },
					Vector3{
						worldTransfrom_.matWorld_.m[3][0],
						worldTransfrom_.matWorld_.m[3][1],
						worldTransfrom_.matWorld_.m[3][2] });
			velocity = Multiply(kBulletSpeed, Normalize(velocity));

			newBullet->Initialize(camera_, position, /*Vector3(0.0f, 0.0f, 0.0f)*/ velocity);
			// 弾を登録する
			bullets_.push_back(newBullet);
		}
		else {
			for (auto& enemy : enemys_) {
				// 弾を生成し、初期化
				PlayerBullet* newBullet = new PlayerBullet();
				velocity = Subtract(Vector3(worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1], worldTransform3DReticle_.matWorld_.m[3][2]), enemy->GetWorldPosition());
				velocity = Multiply(kBulletSpeed, Normalize(velocity));
				newBullet->Initialize(camera_, position, velocity ,enemy);
				// 弾を登録する
				bullets_.push_back(newBullet);
			}


		}

	}
	else if (attackTime_ != 0) {
		attackTime_--;

	}
}

void Player::Reticle(const Matrix4x4& matViewport)
{
	//POINT mousePosition;
	//// マウス座標(スクリーン座標)を取得する
	//GetCursorPos(&mousePosition);

	//// クライアントエリア座標に変換する
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);

	// スプライトの現在座標を取得
	Vector2 spritePostion = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;
	// ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePostion.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePostion.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		// スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePostion);

	}

	//sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));

	// ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV =
		Multiply(Multiply(camera_->GetViewMatrix(), camera_->GetProjectionMatrix()), matViewport);
	// 合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	// スクリーン座標
	Vector3 posNear = Vector3(float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 0);
	Vector3 posFar = Vector3(float(sprite2DReticle_->GetPosition().x), float(sprite2DReticle_->GetPosition().y), 1);

	// スクリーン座標系からワールド座標系へ
	posNear = TransformVector3(posNear, matInverseVPV);
	posFar = TransformVector3(posFar, matInverseVPV);

	// マウスレイの方向
	Vector3 mouseDirection = Subtract(posFar, posNear);
	mouseDirection = Normalize(mouseDirection);
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	mouseDirection = Multiply(kDistanceTestObject, mouseDirection);
	/*worldTransform3DReticle_.translation_ =
		Add(posNear, Multiply(kDistanceTestObject, mouseDirection));*/
	worldTransform3DReticle_.translation_ = Add(posNear, mouseDirection);

	worldTransform3DReticle_.UpdateMatrix(RotationType::Euler);
}

void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}

void Player::SetParent(WorldTransform* parent)
{
	// 親子関係を結ぶ
	worldTransfrom_.parent_ = parent;
}
