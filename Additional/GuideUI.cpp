#include "GuideUI.h"

#include "TextureManager.h"
#include "GlobalVariables.h"

void GuideUI::Initialize()
{
	ApplyGlobalVariables();

	guideATexture_[0] = yunity::TextureManager::GetInstance()->Load("ABotton.png");
	guideATexture_[1] = yunity::TextureManager::GetInstance()->Load("ABottonPush.png");
	guideA_ = std::make_unique<yunity::Sprite>();
	guideA_.reset(yunity::Sprite::Create(guideATexture_[0], guideAPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	guideA_->SetSize(guideASize_);

	guideRBTexture_[0] = yunity::TextureManager::GetInstance()->Load("RBotton.png");
	guideRBTexture_[1] = yunity::TextureManager::GetInstance()->Load("RBottonPush.png");
	guideRB_ = std::make_unique<yunity::Sprite>();
	guideRB_.reset(yunity::Sprite::Create(guideRBTexture_[0], guideRBPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	guideRB_->SetSize(guideRBSize_);

	guideJump_ = std::make_unique<yunity::Sprite>();
	guideJump_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("jump.png"), guideJumpPosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	guideJump_->SetSize(guideJumpSize_);

	guideWireTexture_[0] = yunity::TextureManager::GetInstance()->Load("shot.png");
	guideWireTexture_[1] = yunity::TextureManager::GetInstance()->Load("release.png");
	guideWire_ = std::make_unique<yunity::Sprite>();
	guideWire_.reset(yunity::Sprite::Create(guideWireTexture_[0], guideWirePosition_, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
	guideWire_->SetSize(guideWireSize_);
}

void GuideUI::Draw()
{
	guideA_->Draw();
	guideRB_->Draw();
	guideJump_->Draw();
	guideWire_->Draw();
}

void GuideUI::SetTexrture(GuideType type, int num)
{
	switch (type)
	{
	case GuideUI::GuideType::ABotton:
		guideA_->SetTextureHandle(guideATexture_[num]);
		break;
	case GuideUI::GuideType::RBBotton:
		guideRB_->SetTextureHandle(guideRBTexture_[num]);
		break;
	case GuideUI::GuideType::Jump:
		break;
	case GuideUI::GuideType::Wire:
		guideWire_->SetTextureHandle(guideWireTexture_[num]);
		break;
	default:
		break;
	}

}

void GuideUI::ApplyGlobalVariables()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "GuideUI";

	guideAPosition_ = globalVariables->GetVector2Value(groupName, "GuideAPosition");
	guideASize_ = globalVariables->GetVector2Value(groupName, "GuideASize");
	guideRBPosition_ = globalVariables->GetVector2Value(groupName, "GuideRBPosition");
	guideRBSize_ = globalVariables->GetVector2Value(groupName, "GuideRBSize");
	guideJumpPosition_ = globalVariables->GetVector2Value(groupName, "GuideJumpPosition");
	guideJumpSize_ = globalVariables->GetVector2Value(groupName, "GuideJumpSize");
	guideWirePosition_ = globalVariables->GetVector2Value(groupName, "GuideWirePosition");
	guideWireSize_ = globalVariables->GetVector2Value(groupName, "GuideWireSize");
}
