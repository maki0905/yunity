#pragma once
#include "Object3D.h"

/*
* @brief コインクラス
*/
class Coin : public Object3D{
public:
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
	void OnCollisionEvent() override;

	/// <summary>
	/// 衝突時のイベント
	/// </summary>
	void OnTriggerEvent() override;

	/// <summary>
	/// getter
	/// </summary>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// setter
	/// </summary>
	void SetIsActive(bool isActive) { isActive_ = isActive; }

private:
	bool isActive_ = true;
};