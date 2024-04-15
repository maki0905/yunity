#pragma once

#include "Model.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "Collider.h"

class Skydome
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


private:
	WorldTransform worldTransform_;
	std::unique_ptr<Model> model_ = nullptr;
};

