#include "Score.h"

#include "TextureManager.h"
#include "MathFunction.h"
#include "EngineTimeStep.h"
#include "GlobalVariables.h"

void Score::Initialize()
{
	// jsonデータ読み込み
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "Score";
	scorePosition_ = globalVariables->GetVector2Value(groupName, "BaseScorePosition");
	for (int i = 0; i < 2; i++) {
		period_[i] = globalVariables->GetFloatValue(groupName, "Period" + std::to_string(i));
	}
	scoreBackSize_ = globalVariables->GetVector2Value(groupName, "ScoreBackSize");
	hiScorePosition_ = globalVariables->GetVector2Value(groupName, "HiScorePosition");
	minHiScoreSize_ = globalVariables->GetVector2Value(groupName, "MinHiScoreSize");
	maxHiScoreSize_ = globalVariables->GetVector2Value(groupName, "MaxHiScoreSize");
	scoreNumberSize_ = globalVariables->GetVector2Value(groupName, "ScoreNumberSize");
	scoreNumberTexBase_ = globalVariables->GetVector2Value(groupName, "ScoreNumberTexBase");
	scoreNumberTextureSize_ = globalVariables->GetFloatValue(groupName, "ScoreNumberTextureSize");
	scoreNumberWidth_ = globalVariables->GetFloatValue(groupName, "ScoreNumberWidth");

	// スコア背景
	scoreBackground_ = std::make_unique<yunity::Sprite>();
	scoreBackground_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("ScoreBack.png"), scorePosition_));
	scoreBackground_->SetAnchorPoint(yunity::Sprite::halfAnchorPoint_);
	scoreBackground_->SetSize(scoreBackSize_);

	// ハイスコア
	hiScore_ = std::make_unique<yunity::Sprite>();
	hiScore_.reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("hiscore.png"), { 0.0f, 0.0f }));
	hiScore_->SetAnchorPoint(yunity::Sprite::halfAnchorPoint_);
	hiScore_->SetSize(minHiScoreSize_);
	isHiScore_ = false;

	// スコア数
	for (int index = 0; index < maxDigit; index++) {
		scoreNumber_[index] = std::make_unique<yunity::Sprite>();
		scoreNumber_[index].reset(yunity::Sprite::Create(yunity::TextureManager::GetInstance()->Load("numberWhite.png"), scorePosition_));
		scoreNumber_[index]->SetAnchorPoint(yunity::Sprite::halfAnchorPoint_);
		scoreNumber_[index]->SetSize(scoreNumberSize_);
		scoreNumber_[index]->SetTextureRect(scoreNumberTexBase_, { scoreNumberTextureSize_, scoreNumberTextureSize_ });
	}

	score_ = 0;

	digit_ = 1;

	lerpTime_ = 0.0f;
}

void Score::Update()
{
	// 現在スコアに加算していく
	if (addScore_ != 0) {
		score_ += 1;
		addScore_ -= 1;
	}

	// 桁数に合わせたスコア表示のための処理
	for (int index = maxDigit - 1; index >= 0; index--) {
		uint32_t num = score_ / (uint32_t)std::pow(10, maxDigit - index - 1);
		num %= 10;

		if (num != 0) {
			digit_ = maxDigit - index;
			if (digit_ < 1) {
				digit_ = 1;
			}
		}
		scoreNumber_[index]->SetTextureRect({ 0.0f, scoreNumberTextureSize_ * (float)num }, { scoreNumberTextureSize_, scoreNumberTextureSize_ });
	}

	// ハイスコア表示時のイージング処理
	if (isHiScore_) {
		lerpTime_ += yunity::fixedTimeStep_;
		if (lerpTime_ < period_[0]) {
			hiScore_->SetSize(Lerp(minHiScoreSize_, maxHiScoreSize_, (float)lerpTime_/ period_[0]));
		}
		else if (lerpTime_ < period_[1]) {
			hiScore_->SetSize(Lerp(maxHiScoreSize_, minHiScoreSize_, (float(lerpTime_/ period_[0]) - 1.0f) / period_[1]));
		}
		else {
			lerpTime_ = 0.0f;
		}
	}

#ifdef _DEBUG
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "Score";
	for (int i = 0; i < 2; i++) {
		period_[i] = globalVariables->GetFloatValue(groupName, "Period" + std::to_string(i));
	}
	scoreBackSize_ = globalVariables->GetVector2Value(groupName, "ScoreBackSize");
	hiScorePosition_ = globalVariables->GetVector2Value(groupName, "HiScorePosition");
	minHiScoreSize_ = globalVariables->GetVector2Value(groupName, "MinHiScoreSize");
	maxHiScoreSize_ = globalVariables->GetVector2Value(groupName, "MaxHiScoreSize");
	scoreNumberSize_ = globalVariables->GetVector2Value(groupName, "ScoreNumberSize");
	scoreNumberTexBase_ = globalVariables->GetVector2Value(groupName, "ScoreNumberTexBase");
	scoreNumberTextureSize_ = globalVariables->GetFloatValue(groupName, "ScoreNumberTextureSize");
	scoreNumberWidth_ = globalVariables->GetFloatValue(groupName, "ScoreNumberWidth");
#endif // _DEBUG


}

void Score::Draw()
{
	scoreBackground_->SetPosition(scorePosition_);
	scoreBackground_->Draw();

	// スコアの桁ごとの表示
	switch (digit_)
	{
	case 1:
		scoreNumber_[3]->SetPosition(scorePosition_);
		scoreNumber_[3]->Draw();
		break;
	case 2:
		scoreNumber_[3]->SetPosition({ scorePosition_.x + scoreNumberWidth_, scorePosition_.y});
		scoreNumber_[3]->Draw();
		scoreNumber_[2]->SetPosition({ scorePosition_.x - scoreNumberWidth_, scorePosition_.y });
		scoreNumber_[2]->Draw();
		break;
	case 3 :
		scoreNumber_[3]->SetPosition({ scorePosition_.x + scoreNumberWidth_ * float(digit_- 1), scorePosition_.y });
		scoreNumber_[3]->Draw();
		scoreNumber_[2]->SetPosition(scorePosition_);
		scoreNumber_[2]->Draw();
		scoreNumber_[1]->SetPosition({ scorePosition_.x - scoreNumberWidth_ * float(digit_ - 1), scorePosition_.y });
		scoreNumber_[1]->Draw();

		break;
	case 4 :
		scoreNumber_[3]->SetPosition({ scorePosition_.x + scoreNumberWidth_ * float(digit_ - 1), scorePosition_.y });
		scoreNumber_[3]->Draw();
		scoreNumber_[2]->SetPosition({ scorePosition_.x + scoreNumberWidth_, scorePosition_.y });
		scoreNumber_[2]->Draw();
		scoreNumber_[1]->SetPosition({ scorePosition_.x - scoreNumberWidth_, scorePosition_.y });
		scoreNumber_[1]->Draw();
		scoreNumber_[0]->SetPosition({ scorePosition_.x - scoreNumberWidth_ * float(digit_ - 1), scorePosition_.y });
		scoreNumber_[0]->Draw();
		break;
	}

	if (isHiScore_) {
		hiScore_->SetPosition(Add(scorePosition_, hiScorePosition_));
		hiScore_->Draw();
	}
	
}

void Score::Reset()
{
	score_ = 0;
	digit_ = 1;
}
