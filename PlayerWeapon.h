#pragma once
#include "BaseObject.h"
class PlayerWeapon : public BaseObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	bool GetIsActive() { return isActive_; }

	void SetParent(WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

private:
	bool isActive_;
	float parameter_;

};

