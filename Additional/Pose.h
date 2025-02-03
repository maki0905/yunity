#pragma once

#include "Sprite.h"
#include "Input.h"

class Player;

class Pose {
public:
	void Initialize(Player* player);
	bool Update();
	void Draw();
	bool GetIsPose() { return isPose_; }
private:
	//// コントローラー
	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	Player* player_;

	bool isPose_;

	float time_;

	// ポーズボタン
	std::unique_ptr<yunity::Sprite> poseBotton_;
	// ポーズフォント
	std::unique_ptr<yunity::Sprite> pose_;
	//ポーズ画面
	std::unique_ptr<yunity::Sprite> poseBase_;
	// ポーズの背景
	std::unique_ptr<yunity::Sprite> poseBack_;
	// タイトルフォント
	std::unique_ptr<yunity::Sprite> title_;
	// つづきフォント
	std::unique_ptr<yunity::Sprite> continuation_;
	// リスタートフォント
	std::unique_ptr<yunity::Sprite> restart_;
	// セレクト
	std::unique_ptr<yunity::Sprite> select_;
	Vector2 selectPosition_;

};