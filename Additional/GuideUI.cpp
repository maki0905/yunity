#include "GuideUI.h"

#include "TextureManager.h"
#include "GlobalVariables.h"

void GuideUI::Initialize()
{
	ApplyGlobalVariables();

	// Aボタン初期化
	guideATexture_[0] = yunity::TextureManager::GetInstance()->Load("AButton.png");
	guideATexture_[1] = yunity::TextureManager::GetInstance()->Load("AButtonPush.png");
	guideA_ = std::make_unique<yunity::Sprite>();
	guideA_.reset(yunity::Sprite::Create(guideATexture_[0], guideAPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	guideA_->SetSize(guideASize_);

	// RBボタン初期化
	guideRBTexture_[0] = yunity::TextureManager::GetInstance()->Load("RButton.png");
	guideRBTexture_[1] = yunity::TextureManager::GetInstance()->Load("RButtonPush.png");
	guideRB_ = std::make_unique<yunity::Sprite>();
	guideRB_.reset(yunity::Sprite::Create(guideRBTexture_[0], guideRBPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	guideRB_->SetSize(guideRBSize_);

	// Rスティック初期化
	guideRStickTexture_[0] = yunity::TextureManager::GetInstance()->Load("RStickNeutral.png");
	guideRStickTexture_[1] = yunity::TextureManager::GetInstance()->Load("RStickLeft.png");
	guideRStickTexture_[2] = yunity::TextureManager::GetInstance()->Load("RStickRight.png");
	guideRStick_ = std::make_unique<yunity::Sprite>();
	guideRStick_.reset(yunity::Sprite::Create(guideRStickTexture_[0], guideRStickPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	guideRStick_->SetSize(guideRStickSize_);

	// ジャンプテクスチャ初期化
	guideJump_ = std::make_unique<yunity::Sprite>();
	guideJump_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("jump.png"), guideJumpPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	guideJump_->SetSize(guideJumpSize_);

	// 発射/解除テクスチャ初期化
	guideWireTexture_[0] = yunity::TextureManager::GetInstance()->Load("shot.png");
	guideWireTexture_[1] = yunity::TextureManager::GetInstance()->Load("release.png");
	guideWire_ = std::make_unique<yunity::Sprite>();
	guideWire_.reset(yunity::Sprite::Create(guideWireTexture_[0], guideWirePosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	guideWire_->SetSize(guideWireSize_);

	// エイムテクスチャ初期化
	guideAim_ = std::make_unique<yunity::Sprite>();
	guideAim_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("aim.png"), guideAimPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, yunity::Sprite::halfAnchorPoint_));
	guideAim_->SetSize(guideAimSize_);
}

void GuideUI::Draw()
{
	ApplyGlobalVariables();


	// Aボタン描画
	guideA_->SetPosition(guideAPosition_);
	guideA_->Draw();

	// RBボタン描画
	guideRB_->SetPosition(guideRBPosition_);
	guideRB_->Draw();

	// Rスティック描画
	guideRStick_->SetPosition(guideRStickPosition_);
	guideRStick_->Draw();

	// ジャンプ描画
	guideJump_->SetPosition(guideJumpPosition_);
	guideJump_->Draw();

	// 発射/解除描画
	guideWire_->SetPosition(guideWirePosition_);
	guideWire_->Draw();

	// エイム描画
	guideAim_->SetPosition(guideAimPosition_);
	guideAim_->Draw();
}

void GuideUI::SetButtonState(GuideType type, ButtonState state)
{
	switch (type)
	{
	case GuideUI::GuideType::ABotton:
		guideA_->SetTextureHandle(guideATexture_[static_cast<int>(state)]);
		break;
	case GuideUI::GuideType::RBBotton:
		guideRB_->SetTextureHandle(guideRBTexture_[static_cast<int>(state)]);
		break;
	case GuideUI::GuideType::Jump:
		break;
	case GuideUI::GuideType::Wire:
		guideWire_->SetTextureHandle(guideWireTexture_[static_cast<int>(state)]);
		break;
	default:
		break;
	}
}

void GuideUI::SetStickState(RStickState state)
{
	guideRStick_->SetTextureHandle(guideRStickTexture_[static_cast<int>(state)]);
}


void GuideUI::ApplyGlobalVariables()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "GuideUI";

	guideAPosition_ = globalVariables->GetVector2Value(groupName, "GuideAPosition");
	guideASize_ = globalVariables->GetVector2Value(groupName, "GuideASize");

	guideRBPosition_ = globalVariables->GetVector2Value(groupName, "GuideRBPosition");
	guideRBSize_ = globalVariables->GetVector2Value(groupName, "GuideRBSize");

	guideRStickPosition_ = globalVariables->GetVector2Value(groupName, "GuideRStickPosition");
	guideRStickSize_ = globalVariables->GetVector2Value(groupName, "GuideRStickSize");

	guideJumpPosition_ = globalVariables->GetVector2Value(groupName, "GuideJumpPosition");
	guideJumpSize_ = globalVariables->GetVector2Value(groupName, "GuideJumpSize");

	guideWirePosition_ = globalVariables->GetVector2Value(groupName, "GuideWirePosition");
	guideWireSize_ = globalVariables->GetVector2Value(groupName, "GuideWireSize");

	guideAimPosition_ = globalVariables->GetVector2Value(groupName, "GuideAimPosition");
	guideAimSize_ = globalVariables->GetVector2Value(groupName, "GuideAimSize");
}
