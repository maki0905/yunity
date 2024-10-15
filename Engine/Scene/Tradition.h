#pragma once

#include "Sprite.h"

enum TraditionMode {
	kNoneTradition,
	kFadein,
	kFadeout,
	kMax
};

class Tradition {
public:
	static Tradition* GetInstance();
	void Initialize();
	void Update();
	void Draw();
	void Start() { in_ = true; isActive_ = true; }
	bool End() { return isActive_; }
	void SetSecond(float second) { second_ = second; }
	bool GetIsActive() { return isActive_; }
	bool GetIn() { return in_; }
	bool GetOut() { return out_; }

private:
	std::unique_ptr<Sprite> sprite_;

	TraditionMode mode_;
	bool isActive_;
	bool in_;
	bool out_;
	float time_;
	float second_;
};