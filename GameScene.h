#pragma once
#include <memory>

#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "CollisionManager.h"
#include "FollowCamera.h"

#pragma region 新規作成
#include "Skydome.h"
#include "Floor.h"
#include "MovingFloor.h"
#include "Player.h"
#include "Enemy.h"
#include "StartBox.h"
#include "EndBox.h"
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

	// リセット
	void Reset();

private:
	DirectXCommon* dxCommon_ = nullptr;
	ViewProjection viewProjection_;

	// デバッグカメラフラグ
	bool isDebugCameraFlag_ = false;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;

	// 追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// モデル
	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Model> floorModel_;
	std::unique_ptr<Model> movingFloorModel_;
	std::unique_ptr<Model> startBoxModel_;
	std::unique_ptr<Model> endBoxModel_;

	std::unique_ptr<Model> playerModel_Body_;
	std::unique_ptr<Model> playerModel_Head_;
	std::unique_ptr<Model> playerModel_Larm_;
	std::unique_ptr<Model> playerModel_Rarm_;
	std::unique_ptr<Model> enemyModel_Body_;
	std::unique_ptr<Model> enemyModel_Larm_;
	std::unique_ptr<Model> enemyModel_Rarm_;

	// オブジェクト
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<StartBox> startBox_;
	std::unique_ptr<EndBox> endBox_;
	std::vector<Floor*> floors_;
	std::vector<MovingFloor*> movingFloors_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Enemy> enemy_;
};

