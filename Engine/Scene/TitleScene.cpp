#include "TitleScene.h"

#include "SceneManager.h"

void TitleScene::Initialize()
{
	camera_ = std::make_unique<Camera>();

	worldTransform_.Initialize();

	/*sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("uvChecker.png"), { 0.0f, 0.0f }));*/

	model0_ = std::make_unique<Model>();
	model0_.reset(Model::Create("terrain"));
	model0_->SetCamera(camera_.get());
	model0_->SetLighting(false);

	model1_ = std::make_unique<Model>();
	model1_.reset(Model::Create("hammer"));
	model1_->SetCamera(camera_.get());
	model1_->SetLighting(false);

	

	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_->Initialize();
	particleManager_->SetCamera(camera_.get());
	ParticleEmitter* particleEmitter0 = new ParticleEmitter();
	particleEmitter0->SetCamera(camera_.get());
	particleEmitter0->Initialize({ .scale{1.0f, 1.0f, 1.0f}, .rotate{0.0f, 0.0f, 0.0f}, .translate{0.0f, 0.0f, 0.0f} }, 1.0f, 1.0f);
	particleEmitter0->SetTexturen("circle.png");
	particleManager_->AddEmitter(particleEmitter0);


}

void TitleScene::Update()
{

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
	}

	worldTransform_.UpdateMatrix(RotationType::Euler);

	particleManager_->Update();

	camera_->Update();

}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	model0_->Draw(worldTransform_);
	model1_->Draw(worldTransform_);
	particleManager_->Draw();
}

void TitleScene::DrawFront()
{
	//sprite_->Draw();
}

