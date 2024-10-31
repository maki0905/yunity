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
	//camera_.reset(CameraManager::GetInstance()->GetCamera());
	camera_ = CameraManager::GetInstance()->GetCamera();
	//camera_->SetTranslate({ 0.0f, 30.0f, -30.0f });
	//camera_->SetRotate({ 45.0f * DegToRad(), 0.0f, 0.0f });
	world_ = std::make_unique<World>();
	world_->Initialize({ 0.0f, -15.0, 0.0f });

	/*sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ScoreBackground.png"), { 0.0f, 0.0f }));
	sprite_->SetSize({ 128.0f, 128.0f });
	sprite_->SetTextureRect({ 0.0f, 576.0f}, {64.0f, 64.0f});*/

	bottonSprite_ = std::make_unique<Sprite>();
	bottonSprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ABotton.dds"), { 610.0f, 520.0f }));
	bottonPushSprite_ = std::make_unique<Sprite>();
	bottonPushSprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ABottonPush.png"), { 610.0f, 520.0f }));
	time_ = 0;

	model_ = std::make_unique<Model>();
	model_.reset(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Signboard"));
	//model_ = std::make_unique<Model>();
	//ModelManager::GetInstance()->CreateModel(obj, "terrain");
	Model::DirectionalLight l = { .color = {1.0f, 1.0f, 1.0f, 1.0f}, .direction = {1.0f, -1.0f, 0.0f}, .intensity= 1.0f };
	model_->SetCamera(camera_/*camera_.get()*/);
	model_->SetEnableLighting(true);
	model_->SetDirectionalLight(l);
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 25.0f, -7.0f, 6.0f };

	/*obj_ = std::make_unique<Object3D>();
	obj_->Initialize(ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), Collider::kAABB);
	obj_->SetCamera(camera_);
	obj_->SetMass(0.0f);
	obj_->SetCollisionAttribute(kCollisionAttributeSpike);
	world_->Add(obj_.get());*/

	player_ = std::make_unique<Player>();
	player_->Initialize(camera_/*camera_.get()*/, world_.get());
	player_->SetDirectionalLight(l);
	player_->SetMass(0.0f);
	camera_->SetTarget(nullptr);  


	//ObjectManager::GetInstance()->Load("title1", camera_/*camera_.get()*/, world_.get());
	//ObjectManager::GetInstance()->SetDirectionalLight("title1", l);
	objectManager_ = std::make_unique<ObjectManager>();
	objectManager_->Initialize();
	objectManager_->Load("title1", camera_, world_.get());

	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kHSVFilter, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kGrayscale, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kVignetting, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kOutline, true);
	//RenderTexture::GetInstance()->SelectPostEffect(PostEffects::kRadialBlur, true);
	/*RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kGrayscale)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kVignetting)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kOutline)] = true;
	RenderTexture::GetInstance()->postEffectFlag_[static_cast<uint32_t>(PostEffects::kRadialBlur)] = true;*/

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

	/*floor_ = std::make_unique<Object3D>();
	floor_->Initialize(ModelManager::GetInstance()->CreateModel(obj, "Wood"), world_.get(), Collider::kAABB);
	floor_->SetTranslation({ 0.0f, 10.0f, 0.0f });
	floor_->SetCamera(camera_);
	floor_->SetMass(1.0f);
	floor_->SetCollisionAttribute(kCollisionAttributeFloor);
	floor_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	world_->Add(floor_.get()); 

	anchor_ = std::make_unique<Object3D>();
	anchor_->Initialize(world_.get(), Collider::kAABB);
	anchor_->SetTranslation({ 0.0f, 5.0f, 0.0f });
	anchor_->SetCamera(camera_);
	anchor_->SetMass(0.0f);
	anchor_->SetCollisionAttribute(kCollisionAttributeSpike);
	anchor_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	world_->Add(anchor_.get());

	sp_ = std::make_unique<PrimitiveDrawer>();
	sp_.reset(PrimitiveDrawer::Create());
    sp_->SetCamera(CameraManager::GetInstance()->GetCamera());

	joint_ = std::make_unique<SpringJoint>();
	joint_->CreateSpringJoint(floor_.get(), anchor_.get());
	joint_->SetDamping(1, 1.0f);
	joint_->SetStiffness(1, 20.0f);
	joint_->EnableSpring(1, false);
	joint_->SetEquilibriumPoint(1, 10.0f);
	world_->AddJoint(joint_.get());*/

	// obj1,2
	obj1_ = std::make_unique<Object3D>();
	obj1_->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Coin"), world_.get(), Collider::Shape::kAABB);
	obj1_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	obj1_->SetScale({ 1.0f, 1.0f, 1.0f });
	obj1_->SetCamera(camera_);
	obj1_->SetCollisionAttribute(kCollisionAttributeTrampoline);
	obj1_->SetPosition(Vector3{ 0.0f, 20.0f, 0.0f });
	obj1_->SetFirictionCombine(Body::FrictionCombine::kAverage);
	obj1_->SetMiu(1.0f);
	obj1_->SetAngularDrag(0.00005f);

	//obj2_ = std::make_unique<Object3D>();
	//obj2_->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Wood"), world_.get(), Collider::Shape::kOBB);
	//obj2_->SetCamera(camera_);
	//obj2_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	////obj2_->SetScale({200.0f , 3.0f, 50.0f });
	//obj2_->SetScale({ 4.0, 1.0f, 4.0f });
	//obj2_->SetPosition(Vector3{ 0.0f, 20.0, 0.0f });
	//obj2_->SetCollisionAttribute(kCollisionAttributePlayer);
	//obj2_->SetFirictionCombine(Body::FrictionCombine::kMaximum);
	//obj2_->SetMiu(1.0f);

	//world_->Add(obj1_.get());
	//world_->Add(obj2_.get());

	// springbrige
	//for (uint32_t index = 0; index < 5; index++) {
	//	objs_[index] = std::make_unique<Object3D>();
	//	objs_[index]->SetPosition(Vector3{ -15.0f + 5.0f * index , 10.0f, 0.0f });
	//	objs_[index]->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Cube"), world_.get(), Collider::Shape::kOBB);
	//	objs_[index]->SetCamera(camera_);
	//	objs_[index]->SetScale({ 1.0f, 1.0f, 1.0f });
	//	objs_[index]->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	//	objs_[index]->SetCollisionAttribute(kCollisionAttributeFloor);
	//	if (index > 0 && index < 4) {
	//		if (index % 2 == 0) {
	//			objs_[index]->SetMass(2.0f);
	//		}
	//		else {
	//			objs_[index]->SetMass(2.0f);
	//		}
	//	}
	//	world_->Add(objs_[index].get());
	//}

	//for (uint32_t index = 0; index < 4; index++) {
	//	springJoints_[index] = std::make_unique<SpringJoint>();
	//	springJoints_[index]->CreateSpringJoint(objs_[index].get(), objs_[index + 1].get());
	//	springJoints_[index]->EnableSpring(0, true);
	//	springJoints_[index]->EnableSpring(1, true);
	//	springJoints_[index]->SetEquilibriumPoint(0, 0.0f);
	//	springJoints_[index]->SetEquilibriumPoint(1, 0.0f);
	//	springJoints_[index]->SetStiffness(0, stiffness_);
	//	springJoints_[index]->SetStiffness(1, stiffness_);
	//	springJoints_[index]->SetDamping(0, dampar_);
	//	springJoints_[index]->SetDamping(1, dampar_);
	//	world_->AddJoint(springJoints_[index].get());

	//	lines_[index] = std::make_unique<PrimitiveDrawer>();
	//	lines_[index].reset(PrimitiveDrawer::Create());
	//	lines_[index]->SetCamera(camera_);
	//}

	// pulley
	//for (int index = 0; index < 2; index++) {
	//	pulleyObjs_[index] = std::make_unique<Object3D>();
	//	pulleyObjs_[index]->SetPosition({ -5.0f + index * 10.0f, 5.0f, 0.0f });
	//	pulleyObjs_[index]->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Cube"), world_.get(), Collider::Shape::kOBB);
	//	pulleyObjs_[index]->SetCamera(camera_);
	//	pulleyObjs_[index]->SetScale({ 1.0f, 1.0f, 1.0f });
	//	pulleyObjs_[index]->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	//	pulleyObjs_[index]->SetCollisionAttribute(kCollisionAttributeFloor);
	//	pulleyObjs_[index]->SetMass(20.0f);
	//	world_->Add(pulleyObjs_[index].get());
	//	groundAnchor_[index] = { -5.0f + index * 10.0f, 10.0f, 0.0f };
	//}
	//for (uint32_t index = 0; index < 3; index++) {
	//	pulleyLines_[index] = std::make_unique<PrimitiveDrawer>();
	//	pulleyLines_[index].reset(PrimitiveDrawer::Create());
	//	pulleyLines_[index]->SetCamera(camera_);
	//}

	//ratio_ = 1.0f;
	//pulleyJoint_ = std::make_unique<PulleyJoint>();
	//pulleyJoint_->CreatePulleyJoint(pulleyObjs_[0].get(), pulleyObjs_[1].get(), groundAnchor_[0], groundAnchor_[1], pulleyObjs_[0]->GetTranslation(), pulleyObjs_[1]->GetTranslation(), ratio_);
	//world_->AddJoint(pulleyJoint_.get());

}

void TitleScene::Update()
{
	prePad_ = pad_;

	/*if (Input::GetInstance()->TriggerKey(DIK_I)) {
		joint_->EnableSpring(1, true);
	}*/

	/*Vector3 rotate{ 0.4f, 1.43f, -0.8f };
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZ = Multiply(rotateX, Multiply(rotateY, rotateZ));*/

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
		//player_->Update();
		if (player_->GetWorldTransform()->GetMatWorldTranslation().x >= 35.0f && camera_->GetTarget() == nullptr) {
			camera_->SetTarget(player_->GetWorldTransform());
			CommonData::GetInstance()->scene_ = Scene::kSelect;

			//SceneManager::GetInstance()->ChangeScene("SELECT");
		}
		else if (player_->GetWorldTransform()->GetMatWorldTranslation().x < 35.0f && camera_->GetTarget() != nullptr) {
			camera_->SetTarget(nullptr);
			camera_->SetTranslate({ 0, 10.0f, -50.0f });
			CommonData::GetInstance()->scene_ = Scene::kTitle;

		}
		/*else if (player_->GetWorldTransform()->GetMatWorldTranslation().x < -30.0f) {
			player_->GetWorldTransform()->translation_.x = -30.0f;
			Vector3 velocity = player_->GetVelocity();
			velocity.x *= -0.2f;
			player_->SetVelocity(velocity);
		}*/

		if (Input::GetInstance()->IsControllerConnected()) {
			if (Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					isStart_ = true;
				}
			}
		}

		/*if (Input::GetInstance()->IsControllerConnected()) {
			if (Input::GetInstance()->GetJoystickState(0, pad_)) {
				if ((pad_.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(prePad_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					SceneManager::GetInstance()->ChangeScene("SELECT");
					RenderTexture::GetInstance()->ClearPostEffect();
				}


			}
		}*/

		/*ImGui::Begin("Player");
		Vector3 pos = obj_->GetWorldTransform()->translation_;
		ImGui::DragFloat3("pos", &pos.x);
		obj_->SetPosition(pos);
		ImGui::End();

		obj_->AddForce(obj_->Spring({ 0.0f, 0.0f, 0.0f }, obj_->GetMatWorldTranslation(), 0.0f, 1.0f, 0.1f), 0);*/

		//ObjectManager::GetInstance()->Update("title1");
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
	Vector3 pos;
	ImGui::Begin("obj1");
	pos = obj1_->GetMatWorldTranslation();
	//Vector3 rotate = Multiply(RadToDeg(), obj1_->GetWorldTransform()->rotation_);
	Vector3 rotate = obj1_->GetWorldTransform()->rotation_;
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("rotate", &rotate.x);
	obj1_->SetPosition(pos);
	//obj1_->SetRotation(Multiply(DegToRad(), rotate));
	obj1_->SetRotation(rotate);
	if (ImGui::Button("Gravity")) {
		obj1_->SetMass(0.1f);
	}
	if (ImGui::Button("Reset")) {
		obj1_->SetMass(0.0f);
		obj1_->SetPosition(Vector3{ 0.0f, 20.0f, 0.0f });
		obj1_->SetRotation(Vector3{ 0.0f, 0.0f, 0.0f });
		obj1_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		obj1_->SetAngularVelocity({ 0.0f, 0.0f, 0.0f });
	}
	Vector3 velocity = obj1_->GetVelocity();
	ImGui::DragFloat3("velocity", &velocity.x);
	Vector3 anglur = obj1_->GetAngularVelocity();
	ImGui::DragFloat3("angularvelocity", &anglur.x);
	//ImGui::DragFloat3("torque", &torupu_.x);

	/*if (ImGui::Button("AddTorque")) {
		obj1_->AddTorque(torupu_, 1);
	}*/
	ImGui::End();

	//ImGui::Begin("obj2");
	//pos = obj2_->GetMatWorldTranslation();
	//rotate = Multiply(RadToDeg(), obj2_->GetWorldTransform()->rotation_);
	//ImGui::DragFloat3("pos", &pos.x);
	//ImGui::DragFloat3("rotate", &rotate.x);
	//obj2_->SetPosition(pos);
	//obj2_->SetRotation(Multiply(DegToRad(), rotate));
	//if (ImGui::Button("Gravity")) {
	//	obj2_->SetMass(10.0f);
	//}
	//if (ImGui::Button("Reset")) {
	//	obj2_->SetMass(0.0f);
	//	obj2_->SetPosition(Vector3{ 0.0f, 7.0f, 0.0f });
	//	obj2_->SetRotation(Vector3{ 0.0f, 0.0f, 0.0f });
	//	obj2_->SetVelocity({ 0.0f, 0.0f, 0.0f });
	//	obj2_->SetAngularVelocity({ 0.0f, 0.0f, 0.0f });
	//}
	///*ImGui::DragFloat3("torque", &torupu_.x);

	//if (ImGui::Button("AddTorque")) {
	//	obj2_->AddTorque(torupu_, 1);
	//}*/
	//velocity = obj2_->GetVelocity();
	//ImGui::DragFloat3("velocity", &velocity.x);
	//ImGui::End();

	/*------------------------------------------------------------------------------*/
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
	//ObjectManager::GetInstance()->Draw("title1");
	objectManager_->Draw(/*"title1"*/);

	/*floor_->Draw();
	anchor_->Draw();
	sp_->Draw(anchor_->GetTranslation(), floor_->GetTranslation());*/

	player_->Draw();
	model_->Draw(worldTransform_);

	for (uint32_t index = 0; index < 3; index++) {
		if (isActiveTV_[index]) {
			models_[index]->Draw(TVworldTransform_[index], textureTV_[index]);
		}
	}

	//obj1,2
	obj1_->Draw();
	/*obj1_->Draw();
	obj2_->Draw();*/

	// springbrige
	/*for (uint32_t index = 0; index < 4; index++) {
		lines_[index]->Draw(objs_[index]->GetMatWorldTranslation(), objs_[index + 1]->GetMatWorldTranslation(), {0.0f, 0.0f, 0.0f, 1.0f});
	}

	for (uint32_t index = 0; index < 5; index++) {
		objs_[index]->Draw();
	}*/

	// pulley
	/*for (uint32_t index = 0; index < 2; index++) {
		pulleyLines_[index]->Draw(pulleyObjs_[index]->GetMatWorldTranslation(), groundAnchor_[index], { 0.0f, 0.0f, 0.0f, 1.0f });
	}
	pulleyLines_[2]->Draw(groundAnchor_[0], groundAnchor_[1], { 0.0f, 0.0f, 0.0f, 1.0f });
	for (uint32_t index = 0; index < 2; index++) {
		pulleyLines_[index]->Draw(pulleyObjs_[index]->GetMatWorldTranslation(), groundAnchor_[index], { 0.0f, 0.0f, 0.0f, 1.0f });
		pulleyObjs_[index]->Draw();
	}*/

	//cloth_->Draw();
	//obj_->Draw();
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

