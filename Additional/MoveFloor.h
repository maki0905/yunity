#pragma once
#include "Object3D.h"
#include "Tag.h"
/*
* @brief 動く床のクラス
*/
class MoveFloor : public yunity::Object3D
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void InitializeDirection(Tag type);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
private:
	float time_;
	float amplitude_;
	float cycle_;
};

