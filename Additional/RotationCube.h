#pragma once
#include "Object3D.h"
#include "WindParticle.h"
/*
* @brief 回転オブジェクトクラス
*/
class RotationCube : public yunity::Object3D {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(float torque);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
private:
	float torque_ = 0.0f;

};