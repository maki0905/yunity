#pragma once
#include "Object3D.h"

/*
* @brief チェックポイントクラス
*/
class CheckPoint : public yunity::Object3D {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(yunity::Camera* camera);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 衝突時のイベント
	/// </summary>
	void OnTriggerEvent() override;

private:
	bool isActive_ = false;
};