#include "Score.h"

#include "TextureManager.h"
#include "MathFunction.h"

void Score::Initialize()
{
	scoreBackground_ = std::make_unique<Sprite>();
	scoreBackground_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ScoreBack.png"), { 84.0f, 84.0f }));
	scoreBackground_->SetAnchorPoint({ 0.5f, 0.5f });
	scoreBackground_->SetSize({ 120.0f, 120.0f });
	//scoreBackground_
	for (uint32_t index = 0; index < 4; index++) {
		scoreNumber_[index] = std::make_unique<Sprite>();
		scoreNumber_[index].reset(Sprite::Create(TextureManager::GetInstance()->Load("numberWhite.png"), { 84.0f, 84.0f }));
		scoreNumber_[index]->SetAnchorPoint({ 0.5f, 0.5f });
		scoreNumber_[index]->SetSize({ 44.0f, 44.0f });
		scoreNumber_[index]->SetTextureRect({ 0.0f,384.0f }, { 64.0f, 64.0f });
	}
	score_ = 10;
	digit_ = 1;
}

void Score::Update()
{
	if (addScore_ != 0) {
		score_ += 1;
		addScore_ -= 1;
	}

	for (uint32_t index = 3; index < -1; index--) {
		uint32_t num = score_ / std::pow(10, 4 - index - 1);
		num %= 10;

		if (num != 0) {
			digit_ = 4 - index;
			if (digit_ < 1) {
				digit_ = 1;
			}
		}
		scoreNumber_[/*3 -*/ index]->SetTextureRect({ 0.0f, 64.0f * (float)num }, { 64.0f, 64.0f });
	}

}

void Score::Draw()
{
	scoreBackground_->Draw();

	/*for (uint32_t index = 0; index < 1; index++) {
		scoreNumber_[index]->Draw();
	}*/

	switch (digit_)
	{
	case 1:
		scoreNumber_[3]->SetPosition(pos_);
		scoreNumber_[3]->Draw();
		break;
	case 2:
		scoreNumber_[3]->SetPosition({pos_.x + 16.0f, pos_.y});
		scoreNumber_[3]->Draw();
		scoreNumber_[2]->SetPosition({ pos_.x - 16.0f, pos_.y });
		scoreNumber_[2]->Draw();
		break;
	case 3 :
		scoreNumber_[3]->SetPosition({ pos_.x + 32.0f, pos_.y });
		scoreNumber_[3]->Draw();
		scoreNumber_[2]->SetPosition(pos_);
		scoreNumber_[2]->Draw();
		scoreNumber_[1]->SetPosition({ pos_.x - 32.0f, pos_.y });
		scoreNumber_[1]->Draw();

		break;
	case 4 :
		scoreNumber_[3]->SetPosition({ pos_.x + 48.0f, pos_.y });
		scoreNumber_[3]->Draw();
		scoreNumber_[2]->SetPosition({ pos_.x + 16.0f, pos_.y });
		scoreNumber_[2]->Draw();
		scoreNumber_[1]->SetPosition({ pos_.x - 16.0f, pos_.y });
		scoreNumber_[1]->Draw();
		scoreNumber_[0]->SetPosition({ pos_.x - 48.0f, pos_.y });
		scoreNumber_[0]->Draw();
		break;
	}
	
}

void Score::Reset()
{
	score_ = 0;
	digit_ = 1;
}
