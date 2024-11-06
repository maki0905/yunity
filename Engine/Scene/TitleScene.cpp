#include "TitleScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "RenderTexture.h"
#include "ObjectManager.h"
#include "ImGuiManager.h"
#include "CommonData.h"
#include "Tradition.h"

void TitleScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	world_ = std::make_unique<World>();
	world_->Initialize({ 0.0f, -15.0, 0.0f });

	bottonSprite_ = std::make_unique<Sprite>();
	bottonSprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ABotton.dds"), { 610.0f, 520.0f }));
	bottonPushSprite_ = std::make_unique<Sprite>();
	bottonPushSprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ABottonPush.png"), { 610.0f, 520.0f }));
	time_ = 0;

	model_ = std::make_unique<Model>();
	model_.reset(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Signboard"));
	Model::DirectionalLight l = { .color = {1.0f, 1.0f, 1.0f, 1.0f}, .direction = {1.0f, -1.0f, 0.0f}, .intensity= 1.0f };
	model_->SetCamera(camera_/*camera_.get()*/);
	model_->SetEnableLighting(true);
	model_->SetDirectionalLight(l);
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 25.0f, -7.0f, 6.0f };

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_/*camera_.get()*/, world_.get());
	player_->SetDirectionalLight(l);
	player_->SetMass(0.0f);
	camera_->SetTarget(nullptr);  

	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize();
	objectManager_->Load("title1", camera_, world_.get());

	
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(camera_/*camera_.get()*/, { 5.0f, 5.0f, 5.0f });

	CommonData::GetInstance()->stageNum_ = -1;
	CommonData::GetInstance()->scene_ = Scene::kTitle;

	isStart_ = false;

	for (uint32_t index = 0; index < 3; index++) {
		models_[index] = std::make_unique<Model>();
		models_[index].reset(ModelManager::GetInstance()->CreateModel(obj, "TV"));
		models_[index]->SetCamera(camera_);
		models_[index]->SetEnableLighting(false);
		TVworldTransform_[index].Initialize();
		TVworldTransform_[index].translation_ = { 80.0f + 30.0f * index, 18.0f, 12.0f };
		TVworldTransform_[index].rotation_.x = -15.0f * DegToRad();
		TVworldTransform_[index].scale_ = { 0.0f, 0.0f, 0.0f };
		isActiveTV_[index] = false;
		grow_[index] = { false, 0.0f };
		shrink_[index] = { false, 0.0f };
	}
	textureTV_[0] = TextureManager::Load("Models/TV/TV1.png");
	textureTV_[1] = TextureManager::Load("Models/TV/TV2.png");
	textureTV_[2] = TextureManager::Load("Models/TV/TV3.png");

	preNum_ = CommonData::GetInstance()->stageNum_;

	if (CommonData::GetInstance()->stageNum_ != -1) {
		player_->SetPosition({ 30.0f * CommonData::GetInstance()->stageNum_, -3.5f, 0.0f });
		player_->SetSelect(true);
		TVworldTransform_[CommonData::GetInstance()->stageNum_].scale_ = { 1.0f, 1.0f, 1.0f };
		isActiveTV_[CommonData::GetInstance()->stageNum_] = true;
		isStart_ = true;
	}
	else {
		camera_->SetTranslate({ 0, 10.0f, -50.0f });
	}
	CommonData::GetInstance()->scene_ = Scene::kTitle;
	isMoveCamera[0] = false;
	isMoveCamera[1] = false;
	moveCameraTimer_ = 0.0f;

	/*cloth_ = std::make_unique<Cloth>();
	cloth_->Initialize(world_.get());*/


}

void TitleScene::Update()
{
	prePad_ = pad_;

	if (player_->GetSelect()) {
		if (preNum_ != CommonData::GetInstance()->stageNum_) {
			grow_[CommonData::GetInstance()->stageNum_] = { true, 0.0f, TVworldTransform_[CommonData::GetInstance()->stageNum_].scale_ };
		}

		for (int index = 0; index < 3; index++) {
			if (index != CommonData::GetInstance()->stageNum_) {
				grow_[index] = { false, 0.0f };
			}
		}

		isActiveTV_[CommonData::GetInstance()->stageNum_] = true;

		if (Input::GetInstance()->IsControllerConnected()) {
			if (Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					//SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
					if (!Tradition::GetInstance()->GetIsActive()) {
						isMoveCamera[0] = true;
						oldCameraPos_ = camera_->GetTranslate();
						moveCameraTimer_ = 0.0f;
						Tradition::GetInstance()->Initialize();
						Tradition::GetInstance()->Start();
						camera_->SetTarget(nullptr);
						player_->SetVelocity({ 0.0f, 0.0f, 0.0f });
					}
					
				}
			}
		}
	}
	else {
		for (uint32_t index = 0; index < 3; index++) {
			if (isActiveTV_[index] && !shrink_[index].flag) {
				shrink_[index] = { true, 0.0f, TVworldTransform_[index].scale_ };
			}
		}

	}

	if (!Tradition::GetInstance()->GetIsActive()) {
		RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, false);
		if (isStart_) {
			player_->SetMass(2.0f);
			player_->Update();
		}
		if (player_->GetWorldTransform()->GetMatWorldTranslation().x >= 35.0f && camera_->GetTarget() == nullptr) {
			camera_->SetTarget(player_->GetWorldTransform());
			CommonData::GetInstance()->scene_ = Scene::kSelect;

		}
		else if (player_->GetWorldTransform()->GetMatWorldTranslation().x < 35.0f && camera_->GetTarget() != nullptr) {
			camera_->SetTarget(nullptr);
			camera_->SetTranslate({ 0, 10.0f, -50.0f });
			CommonData::GetInstance()->scene_ = Scene::kTitle;

		}

		if (Input::GetInstance()->IsControllerConnected()) {
			if (Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					isStart_ = true;
				}
			}
		}
	}
	else {
		if (isMoveCamera[0]) {
			moveCameraTimer_ += 1.0f / 30.0f;
			moveCameraTimer_ = std::clamp(moveCameraTimer_, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(oldCameraPos_, { oldCameraPos_.x, 20.0f, -60.0f }, moveCameraTimer_));
			if (moveCameraTimer_ >= 1.0f) {
				moveCameraTimer_ = 0.0f;
				isMoveCamera[0] = false;
				oldCameraPos_ = camera_->GetTranslate();
				RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, true);
			}
		}
		else {
			moveCameraTimer_ += 1.0f / 60.0f;
			moveCameraTimer_ = std::clamp(moveCameraTimer_, 0.0f, 1.0f);
			camera_->SetTranslate(Lerp(oldCameraPos_, { oldCameraPos_.x, oldCameraPos_.y, 12.0f }, moveCameraTimer_));
			Tradition::GetInstance()->Update();
		}
		if (!Tradition::GetInstance()->GetIn()) {
			RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, false);
			SceneManager::GetInstance()->ChangeScene("GAMESTAGE");
		}
	}

	for (uint32_t index = 0; index < 3; index++) {
		if (grow_[index].flag) {
			grow_[index].t += 1.0f / 60.0f;
			if (grow_[index].t > 1.0f) {
				grow_[index].flag = false;
			}
			TVworldTransform_[index].scale_ = Lerp(/*grow_[index].scale*/{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, std::clamp(grow_[index].t, 0.0f, 1.0f));
		}
		if (shrink_[index].flag) {
			shrink_[index].t += 1.0f / 60.0f;
			if (shrink_[index].t > 1.0f) {
				shrink_[index].flag = false;
				isActiveTV_[index] = false;
			}
			TVworldTransform_[index].scale_ = Lerp(shrink_[index].scale, { 0.0f, 0.0f, 0.0f }, std::clamp(shrink_[index].t, 0.0f, 1.0f));
		}
	}

	preNum_ = CommonData::GetInstance()->stageNum_;
	CommonData::GetInstance()->stageNum_ = -1;

	for (uint32_t index = 0; index < 3; index++) {
		TVworldTransform_[index].UpdateMatrix();
	}

	world_->Solve();

	wavelength += amplitude;
	if (wavelength > 10.0f || wavelength < -10.0f) {
		amplitude *= -1.0f;
	}
	worldTransform_.rotation_.z = wavelength * DegToRad();
	worldTransform_.UpdateMatrix();

	time_++;
	if (time_ % 30 == 0) {
		time_ = 0;
		isDraw_ ^= true;

	}

	//cloth_->Update();

#ifdef _DEBUG
	
	ImGui::Begin("a");
	/*Vector3 velocity = floor_->GetVelocity();
	ImGui::DragFloat3("ve", &velocity.x);*/

	ImGui::DragFloat3("pos", &worldTransform_.translation_.x);
	if (ImGui::Button("Off")) {
		model_->SetEnableLighting(false);
		player_->SetEnableLighting(false);
		/*ObjectManager::GetInstance()->SetEnableLighting("title1",false);*/
	}
	if (ImGui::Button("On")) {
		model_->SetEnableLighting(true);
		player_->SetEnableLighting(true);
		/*ObjectManager::GetInstance()->SetEnableLighting("title1", true);*/
	}
	ImGui::End();

#endif

}

void TitleScene::DrawBack()
{
}

void TitleScene::Draw3D()
{
	skydome_->Draw();
	objectManager_->Draw(/*"title1"*/);

	player_->Draw();
	model_->Draw(worldTransform_);

	for (uint32_t index = 0; index < 3; index++) {
		if (isActiveTV_[index]) {
			models_[index]->Draw(TVworldTransform_[index], textureTV_[index]);
		}
	}
}

void TitleScene::DrawFront()
{
	if (!isStart_ || preNum_ > -1) {
		if (isDraw_) {
			bottonSprite_->Draw();
		}
		else {
			bottonPushSprite_->Draw();
		}
	}
	Tradition::GetInstance()->Draw();
}

