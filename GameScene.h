#pragma once
#include <memory>

#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#pragma region 新規作成
#include "Skydome.h"
#include "Player.h"

#pragma endregion


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

	// モデル
	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Model> playerModel_Body_;
	std::unique_ptr<Model> playerModel_Head_;
	std::unique_ptr<Model> playerModel_Larm_;
	std::unique_ptr<Model> playerModel_Rarm_;

	// オブジェクト
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Player> player_;
};

