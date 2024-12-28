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
	virtual void Update();

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
	std::unique_ptr<yunity::SpringJoint> springJoint_;

	std::unique_ptr<yunity::Object3D> anchor_;

	Vector3 fixedPosition_;

	const float stiffness_ = 20.0f;
	const float equilibriumPoint_ = 10.0f;
	const float damping_ = 1.0f;
	bool flag_;
	float time_;
};