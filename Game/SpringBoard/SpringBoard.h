#pragma once
#include "Object3D.h"
#include "SpringJoint.h"



class SpringBoard : public yunity::Object3D {
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

	void SetFixedPosition(Vector3 fixedPosition) { fixedPosition_ = fixedPosition; }

	/// <summary>
	/// スプリングジョイントの設定
	/// </summary>
	/// <param name="obj"></param>
	void SetSpringJoint();

	/// <summary>
	/// 衝突時のイベント
	/// </summary>
	void OnCollisionEvent() override;

private:
	// スプリングジョイント
	std::unique_ptr<yunity::SpringJoint> springJoint_;
	// アンカー
	std::unique_ptr<yunity::Object3D> anchor_;
	// モデル
	std::unique_ptr<yunity::Model> springModel_;
	// ワールド変換
	yunity::WorldTransform springWorldTransform_;
	// 固定位置
	Vector3 fixedPosition_;
	// 剛性
	float stiffness_ ;
	// 自然長
	float equilibriumPoint_;
	// 減衰
	float damping_;
	// アンカーの位置
	Vector3 anchorPosition_;
	// 伸び時間
	float elongationTime_;
	bool flag_;
	float time_;
};