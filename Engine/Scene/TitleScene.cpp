#include "TitleScene.h"

#include "SceneManager.h"

void TitleScene::Initialize()
{
	//camera_.reset(CameraManager::GetInstance()->GetCamera());
	camera_ = CameraManager::GetInstance()->GetCamera();
	worldTransform_.Initialize();

	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("uvChecker.png"), { 0.0f, 0.0f }));

	model_ = std::make_unique<Model>();
	model_.reset(Model::Create("terrain"));
	model_->SetCamera(camera_/*camera_.get()*/);
	model_->SetLighting(false);
}

void TitleScene::Update()
{

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
	}

	worldTransform_.UpdateMatrix(RotationType::Euler);
}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	model_->Draw(worldTransform_);
}

void TitleScene::DrawFront()
{
	//sprite_->Draw();
}

