#pragma once
#include "Object3D.h"

#include "PillarTop.h"
/*
* @brief ポールクラス
*/
class Pillar : public yunity::Object3D {
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
	/// 描画
	/// </summary>
	void Draw() override;

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
	float toruqe_ = 2.0f;
	std::unique_ptr<PillarTop> top_;
};