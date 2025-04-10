#pragma once
#include "Object3D.h"
#include "Particle/WindParticle.h"
/*
* @brief 力場クラス
*/
class ForceField : public yunity::Object3D {
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
	/// 衝突時のイベント
	/// </summary>
	void OnTriggerEvent() override;

private:
	bool isActive_ = true;
	bool isWind_ = false;
	float time_ = 0;
	// 制限時間
	float limitTime_;
	// 上昇量
	float force_ ;
	// パーティクル
	std::unique_ptr<WindParticle> windParticle_;

};