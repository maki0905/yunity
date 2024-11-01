#pragma once

#include <memory>


#include "IScene.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "Model.h"
#include "World.h"
#include "SkyBox.h"
#include "Player.h"
#include "World.h"
#include "ObjectManager.h"
#include "Skydome.h"
#include "Coin.h"
#include "SpringBoard.h"

/*
* @brief ステージシーンクラス
*/
class StageScene : public IScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawBack() override;
	
	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw3D() override;
	
	/// <summary>
	/// 前景描画
	/// </summary>
	void DrawFront() override;
private:
	/*bool isDebug_ = false;

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<DebugCamera> debugCamera_;


	std::unique_ptr<Player> player_;
	std::unique_ptr<BlockManager> blockManager_;

	std::unique_ptr<Model> start_;
	WorldTransform worldTransform_start_;
	std::unique_ptr<Model> end_;
	WorldTransform worldTransform_end_;

	std::unique_ptr<World> world_;
	std::unique_ptr<PrimitiveDrawer> primitiveDrawer_;*/
	bool isDebug_ = false;
	bool inStage_ = false;

	Camera* camera_;
	//std::unique_ptr<Camera> camera_;
	std::unique_ptr<DebugCamera> debugCamera_;

	//std::vector<Object3D*> objects_;

	std::unique_ptr<SkyBox> skybox_;
	WorldTransform skyboxWorldTransform_;
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Player> player_;

	std::unique_ptr<World> world_;

	/*std::unique_ptr<Model> start_;
	WorldTransform startWT_;
	std::unique_ptr<Model> end_;
	WorldTransform endWT_;*/
	WorldTransform startWT_;
	std::vector<Object3D*> floors_;
	std::vector<Object3D*> trampolines_;

	std::string stageName_ = "stage1";

	float moveCameraTimer_;
	Vector3 oldCameraPos_;

	std::unique_ptr<Model> start_;
	Vector3 startPos_;
	uint32_t textureTV_;

	std::unique_ptr<ObjectManager> objectManager_;

	bool isReset_;
	bool isDebt_;
	float upTime_;
	float downTime_;
	float resetTime_;
	float sizeTime_;
	Vector3 dieCamera_;
	Vector3 diePos_;
	Vector3 topPos_;

	// ジャンプ台
	static const uint32_t springBoardCount_ = 2;
	std::array<std::unique_ptr<SpringBoard>, springBoardCount_> springTops_;
	std::array<std::unique_ptr<Object3D>, springBoardCount_> springAnchores_;
	std::array<std::unique_ptr<PrimitiveDrawer>, springBoardCount_> springLines_;

	// 吊り橋
	static const uint32_t bridgeCount_ = 9;
	std::array<std::unique_ptr<Object3D>, bridgeCount_> bridge_;
	std::array<std::unique_ptr<PrimitiveDrawer>, bridgeCount_ - 1> bridgeLines_;

	/*std::unique_ptr<Object3D> spike_;
	std::unique_ptr<Object3D> stand_;
	std::unique_ptr<SpringJoint> springJoint_;

	std::unique_ptr<PrimitiveDrawer> line_;*/

};

