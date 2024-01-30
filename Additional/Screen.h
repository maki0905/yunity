#pragma once

#include <memory>

#include "Sprite.h"
#include "Vector4.h"


class Screen {

public:

	enum class Type
	{
		None,
		Fade_Out_In,
		Fade_In_Out
	};

public:
	static Screen* GetInstance();
	void Initialize();
	void Update();
	void Draw();

	void Playback(Type type, float duration = 1.0f, float waitingTime = 0.0f);

private:
	bool FadeOut();
	bool FadeIn();

private:
	Screen() = default;
	~Screen() = default;
	Screen(const Screen&) = delete;
	const Screen& operator=(const Screen&) = delete;

private:
	bool isOver_;
	bool flap_;
	std::unique_ptr<Sprite> sprite_;
	Vector4 color_;
	Type type_;

	float t_;
	float duration_;
	float waitingTime_;

};