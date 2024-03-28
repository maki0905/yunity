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
	void Initialize(Model* model, Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	Collider* collider_;
};

