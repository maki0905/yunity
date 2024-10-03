#include "TitleScene.h"

#include "SceneManager.h"
#include "ModelManager.h"
#include "RenderTexture.h"
#include "ObjectManager.h"
#include "ImGuiManager.h"
#include "CommonData.h"

void TitleScene::Initialize()
{
	//camera_.reset(CameraManager::GetInstance()->GetCamera());
	camera_ = CameraManager::GetInstance()->GetCamera();
	world_ = std::make_unique<World>();
	world_->Initialize({ 0.0f, /*0.0f*/-15.0f, 0.0f });

	sprite_ = std::make_unique<Sprite>();
	sprite_.reset(Sprite::Create(TextureManager::GetInstance()->Load("ScoreBackground.png"), { 0.0f, 0.0f }));
	sprite_->SetSize({ 128.0f, 128.0f });
	sprite_->SetTextureRect({ 0.0f, 576.0f}, {64.0f, 64.0f});

	model_ = std::make_unique<Model>();
	model_.reset(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Signboard"));
	//model_ = std::make_unique<Model>();
	//ModelManager::GetInstance()->CreateModel(obj, "terrain");
	Model::DirectionalLight l = { .color = {1.0f, 1.0f, 1.0f, 1.0f}, .direction = {-1.0f, -1.0f, 0.0f}, .intensity= 1.0f };
	model_->SetCamera(camera_/*camera_.get()*/);
	model_->SetEnableLighting(true);
	model_->SetDirectionalLight(l);
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 30.0f, -7.0f, 6.0f };

	/*obj_ = std::make_unique<Object3D>();
	obj_->Initialize(ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), Collider::kAABB);
	obj_->SetCamera(camera_);
	obj_->SetMass(0.0f);
	obj_->SetCollisionAttribute(kCollisionAttributeSpike);
	world_->Add(obj_.get());*/

	//player_ = std::make_unique<Player>();
	//player_->Initialize(camera_/*camera_.get()*/, world_.get());
	//player_->SetDirectionalLight(l);
	//camera_->SetTarget(nullptr);  

	ObjectManager::GetInstance()->Load("title", camera_/*camera_.get()*/, world_.get());
	ObjectManager::GetInstance()->SetDirectionalLight("title", l);
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

	obj1_ = std::make_unique<Object3D>();
	obj1_->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Cube"),world_.get(), Collider::Shape::kOBB);
	obj1_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	obj1_->SetScale({4.5f, 4.5f, 4.5f });
	obj1_->SetCamera(camera_);
	obj1_->SetCollisionAttribute(kCollisionAttributePlayer);
	obj1_->SetPosition(Vector3{ 0.0f, 20.0f, 0.0f });
	obj1_->SetFirictionCombine(Body::FrictionCombine::kMaximum);
	obj1_->SetMiu(1.0f);
	obj1_->SetAngularDrag(0.00005f);
	obj1_->SetInertiaMoment(0.01f);
	
	obj2_ = std::make_unique<Object3D>();
	obj2_->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Cube"),world_.get(), Collider::Shape::kOBB);
	obj2_->SetCamera(camera_);
	obj2_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	//obj2_->SetScale({200.0f , 3.0f, 50.0f });
	obj2_->SetScale({ 4.0, 1.0f, 4.0f });
	obj2_->SetPosition(Vector3{ 0.0f, 5.0, 0.0f });
	obj2_->SetCollisionAttribute(kCollisionAttributeFloor);
	obj2_->SetFirictionCombine(Body::FrictionCombine::kMaximum);
	obj2_->SetMiu(1.0f);
	
	obj3_ = std::make_unique<Object3D>();
	obj3_->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Cube"), world_.get(), Collider::Shape::kOBB);
	obj3_->SetCamera(camera_);
	obj3_->SetScale({ 100.0f, 1.0f, 50.0f });
	obj3_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	obj3_->SetPosition(Vector3{ 0.0f, 0.0f, 0.0f });
	obj3_->SetCollisionAttribute(kCollisionAttributeTrampoline);

	obj4_ = std::make_unique<Object3D>();
	obj4_->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Cube"), world_.get(), Collider::Shape::kOBB);
	obj4_->SetCamera(camera_);
	obj4_->SetScale({ 4.0f, 1.0f, 4.0f });
	obj4_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	obj4_->SetPosition(Vector3{ 0.0f, 11.0f, 0.0f });
	obj4_->SetCollisionAttribute(kCollisionAttributeGoal);
	obj4_->SetFirictionCombine(Body::FrictionCombine::kMaximum);
	obj4_->SetMiu(1.0f);

	obj5_ = std::make_unique<Object3D>();
	obj5_->Initialize(ModelManager::GetInstance()->CreateModel(obj,/* ""*/"Cube"), world_.get(), Collider::Shape::kOBB);
	obj5_->SetCamera(camera_);
	obj5_->SetScale({ 4.0f, 1.0f, 4.0f });
	obj5_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	obj5_->SetPosition(Vector3{ 0.0f, 14.0f, 0.0f });
	obj5_->SetCollisionAttribute(kCollisionAttributeMoveFloorLeft);
	obj5_->SetFirictionCombine(Body::FrictionCombine::kMaximum);
	obj5_->SetMiu(1.0f);

	world_->Add(obj1_.get());
	world_->Add(obj2_.get());
	world_->Add(obj3_.get());
	world_->Add(obj4_.get());
	world_->Add(obj5_.get());

}

void TitleScene::Update()
{
	prePad_ = pad_;

	/*Vector3 rotate{ 0.4f, 1.43f, -0.8f };
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZ = Multiply(rotateX, Multiply(rotateY, rotateZ));*/

	/*player_->Update();
	if (player_->GetWorldTransform()->GetMatWorldTranslation().x >= 35.0f) {
		SceneManager::GetInstance()->ChangeScene("SELECT");
	}
	else if (player_->GetWorldTransform()->GetMatWorldTranslation().x < -30.0f) {
		player_->GetWorldTransform()->translation_.x = -30.0f;
		Vector3 velocity = player_->GetVelocity();
		velocity.x *= -0.2f;
		player_->SetVelocity(velocity);
	}*/
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

	if (flag_) {
		world_->Solve();
	}

	i += k;
	if (i > 10.0f || i < -10.0f) {
		k *= -1.0f;
	}
	worldTransform_.rotation_.z = i * DegToRad();
	worldTransform_.UpdateMatrix();

	if (Input::GetInstance()->PushKey(DIK_A)) {
		//obj2_->AddForce({ -1.0f, 0.0f, 0.0f }, 1);
		obj1_->AddForce({ -1.15f, 0.0f, 0.0f }, 1);
		//obj4_->AddForce({ -1.05f, 0.0f, 0.0f }, 1);
		//obj5_->AddForce({ -1.1f, 0.0f, 0.0f }, 1);
	}
	if (Input::GetInstance()->PushKey(DIK_D)) {
		//obj2_->AddForce({ 1.0f, 0.0f, 0.0f }, 1);
		obj1_->AddForce({ 1.15f, 0.0f, 0.0f }, 1);
		//obj4_->AddForce({ 1.05f, 0.0f, 0.0f }, 1);
		//obj5_->AddForce({ 1.1f, 0.0f, 0.0f }, 1);
	}

#ifdef _DEBUG
	ImGui::Begin("a");
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x);
	/*if (ImGui::Button("Off")) {
		model_->SetEnableLighting(false);
		player_->SetEnableLighting(false);
		ObjectManager::GetInstance()->SetEnableLighting("title",false);
	}
	if (ImGui::Button("On")) {
		model_->SetEnableLighting(true);
		player_->SetEnableLighting(true);
		ObjectManager::GetInstance()->SetEnableLighting("title", true);
	}*/

	ImGui::End();

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
		obj1_->SetMass(1.0f);
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
	ImGui::DragFloat3("torque", &torupu_.x);

	if (ImGui::Button("AddTorque")) {
		obj1_->AddTorque(torupu_, 1);
	}
	ImGui::End();

	ImGui::Begin("obj2");
	pos = obj2_->GetMatWorldTranslation();
	rotate = Multiply(RadToDeg(), obj2_->GetWorldTransform()->rotation_);
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("rotate", &rotate.x);
	obj2_->SetPosition(pos);
	obj2_->SetRotation(Multiply(DegToRad(), rotate));
	if (ImGui::Button("Gravity")) {
		obj2_->SetMass(1.0f);
	}
	if (ImGui::Button("Reset")) {
		obj2_->SetMass(0.0f);
		obj2_->SetPosition(Vector3{ 0.0f, 7.0f, 0.0f });
		obj2_->SetRotation(Vector3{ 0.0f, 0.0f, 0.0f });
		obj2_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		obj2_->SetAngularVelocity({ 0.0f, 0.0f, 0.0f });
	}
	ImGui::DragFloat3("torque", &torupu_.x);

	if (ImGui::Button("AddTorque")) {
		obj2_->AddTorque(torupu_, 1);
	}
	velocity = obj2_->GetVelocity();
	ImGui::DragFloat3("velocity", &velocity.x);
	ImGui::End();

	ImGui::Begin("obj4");
	pos = obj4_->GetMatWorldTranslation();
	rotate = Multiply(RadToDeg(), obj4_->GetWorldTransform()->rotation_);
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("rotate", &rotate.x);
	obj4_->SetPosition(pos);
	obj4_->SetRotation(Multiply(DegToRad(), rotate));
	if (ImGui::Button("Gravity")) {
		obj4_->SetMass(1.0f);
	}
	if (ImGui::Button("Reset")) {
		obj4_->SetMass(0.0f);
		obj4_->SetPosition(Vector3{ 0.0f, 10.0f, 0.0f });
		obj4_->SetRotation(Vector3{ 0.0f, 0.0f, 0.0f });
		obj4_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		obj4_->SetAngularVelocity({ 0.0f, 0.0f, 0.0f });
	}
	ImGui::DragFloat3("torque", &torupu_.x);

	if (ImGui::Button("AddTorque")) {
		obj4_->AddTorque(torupu_, 1);
	}
	velocity = obj4_->GetVelocity();
	ImGui::DragFloat3("velocity", &velocity.x);
	ImGui::End();

	ImGui::Begin("obj5");
	pos = obj5_->GetMatWorldTranslation();
	rotate = Multiply(RadToDeg(), obj5_->GetWorldTransform()->rotation_);
	ImGui::DragFloat3("pos", &pos.x);
	ImGui::DragFloat3("rotate", &rotate.x);
	obj5_->SetPosition(pos);
	obj5_->SetRotation(Multiply(DegToRad(), rotate));
	if (ImGui::Button("Gravity")) {
		obj5_->SetMass(1.0f);
	}
	if (ImGui::Button("Reset")) {
		obj5_->SetMass(0.0f);
		obj5_->SetPosition(Vector3{ 0.0f, 15.0f, 0.0f });
		obj5_->SetRotation(Vector3{ 0.0f, 0.0f, 0.0f });
		obj5_->SetVelocity({ 0.0f, 0.0f, 0.0f });
		obj5_->SetAngularVelocity({ 0.0f, 0.0f, 0.0f });
	}
	ImGui::DragFloat3("torque", &torupu_.x);

	if (ImGui::Button("AddTorque")) {
		obj5_->AddTorque(torupu_, 1);
	}
	velocity = obj5_->GetVelocity();
	ImGui::DragFloat3("velocity", &velocity.x);
	ImGui::End();


	ImGui::Begin("Debug");
	if (ImGui::Button("OnOff")) {
		flag_ ^= true;
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
	//ObjectManager::GetInstance()->Draw("title");
	//player_->Draw();
	model_->Draw(worldTransform_);

	obj1_->Draw();
	obj2_->Draw();
	obj3_->Draw();
	obj4_->Draw();
	obj5_->Draw();
	//obj_->Draw();
}

void TitleScene::DrawFront()
{
}

