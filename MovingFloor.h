#pragma once
#include "BaseObject.h"
#include "Plane.h"
class MovingFloor :public BaseObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models_">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(const ViewProjection& viewProjection) override;

	void SetPosition(const Vector3& position);
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
private:

	Vector3 startPosition_;
	Vector3 velocity_;

};

