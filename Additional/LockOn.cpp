#include "LockOn.h"
#include "TextureManager.h"
#include "Enemy.h"
#include "WindowsAPI.h"

void LockOn::Initalize()
{
	lockOnMark_ = std::make_unique<Sprite>();

	textureHandle_ = TextureManager::Load("lockOn.png");
	lockOnMark_.reset(Sprite::Create(textureHandle_, { 0.0f, 0.0f }));
	lockOnMark_->SetAnchorPoint(Vector2{ 0.5f, 0.5f });
	lockOnMark_->SetSize(Vector2{ 20.0f, 20.0f });
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	Input::GetInstance()->GetJoystickState(0, joyState_);

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
		if (!(joyStatePre_.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)) {
			if (mode_ == LockOnMode::kAuto) {
				mode_ = LockOnMode::kManual;
			}
			else {
				mode_ = LockOnMode::kAuto;
			}
		}
	}

	// ロックオン状態なら
	if (target_) {
		if (mode_ == LockOnMode::kManual) {
			if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
				if (!(joyStatePre_.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
					ReSearch(enemies, camera);
				}
			}
		}

		// ロックオンボタンをトリガーしたら
		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			if (!(joyStatePre_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
				// ロックオンを外す
				target_ = nullptr;
			}
		}
		else if (IsOutOfRange(camera)) {
			// ロックオンを外す
			target_ = nullptr;
		}
		else if (target_->GetIsDestroy()) {
			target_ = nullptr;
		}
	}
	else if (mode_ == LockOnMode::kAuto) {
		// ロックオン対象の検索
		Search(enemies, camera);
	}
	else if (mode_ == LockOnMode::kManual) {
		if (!(joyStatePre_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				// ロックオン対象の検索
				Search(enemies, camera);

			}
		}


	}

	if (target_) {
		// 敵のロックオン座標取得
		Vector3 positionWorld = target_->GetModelCenter();
		// ワールド座標からスクリーン座標に変換
		Vector3 positionScreen = MapWorldToScreen(positionWorld, camera.matView, camera.matProjection, WindowsAPI::kWindowWidth, WindowsAPI::kWindowHeight);
		// Vector2に格納
		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);
		// スプライトの座標を設定
		lockOnMark_->SetPosition(positionScreenV2);
	}
	joyStatePre_ = joyState_;
}

void LockOn::Draw()
{
	if (target_) {
		lockOnMark_->Draw();
	}
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// 全ての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		if (enemy->GetIsActive() && !enemy->GetIsDestroy()) {
			// 敵のロックオン座標取得
			Vector3 positionWorld = enemy->GetModelCenter();

			// ワールド->ビュー座標変換
			Vector3 positionView = TransformVector3(positionWorld, camera.matView);

			// 距離条件チェック
			if (minDistace_ <= positionView.z && positionView.z <= maxDistance_) {
				// カメラ前方との角度を計算
				/*positionView = Normalize(positionView);
				float dot = Dot({ 0.0f, 0.0f, 1.0f }, positionView);*/
				float arcTangent = std::atan2f(std::sqrtf(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);
				//arcTangent = std::acosf(dot);
				// 角度条件チェック(コーンに収まっているか)
				if (std::abs(arcTangent) <= angleRange_) {
					targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
				}

			}
		}
	}

	// ロックオン対象をリセット
	target_ = nullptr;
	if (targets.size()) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象とする
		target_ = targets.front().second;

	}

}

void LockOn::ReSearch(const std::list<std::unique_ptr<Enemy>>& enemies, const Camera& camera)
{
	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// 全ての敵に対して順にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {
		if (enemy->GetIsActive() && !enemy->GetIsDestroy()) {
			// 敵のロックオン座標取得
			Vector3 positionWorld = enemy->GetModelCenter();

			// ワールド->ビュー座標変換
			Vector3 positionView = TransformVector3(positionWorld, camera.matView);

			// 距離条件チェック
			if (minDistace_ <= positionView.z && positionView.z <= maxDistance_) {
				// カメラ前方との角度を計算
				/*positionView = Normalize(positionView);
				float dot = Dot({ 0.0f, 0.0f, 1.0f }, positionView);
				float arcTangent = std::acosf(dot);*/
				float arcTangent = std::atan2f(std::sqrtf(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

				// 角度条件チェック(コーンに収まっているか)
				if (std::abs(arcTangent) <= angleRange_) {
					targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
				}

			}
		}
	}

	if (targets.size()) {
		// 距離で昇順にソート
		targets.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });

		while (1) {
			if (targets.front().second != target_) {
				target_ = nullptr;
				target_ = targets.front().second;
				break;
			}
			else {
				targets.pop_front();
				if (!targets.size()) {
					break;
				}
			}
		}

	}
}

bool LockOn::IsOutOfRange(const Camera& camera)
{
	// 敵のロックオン座標取得
	Vector3 positionWorld = target_->GetModelCenter();

	// ワールド->ビュー座標変換
	Vector3 positionView = TransformVector3(positionWorld, camera.matView);

	// 距離条件チェック
	if (minDistace_ <= positionView.z && positionView.z <= maxDistance_) {
		// カメラ前方との角度を計算
		float arcTangent = std::atan2f(std::sqrtf(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);
		// 角度条件チェック(コーンに収まっているか)
		if (std::abs(arcTangent) <= angleRange_) {
			return false;
		}

	}
	// 範囲外である
	return true;

}

Vector3 LockOn::GetTargetPosition() const
{
	if (target_) {
		return target_->GetModelCenter();
	}
	return Vector3();
}



