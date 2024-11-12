#include "Tradition.h"

#include "TextureManager.h"


Tradition* Tradition::GetInstance()
{
    static Tradition instance;
    return &instance;
}

void Tradition::Initialize()
{
	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("white1x1.png"), { 640.0f, 360.0f }, { 1.0f, 1.0f, 1.0f, 0.0f }, { 0.5f, 0.5f }));
	sprite_->SetSize({ 1280.0f, 720.0 });
	in_ = false;
	out_ = false;
	time_ = 0.0f;
	second_ = 1.0f / 60.0f;
	isActive_ = false;
}

void Tradition::Update()
{
	if (isActive_) {
		if (in_ && time_ < 1.0f) {
			time_ += 1.0f / 60.0f;
			time_ = std::clamp(time_, 0.0f, 1.0f);
			sprite_->SetColor({ 1.0f, 1.0f, 1.0f, time_ });
			if (time_ == 1.0f) {
				in_ = false;
				out_ = true;
			}
		}
		if (out_ && time_ > 0.0f) {
			time_ -= 1.0f / 60.0f;
			time_ = std::clamp(time_, 0.0f, 1.0f);
			sprite_->SetColor({ 1.0f, 1.0f, 1.0f, time_ });
			if (time_ == 0.0f) {
				out_ = false;
				isActive_ = false;
			}
		}
	}
}

void Tradition::Draw()
{
	if (isActive_) {
		sprite_->Draw();
	}
}
