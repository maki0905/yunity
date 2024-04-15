#pragma once

#include <memory>

#include "IScene.h"

#include "Camera.h"
#include "DebugCamera.h"
#include "Player.h"
#include "BlockManager.h"
#include "Block.h"
#include "Sprite.h"
#include "Skydome.h"
#include "World.h"
#include "RailCamera.h"
#include "Enemy.h"
#include "EnemyBullet.h"



class StageScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生データの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 敵発生
	/// </summary>
	void EnemyPop(Vector3 position);

private:
	bool isDebug_ = false;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<DebugCamera> debugCamera_;

	XINPUT_STATE pud_;

	// 自キャラ
	std::unique_ptr<Player> player_ = nullptr;

	// 敵キャラ
	std::list<Enemy*> enemys_;

	// 敵弾
	std::list<EnemyBullet*> enemyBullets_;
	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 天球
	std::unique_ptr<Skydome> skydome_ = nullptr;

	// レールカメラ
	std::unique_ptr<RailCamera> railCamera_ = nullptr;

	// スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;

	// 待機中フラグ
	bool waitFlag_ = false;
	// 待機タイマー
	int32_t waitTimer_ = 0;


	std::unique_ptr<World> world_;

	

};

