#include "Screen.h"

#include "TextureManager.h"
#include "MathFunction.h"

Screen* Screen::GetInstance()
{
	static Screen instance;
	return &instance;
}

void Screen::Initialize()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("Black.png"), { 0.0f, 0.0f }));
	sprite_->SetSize({ 1280.0f, 720.0f });
	color_ = { 1.0f, 1.0f, 1.0f, 0.0f };
	sprite_->SetColor(color_);
	t_ = 0.0f;
	duration_ = 0.0f;
	waitingTime_ = 0.0f;
	isOver_ = true;
	flap_ = false;
	type_ = Type::None;

}

void Screen::Update()
{
	if (type_ != Type::None) {
		t_ += 1.0f / (60.0f * duration_);
		t_ = std::clamp(t_, 0.0f, 1.0f);
	}

	switch (type_)
	{
	case Screen::Type::None:
		t_ = 0.0f;
		break;
	case Screen::Type::Fade_Out_In:

		if (flap_) {
			if (FadeIn()) {
				type_ = Type::None;
				isOver_ = true;
			}
		}
		else {
			flap_ = FadeOut();
		}
		break;
	case Screen::Type::Fade_In_Out:
		if (flap_) {
			if (FadeOut()) {
				type_ = Type::None;
				isOver_ = true;
			}
		}
		else {
			flap_ = FadeIn();
		}
		break;
	default:
		break;
	}

	sprite_->SetColor(color_);
}

void Screen::Draw()
{
	sprite_->Draw();
}

void Screen::Playback(Type type, float duration, float waitingTime)
{
	type_ = type;
	isOver_ = false;
	flap_ = false;
	duration_ = duration;
	waitingTime_ = waitingTime;

}

bool Screen::FadeOut()
{
	if (t_ < 1.0f) {
		color_.w = Lerp(0.0f, 1.0f, t_);
		return false;
	}
	else if (waitingTime_ > 0.0f) {
		waitingTime_ -= 1.0f / 60.0f;
		return false;
	}
	t_ = 0.0f;
	return true;

}

bool Screen::FadeIn()
{
	if (t_ < 1.0f) {
		color_.w = Lerp(1.0f, 0.0f, t_);
		return false;
	}
	else if (waitingTime_ > 0.0f) {
		waitingTime_ -= 1.0f / 60.0f;
		return false;
	}
	t_ = 0.0f;
	return true;
}
