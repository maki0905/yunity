#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <vector>


class Player
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	WorldTransform worldTransform_Body_;
	WorldTransform worldTransform_Head_;
	WorldTransform worldTransform_Larm_;
	WorldTransform worldTransform_Rarm_;

	// モデル
	std::vector<Model*> models_;


	Model* model_ = nullptr;
};

