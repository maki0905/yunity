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

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:
	std::unique_ptr<Sprite> scoreBackground_;
	std::unique_ptr<Sprite> scoreNumber_[4];
	uint32_t score_;
	uint32_t digit_;
	Vector2 pos_ = { 84.0f, 84.0f };
	uint32_t addScore_;
};