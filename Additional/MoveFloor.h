#pragma once
#include "Object3D.h"

/*
* @brief 動く床のクラス
*/
class MoveFloor : public Object3D
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void InitializeDirection();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
private:
	float amplitude_ = 0.08f;
	float time_ = 0.0f;
	float cycle_ = 5.0f;
};

