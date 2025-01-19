#pragma once

#include <memory>

#include "IScene.h"


/*
* @brief セレクトシーンクラス
*/
class SelectScene : public yunity::IScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawBack() override;
	
	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw3D() override;
	
	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawFront() override;

private:
};