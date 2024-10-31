#pragma once

#include "Object3D.h"

/*
* @brief TVクラス
*/
class TV :public Object3D{
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
	bool isActive_;
	bool preIsActive_;
	bool change_;
	bool isGrow_;
	float growTime_;
	const float second_ = 1.0f / 60.0f;
	uint32_t textureHandle_[2];
	WorldTransform modelWorldTransform_;
};