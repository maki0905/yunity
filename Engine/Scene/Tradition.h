#pragma once

#include "Sprite.h"

enum TraditionMode {
	kNoneTradition,
	kFadein,
	kFadeout,
	kMax
};

/*
* @brief トラディションクラス
*/
class Tradition {
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static Tradition* GetInstance();
	
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
	/// スタート
	/// </summary>
	void Start() { in_ = true; isActive_ = true; }
	
	/// <summary>
	/// 終了確認
	/// </summary>
	/// <returns></returns>
	bool End() { return isActive_; }
	
	/// <summary>
	/// /
	/// </summary>
	/// <param name="second"></param>
	void SetSecond(float second) { second_ = second; }
	
	/// <summary>
	/// アクティブ取得
	/// </summary>
	/// <returns></returns>
	bool GetIsActive() { return isActive_; }
	
	/// <summary>
	/// イン取得
	/// </summary>
	/// <returns></returns>
	bool GetIn() { return in_; }
	
	/// <summary>
	/// アウト取得
	/// </summary>
	/// <returns></returns>
	bool GetOut() { return out_; }

	/// <summary>
	/// タイムの取得
	/// </summary>
	/// <returns></returns>
	float GetTime() { return time_; }

private:
	std::unique_ptr<Sprite> sprite_;

	TraditionMode mode_;
	bool isActive_;
	bool in_;
	bool out_;
	float time_;
	float second_;
};