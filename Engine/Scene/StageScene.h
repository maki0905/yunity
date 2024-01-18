#pragma once

#include <memory>

#include "IScene.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "Model.h"
#include "Sprite.h"
#include "PrimitiveDrawer.h"
#include "Player.h"
#include "Skydome.h"
#include "ParticleDrawer.h"
#include "Block.h"
#include "CollisionManager.h"
#include "Random.h"

class StageScene : public IScene
{
public:
	void Initialize() override;
	void Update() override;
	void DrawBack() override;
	void Draw3D() override;
	void DrawFront() override;
private:
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Model> skydome_ = nullptr;
	std::unique_ptr<PrimitiveDrawer> sphere_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector2 position_;

	Camera camera_;
	WorldTransform worldTransform_;
	WorldTransform worldTrasnform1_;
	WorldTransform worldTransformParticle_[10];

	Player* player_ = nullptr;
	Block* block_ = nullptr;
	std::unique_ptr<Skydome> skydome1_;

	std::unique_ptr<CollisionManager> collisionManager_;

	ParticleDrawer* particle_ = nullptr;
	uint32_t index_particle = 0;
	

	float mass_ = 1.0f;
	float gravityScale_ = 0.1f;
	float miu_ = 0.5f;


	Vector3 velocity_[10];
	Vector3 acceleration_{};

	Random::RandomNumberGenerator rng;

	//std::unique_ptr<Player> player_ = nullptr;
	
};

