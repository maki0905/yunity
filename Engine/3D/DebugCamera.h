#pragma once

#include "Input.h"
#include "Camera.h"



/// <summary>
/// デバッグ用カメラ
/// </summary>
class DebugCamera {
	// カメラ注視点までの距離
	static const float distance_;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DebugCamera();

	// 更新
	void Update(Camera* camera);

};