#pragma once

#include <array>

#include "Sprite.h"

class GuideUI {
public:
	// ガイドの種類
	enum class GuideType {
		ABotton,
		RBBotton,
		Jump,
		Wire
	};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// テクスチャハンドル
	/// </summary>
	/// <param name="type"></param>
	/// <param name="num"></param>
	void SetTexrture(GuideType type, int num);

	/// <summary>
	/// 外部ファイルからデータ読み込み
	/// </summary>
	void ApplyGlobalVariables();
private:
	// Aボタン
	std::unique_ptr<yunity::Sprite> guideA_;
	std::array<uint32_t, 2> guideATexture_;
	Vector2 guideAPosition_;
	Vector2 guideASize_;

	// RBボタン
	std::unique_ptr<yunity::Sprite> guideRB_;
	std::array<uint32_t, 2> guideRBTexture_;
	Vector2 guideRBPosition_;
	Vector2 guideRBSize_;

	// ジャンプ
	std::unique_ptr<yunity::Sprite> guideJump_;
	Vector2 guideJumpPosition_;
	Vector2 guideJumpSize_;

	// ワイヤーの発射・解除
	std::unique_ptr<yunity::Sprite> guideWire_;
	std::array<uint32_t, 2> guideWireTexture_;
	Vector2 guideWirePosition_;
	Vector2 guideWireSize_;

};