#pragma once

#include "Framework.h"
#include "CameraManager.h"

/*
* @brief ゲームマネージャークラス
*/
class GameManager : public Framework
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ファイナライザ
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	
};

