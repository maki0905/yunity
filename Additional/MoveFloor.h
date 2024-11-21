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
	float amplitude_ = 0.08f;
	const float secound_ = 1.0f / 60.0f;
	float time_ = 0.0f;
	float cycle_ = 5.0f;
};

