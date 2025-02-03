#include "Pose.h"

#include "GlobalVariables.h"
#include "TextureManager.h"
#include "WindowsAPI.h"
#include "EngineTimeStep.h"
#include "SceneManager.h"
#include "Player.h"
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
	poseBotton_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("poseBotton.png"), poseBottonPosition, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.5f, 0.5f}));
	poseBotton_->SetSize(poseBottonSize);
	// ポーズフォント
	pose_ = std::make_unique<yunity::Sprite>();
	pose_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("pose.png"), posePosition, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	pose_->SetSize(poseSize);
	//ポーズ画面
	poseBase_ = std::make_unique<yunity::Sprite>();
	poseBase_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("poseBase.png"), poseBasePosition, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	
	// ポーズの背景
	poseBack_ = std::make_unique<yunity::Sprite>();
	poseBack_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("Black1x1.png"), {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, poseBackTransparency}));
	poseBack_->SetSize({ yunity::WindowsAPI::kWindowWidth, yunity::WindowsAPI::kWindowHeight });
	// タイトルフォント
	title_ = std::make_unique<yunity::Sprite>();
	title_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("title.png"), titlePosition, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	title_->SetSize(titleSize);
	// つづきフォント
	continuation_ = std::make_unique<yunity::Sprite>();
	continuation_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("continuation.png"), continuationPosition, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	continuation_->SetSize(continuationSize);
	// リスタートフォント
	restart_ = std::make_unique<yunity::Sprite>();
	restart_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("restart.png"), restartPosition, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	restart_->SetSize(restartSize);
	// セレクト
	select_ = std::make_unique<yunity::Sprite>();
	select_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("yellow1x1.png"), {0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	select_->SetSize(selectSize);
	select_->SetPosition(continuation_->GetPosition());

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

	if (yunity::Input::GetInstance()->IsControllerConnected()) {
		if (yunity::Input::GetInstance()->GetJoystickState(0, pad_)) {
			if (isPose_) {
				float move = 0.0f;
				if (time_ == 0.0f) {
					move = (float)pad_.Gamepad.sThumbLY;
					if (move != 0.0f) {
						move = move / std::abs(move);
						time_ = 0.2f;
					}
				}

				if (select_->GetPosition().y == continuation_->GetPosition().y) {
					if (move > 0.0f) {
						select_->SetPosition(title_->GetPosition());
					}
					else if (move < 0.0f) {
						select_->SetPosition(restart_->GetPosition());
					}

					if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
						isPose_ = false;
					}
				}
				else if (select_->GetPosition().y == restart_->GetPosition().y) {
					if (move > 0.0f) {
						select_->SetPosition(continuation_->GetPosition());
					}
					else if (move < 0.0f) {
						select_->SetPosition(title_->GetPosition());
					}

					if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
						player_->InitializeDeth();
						player_->ChangeProductionState(std::make_unique<ResetProduction>());
						isPose_ = false;
						return true;
					}
				}
				else if (select_->GetPosition().y == title_->GetPosition().y) {
					if (move > 0.0f) {
						select_->SetPosition(restart_->GetPosition());
					}
					else if (move < 0.0f) {
						select_->SetPosition(continuation_->GetPosition());
					}

					if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
						yunity::SceneManager::GetInstance()->ChangeScene("TITLE");
						return true;
					}
				}
			}

			if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_START) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_START)) {
				isPose_ ^= true;
			}


		}
	}

	if (time_ > 0.0f) {
		time_ -= yunity::fixedTimeStep_;
		time_ = std::clamp(time_, 0.0f, 1.0f);
	}


	return isPose_;
}

void Pose::Draw()
{
	poseBotton_->Draw();
	pose_->Draw();

	if (isPose_) {
		poseBack_->Draw();
		poseBase_->Draw();
		select_->Draw();
		title_->Draw();
		continuation_->Draw();
		restart_->Draw();
	}

}
