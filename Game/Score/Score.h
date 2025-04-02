#pragma once
#include <memory>
#include <array>

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
	void SetPosition(Vector2 position) { scorePosition_ = position; }

	/// <summary>
	/// ハイスコアテキスト表示設定
	/// </summary>
	/// <param name="flag"></param>
	void SetDisplayHiScore(bool flag) { isHiScore_ = flag; }

private:
	// スコアの最大桁
	static const int maxDigit = 4;
	// スコア背景
	std::unique_ptr<yunity::Sprite> scoreBackground_;
	// ハイスコア
	std::unique_ptr<yunity::Sprite> hiScore_;
	// スコア数
	std::unique_ptr<yunity::Sprite> scoreNumber_[maxDigit];
	// 現在のスコア
	uint32_t score_;
	// 現在の桁数
	uint32_t digit_;
	// スコア表示の位置
	Vector2 scorePosition_;
	// 加算されるスコア
	uint32_t addScore_;
	// ハイスコアの表示フラグ
	bool isHiScore_;
	// 線形補間用タイム
	float lerpTime_;
	// 線形補間用の制限時間
	std::array<float, 2> period_;
	// スコア背景のサイズ
	Vector2 scoreBackSize_;
	// ハイスコアのスコア表示位置からみた位置
	Vector2 hiScorePosition_;
	// ハイスコアの最小サイズ
	Vector2 minHiScoreSize_;
	// ハイスコアの最大サイズ
	Vector2 maxHiScoreSize_;
	// スコア数のサイズ
	Vector2 scoreNumberSize_;
	// スコア数のテクスチャ左上座標
	Vector2 scoreNumberTexBase_;
	// スコア数のテクスチャサイズ
	float scoreNumberTextureSize_;
	// スコア数同士の幅
	float scoreNumberWidth_;
};