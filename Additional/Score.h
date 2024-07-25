#pragma once
#include <memory>

#include "Sprite.h"

class Score {
public:
	void Initialize();
	void Update();
	void Draw();

	void AddScore(uint32_t addScore) { addScore_ += addScore; }
	void Reset();

private:
	std::unique_ptr<Sprite> scoreBackground_;
	std::unique_ptr<Sprite> scoreNumber_[4];
	uint32_t score_;
	uint32_t digit_;
	Vector2 pos_ = { 84.0f, 84.0f };
	uint32_t addScore_;
};