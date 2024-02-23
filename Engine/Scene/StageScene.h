#pragma once

#include <memory>
#include <list>
#include <iostream>

#include "IScene.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Model.h"
#include "Sprite.h"
#include "CollisionManager.h"
#include "Random.h"
#include "DebugCamera.h"
#include "FollowCamera.h"
#include "ParticleManager.h"
#include "SphereDrawer.h"
#include "Audio/Audio.h"
#include "ModelManager.h"

#pragma region 新規作成
#include "Skydome.h"
#include "Floor.h"
#include "MovingFloor.h"
#include "Player.h"
#include "Enemy.h"
#include "StartBox.h"
#include "EndBox.h"
#include "LockOn.h"
#include "Explosion.h"
#pragma endregion

class StageScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;

	void Reset();
private:

	std::unique_ptr<Camera> camera_;
	std::unique_ptr<DebugCamera> debugCamera_;
	bool isDebug_ = false;

	// �Ǐ]�J����
	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// ���f��
	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Model> floorModel_;
	std::unique_ptr<Model> movingFloorModel_;
	std::unique_ptr<Model> startBoxModel_;
	std::unique_ptr<Model> endBoxModel_;

	std::unique_ptr<Model> playerModel_Body_;
	std::unique_ptr<Model> playerModel_Head_;
	std::unique_ptr<Model> playerModel_Larm_;
	std::unique_ptr<Model> playerModel_Rarm_;
	std::unique_ptr<Model> playerModel_Hammer_;
	std::unique_ptr<Model> enemyModel_Body_;
	std::unique_ptr<Model> enemyModel_Larm_;
	std::unique_ptr<Model> enemyModel_Rarm_;

	std::unique_ptr<Model> particle_;

	// �I�u�W�F�N�g
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<StartBox> startBox_;
	std::unique_ptr<EndBox> endBox_;
	std::vector<Floor*> floors_;
	std::vector<MovingFloor*> movingFloors_;
	std::unique_ptr<Player> player_;
	//std::unique_ptr<Enemy> enemy_;
	std::list<std::unique_ptr<Enemy>> enemies_;
	std::unique_ptr<LockOn> lockOn_;


	Vector3 enemyPos_[5];
	//std::unique_ptr<Pirticle> pirticle_;
	std::list<Explosion*> explosion_;

	Random::RandomNumberGenerator rng;


	
};

