#pragma once
#include "BaseObject.h"
class Floor : public BaseObject
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
	void Draw(const Camera& camera) override;

	void SetPosition(const Vector3& position);

	void SetScale(const Vector3& scale);

private:
	WorldTransform worldTransform_;

};

