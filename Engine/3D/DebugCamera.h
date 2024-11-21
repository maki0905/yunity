#pragma once

#include "Input.h"
#include "Camera.h"


namespace yunity{
	/// <summary>
/// デバッグ用カメラ
/// </summary>
	class DebugCamera {
		// カメラ注視点までの距離
		static const float distance_;

	public:
		// 更新
		void Update(Camera* camera);

	};
}