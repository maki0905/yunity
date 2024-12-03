#pragma once
#include "Object3D.h"

/*
* @brief ポールクラス
*/
class PillarTop : public yunity::Object3D {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// getter
	/// </summary>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// setter
	/// </summary>
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	void SetIsRoll(bool isRoll) { isRoll_ = isRoll; }

private:
	bool isActive_ = true;
	bool isRoll_ = false;
	float toruqe_ = 10.0f;
};