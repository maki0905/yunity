#pragma once

#include <memory>

#include "Model.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "Collider.h"

/*
* @brief スカイドームクラス
*/
class Skydome
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera, Vector3 size = {1.0f, 1.0f, 1.0f});

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
	std::unique_ptr<Model> model_;
};

