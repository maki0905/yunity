#include "Pose.h"

#include "GlobalVariables.h"
#include "TextureManager.h"
#include "WindowsAPI.h"
#include "EngineTimeStep.h"
#include "SceneManager.h"
#include "Player/Player.h"
#include "CommonData.h"

void Pose::Initialize(Player* player)
{
    
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "Pose";

	float poseBackTransparency = globalVariables->GetFloatValue(groupName, "PoseBackTransparency");
	Vector2 poseBottonPosition = globalVariables->GetVector2Value(groupName, "PoseBottonPosition");
	Vector2 poseBottonSize = globalVariables->GetVector2Value(groupName, "PoseBottonSize");
	Vector2 posePosition = globalVariables->GetVector2Value(groupName, "PosePosition");
	Vector2 poseSize = globalVariables->GetVector2Value(groupName, "PoseSize");
	Vector2 poseBasePosition = globalVariables->GetVector2Value(groupName, "PoseBasePosition");
	Vector2 poseBaseSize = globalVariables->GetVector2Value(groupName, "PoseBaseSize");
	Vector2 poseBackSize = globalVariables->GetVector2Value(groupName, "PoseBackSize");
	Vector2 titlePosition = globalVariables->GetVector2Value(groupName, "TitlePosition");
	Vector2 titleSize = globalVariables->GetVector2Value(groupName, "TitleSize");
	Vector2 continuationPosition = globalVariables->GetVector2Value(groupName, "ContinuationPosition");
	Vector2 continuationSize = globalVariables->GetVector2Value(groupName, "ContinuationSize");
	Vector2 restartPosition = globalVariables->GetVector2Value(groupName, "RestartPosition");
	Vector2 restartSize = globalVariables->GetVector2Value(groupName, "RestartSize");
	Vector2 selectSize = globalVariables->GetVector2Value(groupName, "SelectSize");

	// ポーズボタン
	poseBotton_ = std::make_unique<yunity::Sprite>();
	poseBotton_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("poseBotton.png"), poseBottonPosition, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	poseBotton_->SetSize(poseBottonSize);
	// ポーズフォント
	pose_ = std::make_unique<yunity::Sprite>();
	pose_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("pose.png"), posePosition, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	pose_->SetSize(poseSize);
	//ポーズ画面
	poseBase_ = std::make_unique<yunity::Sprite>();
	poseBase_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("poseBase.png"), poseBasePosition, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	
	// ポーズの背景
	poseBack_ = std::make_unique<yunity::Sprite>();
	poseBack_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("Black1x1.png"), {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, poseBackTransparency}));
	poseBack_->SetSize({ yunity::WindowsAPI::kWindowWidth, yunity::WindowsAPI::kWindowHeight });
	// タイトルフォント
	title_ = std::make_unique<yunity::Sprite>();
	title_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("title.png"), titlePosition, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	title_->SetSize(titleSize);
	// つづきフォント
	continuation_ = std::make_unique<yunity::Sprite>();
	continuation_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("continuation.png"), continuationPosition, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	continuation_->SetSize(continuationSize);
	// リスタートフォント
	restart_ = std::make_unique<yunity::Sprite>();
	restart_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("restart.png"), restartPosition, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	restart_->SetSize(restartSize);
	// セレクター
	selecter_ = std::make_unique<yunity::Sprite>();
	selecter_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("yellow1x1.png"), {0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	selecter_->SetSize(selectSize);
	selecter_->SetPosition(continuation_->GetPosition());

	isPose_ = false;
	time_ = 0.0f;

	player_ = player;
}

bool Pose::Update()
{
	prePad_ = pad_;

	if (CommonData::GetInstance()->flagState_ != FlagState::kPlay) {
		return false;
	}

	continuation_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	restart_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	title_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	if (time_ > 0.0f) {
		time_ -= yunity::fixedTimeStep_;
		time_ = std::clamp(time_, 0.0f, 1.0f);
	}


	if (yunity::Input::GetInstance()->IsControllerConnected()) {
		if (yunity::Input::GetInstance()->GetJoystickState(0, pad_)) {
			if (isPose_) {
				float move = 0.0f;
				if (time_ == 0.0f) { // 制限
					move = (float)pad_.Gamepad.sThumbLY;
					if (move != 0.0f) {
						move = move / std::abs(move);
						time_ = 0.2f;
					}
				}

				if (selecter_->GetPosition().y == continuation_->GetPosition().y) { // つづきを選択中
					if (move > 0.0f) { // 上へ移動
						selecter_->SetPosition(title_->GetPosition());
					}
					else if (move < 0.0f) { // 下へ移動
						selecter_->SetPosition(restart_->GetPosition());
					}

					continuation_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

					if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) { // 決定
						isPose_ = false;
						time_ = 0.1f;
						return true;
					}
				}
				else if (selecter_->GetPosition().y == restart_->GetPosition().y) { // リスタートを選択中
					if (move > 0.0f) { // 上へ移動
						selecter_->SetPosition(continuation_->GetPosition());
					}
					else if (move < 0.0f) { // 下へ移動
						selecter_->SetPosition(title_->GetPosition());
					}

					restart_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

					if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) { // 決定
						player_->InitializeDeth();
						player_->ChangeProductionState(std::make_unique<ResetProduction>());
						isPose_ = false;
						selecter_->SetPosition(continuation_->GetPosition());
						return true;
					}
				}
				else if (selecter_->GetPosition().y == title_->GetPosition().y) { // タイトルを選択中
					if (move > 0.0f) { // 上へ移動
						selecter_->SetPosition(restart_->GetPosition());
					}
					else if (move < 0.0f) { // 下へ移動
						selecter_->SetPosition(continuation_->GetPosition());
					}

					title_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

					if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) { // 決定
						yunity::SceneManager::GetInstance()->ChangeScene("TITLE");
						return true;
					}
				}
			}
			else {
				if (time_ != 0.0f) {
					return true;
				}
			}

			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_START) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_START)) {
				isPose_ ^= true;
			}


		}
	}

	return isPose_;
}

void Pose::Draw()
{
	// ポーズボタン
	poseBotton_->Draw();
	// ポーズフォント
	pose_->Draw();

	if (isPose_) {
		// ポーズ背景
		poseBack_->Draw();
		// ポーズ画面
		poseBase_->Draw();
		// セレクター
		selecter_->Draw();
		// タイトルフォント
		title_->Draw();
		// つづきフォント
		continuation_->Draw();
		// リスタートフォント
		restart_->Draw();
	}

}
