#pragma once

#include "Object3D.h"

/*
* @brief TVクラス
*/
class TV :public yunity::Object3D{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void InitializeTexture();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 衝突時のイベント
	/// </summary>
	void OnTriggerEvent() override;

private:
	float time_;
	float changeTime_;
	bool isActive_;
	bool change_;
	uint32_t textureHandle_[2];
};