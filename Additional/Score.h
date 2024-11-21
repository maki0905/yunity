#pragma once
#include <memory>

#include "Sprite.h"

/*
* @brief スコアクラス
*/
class Score {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// スコア加算
	/// </summary>
	/// <param name="addScore">加算するスコア</param>
	void AddScore(uint32_t addScore) { addScore_ += addScore; }

	uint32_t GetScore() { return score_; }

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 位置設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector2 position) { pos_ = position; }

	/// <summary>
	/// ハイスコアテキスト表示設定
	/// </summary>
	/// <param name="flag"></param>
	void SetDisplayHiScore(bool flag) { isHiScore_ = flag; }

private:
	std::unique_ptr<yunity::Sprite> scoreBackground_;
	std::unique_ptr<yunity::Sprite> hiScore_;
	std::unique_ptr<yunity::Sprite> scoreNumber_[4];
	uint32_t score_;
	uint32_t digit_;
	Vector2 pos_ = { 84.0f, 84.0f };
	uint32_t addScore_;
	bool isHiScore_;
	float lerpTime_;
	const float deltaTime_ = 1.0f / 30.0f;
	const Vector2 minHiScoreSize_ = { 120.0f, 30.0f };
	const Vector2 maxHiScoreSize_ = { 180.0f, 45.0f };
};