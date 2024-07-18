#include "ClearScene.h"

#include "ModelManager.h"
#include "LevelEditor.h"

void ClearScene::Initialize()
{
	//camera_ = std::make_unique<Camera>();
	//debugCamera_ = std::make_unique<DebugCamera>();
	//isDebug_ = false;

	//model0_ = std::make_unique<Model>();
	////model0_.reset(Model::Create("terrain", "obj"));
	////model0_.reset(Model::Create("plane", "gltf"));
	////model0_.reset(ModelManager::GetInstance()->CreateModel(gltf, true, "simpleSkin"));
	//model0_.reset(ModelManager::GetInstance()->CreateModel(gltf, "human", "sneakWalk", ModelType::kSkin));
	//model0_->SetCamera(camera_.get());
	//worldTransform0_.Initialize();
	//worldTransform0_.scale_ = Vector3(10.0f, 10.0f, 10.0f);
	//worldTransform0_.translation_ = { 10.0f, 0.0f, 0.0f };
	//worldTransform0_.quaternion_ = { 0.0f, 1.0f, 0.0f, 0.0f };

	//model0_->SetAnimation("walk", ModelManager::GetInstance()->GetAnimation(gltf, "human", "walk"), AnimationCommon::kLooping);
	//model0_->SetAnimation("sneakWalk", ModelManager::GetInstance()->GetAnimation(gltf, "human", "sneakWalk"), AnimationCommon::kLooping);

	//model1_ = std::make_unique<Model>();
	//model1_.reset(ModelManager::GetInstance()->CreateModel(gltf, "human", "walk", ModelType::kSkin));
	//model1_->SetCamera(camera_.get());
	//worldTransform1_.Initialize();
	//worldTransform1_.scale_ = Vector3(10.0f, 10.0f, 10.0f);
	//worldTransform1_.translation_ = { -10.0f, 0.0f, 0.0f };
	//worldTransform1_.quaternion_ = { 0.0f, 1.0f, 0.0f, 0.0f };

	//model2_ = std::make_unique<Model>();
	//model2_.reset(ModelManager::GetInstance()->CreateModel(obj, "needle_Body"));
	//model2_->SetCamera(camera_.get());
	//worldTransform2_.Initialize();
	//worldTransform2_.scale_ = { 100.0f, 100.0f, 100.0f };

	//uint32_t handle = TextureManager::GetInstance()->Load("rostock_laage_airport_4k.dds");
	////worldTransform2_.scale_ = { 10.0f, 10.0f, 10.0f };

	//skybox_ = std::make_unique<SkyBox>();
	//skybox_.reset(SkyBox::Create());
	//skybox_->SetCamera(camera_.get());
	//skybox_->SetTexture("rostock_laage_airport_4k.dds");

	//world_ = std::make_unique<World>();
	//world_->Initialize();


	//player_ = std::make_unique<Player>();
	//player_->Initialize(camera_.get(), world_.get());

	//objects_.clear();
	//LevelData* levelData = LevelEditor::GetInstance()->LoadFile("stage");
	
	// レベルデータからオブジェクトを生成、配置
	//for (auto& objectData : levelData->objects) {
	//	// ファイル名から登録済みモデルを検索
	//	Model* model = nullptr;
	//	model = ModelManager::GetInstance()->CreateModel(obj, objectData.fileName);
	//	model->SetCamera(camera_.get());

	//	Object3D* newObject = new Object3D();
	//	newObject->Create(model, world);
	//	newObject->SetPosition(objectData.translation);
	//	newObject->SetRotation(objectData.rotation);
	//	newObject->SetScale(objectData.scaling);

	//	objects_.push_back(newObject);
	//}


}

void ClearScene::Update()
{
	//worldTransform0_.rotation_.y += 0.05f;
	//worldTransform0_.quaternion_.y += 0.01f;
	//worldTransform0_.rotation_.y += 0.05f;
	//worldTransform0_.UpdateMatrix(RotationType::Euler);
	worldTransform0_.UpdateMatrix();
	worldTransform1_.UpdateMatrix();
	//worldTransform2_.rotation_.y += 1.0f;
	worldTransform2_.UpdateMatrix();

	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		model0_->PlayAnimation("walk", AnimationCommon::kLooping);
	}
	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		model0_->PlayAnimation("sneakWalk", AnimationCommon::kLooping);
	}

	player_->Update();

	world_->Solve();

	for (auto& object : objects_) {
		object->Update();
	}

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

void ClearScene::DrawBack()
{
}

void ClearScene::Draw3D()
{
	skybox_->Draw(worldTransform2_);
	/*for (auto& object : objects_) {
		object->Draw();
	}*/
	/*model0_->Draw(worldTransform0_);
	model1_->Draw(worldTransform1_);*/
	//model2_->Draw(worldTransform2_);

	player_->Draw();

}

void ClearScene::DrawFront()
{
}
