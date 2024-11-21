#pragma once

#include "Input.h"

enum SCENE { TITLE, GAME_STAGE, CLEAR, COUNT };

namespace yunity {
	/*
	* @brief 基底シーンクラス
	*/
	class IScene
	{
	public:

		virtual ~IScene() {};
		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// 背景描画
		/// </summary>
		virtual void DrawBack() = 0;

		/// <summary>
		/// 3D描画
		/// </summary>
		virtual void Draw3D() = 0;

		/// <summary>
		/// 前景描画
		/// </summary>
		virtual void DrawFront() = 0;


	};

}

