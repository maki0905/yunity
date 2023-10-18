#pragma once
#include <memory>

#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "ViewProjection.h"
#include "WorldTransform.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	DirectXCommon* dxCommon_ = nullptr;
	ViewProjection viewProjection_;
	WorldTransform worldTransform_;
	Input* input_ = nullptr;

	std::unique_ptr <Model> model_ = nullptr;

};

