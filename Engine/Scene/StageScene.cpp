#include "StageScene.h"

#include "Input.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "ModelManager.h"

void StageScene::Initialize()
{
	world_ = std::make_unique<World>();
	world_->Initialize();

	camera_ = std::make_unique<Camera>();
	debugCamera_ = std::make_unique<DebugCamera>();
	isDebug_ = false;

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_.get());

	blockManager_ = std::make_unique<BlockManager>();
	blockManager_->Initialize(camera_.get(), world_.get());

	start_ = std::make_unique<Model>();
	start_.reset(ModelManager::GetInstance()->CreateModel(obj, false, "startBox"));
	start_->SetCamera(camera_.get());
	worldTransform_start_.Initialize();
	worldTransform_start_.translation_ = { -20.0f, 5.0f, 0.0f };
	worldTransform_start_.UpdateMatrix(RotationType::Euler);

	end_ = std::make_unique<Model>();
	start_.reset(ModelManager::GetInstance()->CreateModel(obj, false, "startBox"));
	end_->SetCamera(camera_.get());
	worldTransform_end_.Initialize();
	worldTransform_end_.translation_ = { 50.0f, 3.0f, 0.0f };
	worldTransform_end_.UpdateMatrix(RotationType::Euler);

	world_->Add(player_.get());

	player_->SetTranslation(worldTransform_start_.translation_);

	primitiveDrawer_.reset(PrimitiveDrawer::Create(PrimitiveDrawer::Type::kBox));
	primitiveDrawer_->SetCamera(camera_.get());

}

void StageScene::Update()
{
	player_->Update();

	if (player_->GetTranslation().x > worldTransform_end_.translation_.x) {
		player_->SetTranslation(worldTransform_start_.translation_);
	}

	blockManager_->Update(world_.get());

	world_->Solve();

	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
	}

	if (isDebug_) {
		debugCamera_->Update(camera_.get());
		camera_->Update();
	}
	else {
		camera_->Update();
	}
}

void StageScene::DrawBack()
{
}

void StageScene::Draw3D()
{
	player_->Draw();
	blockManager_->Draw();
	primitiveDrawer_->Draw(worldTransform_start_);
	start_->Draw(worldTransform_start_);
	end_->Draw(worldTransform_end_);
	//primitiveDrawer_->Draw(worldTransform_start_);
}

void StageScene::DrawFront()
{
	
}
