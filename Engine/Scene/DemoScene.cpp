#include "DemoScene.h"

#include "ImGuiManager.h"
#include "ModelManager.h"
#include "LevelEditor.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "Object3D.h"
#include "Collider.h"

void DemoScene::Initialize()
{
	camera_ = CameraManager::GetInstance()->GetCamera();
	debugCamera_ = std::make_unique<yunity::DebugCamera>();

	world_ = std::make_unique<yunity::World>();
	world_->Initialize({ 0.0f, -9.0f, 0.0f });

	object_ = std::make_unique<yunity::Object3D>();
	object_->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Grasslands"), world_.get(), yunity::Collider::Shape::kAABB);
	object_->SetCamera(camera_);
	object_->SetScale({ 40.0f, 1.0f, 10.0f });
	object_->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
	world_->Add(object_.get());

	force_ = { 0.0f, 0.0f, 0.0f };
	velocity_ = { 0.0f, 0.0f, 0.0f };

	demoNumber_ = DemoScene::kDemo1;
	preDemoNumber_ = DemoScene::kMax;

	onoff_ = false;

}

void DemoScene::Update()
{

	if (demoNumber_ != preDemoNumber_) {
		
		for (auto& object : objects_) {
			world_->Take(object.get());
		}
		for (auto& joint : springJoints_) {
			world_->TakeJoint(joint.get());
		}
		objects_.clear();
		springJoints_.clear();
		lines_.clear();

		switch (demoNumber_)
		{
		case DemoScene::kDemo1:
			Demo1Initialize();
			break;
		case DemoScene::kDemo2:
			Demo2Initialize();
			break;
		case DemoScene::kDemo3:
			Demo3Initialize();
			break;
		case DemoScene::kDemo4:
			Demo4Initialize();
			break;
		case DemoScene::kDemo5:
			break;
		case DemoScene::kDemo6:
			break;
		case DemoScene::kDemo7:
			break;
		case DemoScene::kDemo8:
			break;
		case DemoScene::kDemo9:
			break;
		case DemoScene::kDemo10:
			break;
		default:
			break;
		}
	}

	preDemoNumber_ = demoNumber_;

	switch (demoNumber_)
	{
	case DemoScene::kDemo1:
		Demo1Update();
		break;
	case DemoScene::kDemo2:
		Demo2Update();
		break;
	case DemoScene::kDemo3:
		Demo3Update();
		break;
	case DemoScene::kDemo4:
		Demo4Update();
		break;
	case DemoScene::kDemo5:
		break;
	case DemoScene::kDemo6:
		break;
	case DemoScene::kDemo7:
		break;
	case DemoScene::kDemo8:
		break;
	case DemoScene::kDemo9:
		break;
	case DemoScene::kDemo10:
		break;
	default:
		break;
	}



	ImGui::Begin("DemoScene");
	if (ImGui::Button("onoff")) {
		onoff_ ^= true;
	}
	if (ImGui::Button("Mass")) {
		for (int i = 0; i < 2; i++) {
			objects_[i]->SetMass(1.0f);
		}
	}

	ImGui::DragFloat3("Velocity", &velocity_.x);

	if (ImGui::Button("ResetVelocity")) {
		for (int i = 0; i < 2; i++) {

			objects_[i]->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}
	}

	std::string demo = "Demo" + std::to_string(int(demoNumber_) + 1);
	ImGui::Text(demo.c_str());
	for (int i = DemoNumber::kDemo1; i < DemoNumber::kMax; i++) {
		std::string name = "Demo" + std::to_string(i + 1);
		if (ImGui::Button(name.c_str())) {
			demoNumber_ = static_cast<DemoNumber>(i);
		}
	}


	ImGui::End();


	if (onoff_) {
		world_->Solve();
	}

#ifdef _DEBUG
	if (yunity::Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
	}

	if (isDebug_) {
		debugCamera_->Update(camera_);
	}
	camera_->Update();


#endif

}

void DemoScene::DrawBack()
{
}

void DemoScene::Draw3D()
{
	object_->Draw();

	for (auto& line : lines_) {
		line->Draw({0.0f, 0.0f, 0.0f, 1.0f});
	}
	for (auto& object : objects_) {
		object->Draw();
	}
}

void DemoScene::DrawFront()
{
}

void DemoScene::Demo1Initialize()
{
	for (int i = 0; i < 2; i++) {
		std::unique_ptr<yunity::Object3D> object = std::make_unique<yunity::Object3D>();
		object->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), yunity::Collider::Shape::kAABB);
		object->SetCamera(camera_);
		object->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		object->SetMass(0.0f);
		object->SetDrag(0.0f);
		object->SetAngularDrag(0.0f);
		object->SetInertiaMoment(0.0f);
		object->SetPosition({ -3.0f + 6.0f * float(i), 5.0f, 0.0f });
		object->GetWorldTransform()->UpdateMatrix();
		object->SetBounciness(0.0f);
		object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		world_->Add(object.get());
		objects_.push_back(std::move(object));
	}

	camera_->SetTranslate({ 0.0f, 10.0f, -50.0f });
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });

}

void DemoScene::Demo2Initialize()
{
	for (int i = 0; i < 2; i++) {
		std::unique_ptr<yunity::Object3D> object = std::make_unique<yunity::Object3D>();
		object->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), yunity::Collider::Shape::kAABB);
		object->SetCamera(camera_);
		object->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		object->SetMass(0.0f);
		object->SetDrag(0.0f);
		object->SetAngularDrag(0.0f);
		object->SetInertiaMoment(0.0f);
		object->SetPosition({ 0.0f, 2.0f, -3.0f + 6.0f * float(i) });
		object->GetWorldTransform()->UpdateMatrix();
		object->SetBounciness(0.0f);
		object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		world_->Add(object.get());
		objects_.push_back(std::move(object));
	}

	camera_->SetTranslate({ 0.0f, 50.0f, 0.0f });
	camera_->SetRotate({ 90.0f * DegToRad(), 90.0f * DegToRad() , 0.0f});
}

void DemoScene::Demo3Initialize()
{


	for (int i = 0; i < 6; i++) {
		std::unique_ptr<yunity::Object3D> object = std::make_unique<yunity::Object3D>();
		object->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), yunity::Collider::Shape::kAABB);
		object->SetCamera(camera_);
		object->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		object->SetMass(0.0f);
		object->SetDrag(0.0f);
		object->SetAngularDrag(0.0f);
		object->SetInertiaMoment(0.0f);
		object->SetPosition({ -10.0f + 5.0f * float(i), 8.0f, 0.0f });
		object->GetWorldTransform()->UpdateMatrix();
		object->SetBounciness(0.0f);
		object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		world_->Add(object.get());
		objects_.push_back(std::move(object));
	}
	for (int i = 0; i < 4; i++) {
		std::unique_ptr<yunity::SpringJoint> springJoint = std::make_unique<yunity::SpringJoint>();

		springJoint->CreateSpringJoint(objects_[i].get(), objects_[i + 1].get());
		// 有効軸設定
		springJoint->EnableSpring(0, false);
		springJoint->EnableSpring(1, false);
		springJoint->EnableSpring(2, false);
		// 各軸の自然長
		springJoint->SetEquilibriumPoint(0, 0.0f);
		springJoint->SetEquilibriumPoint(1, 0.0f);
		springJoint->SetEquilibriumPoint(2, 0.0f);
		// 各軸の剛性
		springJoint->SetStiffness(0, 0.0f);
		springJoint->SetStiffness(1, 0.0f);
		springJoint->SetStiffness(2, 0.0f);
		// 各軸の減衰
		springJoint->SetDamping(0, 0.0f);
		springJoint->SetDamping(1, 0.0f);
		springJoint->SetDamping(2, 0.0f);
		// ワールドに追加
		world_->AddJoint(springJoint.get());

		springJoints_.push_back(std::move(springJoint));

		std::unique_ptr<yunity::PrimitiveDrawer> line = std::make_unique<yunity::PrimitiveDrawer>();
		line.reset(yunity::PrimitiveDrawer::Create());
		line->SetCamera(camera_);
		line->SetLinePoints(objects_[i]->GetWorldTransform(), objects_[i + 1]->GetWorldTransform());
		lines_.push_back(std::move(line));

	}

	camera_->SetTranslate({ 0.0f, 10.0f, -50.0f });
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });

}

void DemoScene::Demo4Initialize()
{
	for (int i = 0; i < 3; i++) {
		std::unique_ptr<yunity::Object3D> object = std::make_unique<yunity::Object3D>();
		object->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), yunity::Collider::Shape::kAABB);
		object->SetCamera(camera_);
		object->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		object->SetMass(0.0f);
		object->SetDrag(0.0f);
		object->SetAngularDrag(0.0f);
		object->SetInertiaMoment(0.0f);
		object->SetPosition({ -5.0f + 10.0f * float(i), 8.0f, 0.0f });
		object->GetWorldTransform()->UpdateMatrix();
		object->SetBounciness(0.0f);
		object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		world_->Add(object.get());
		objects_.push_back(std::move(object));
	}

	std::unique_ptr<yunity::SpringJoint> springJoint = std::make_unique<yunity::SpringJoint>();

	springJoint->CreateSpringJoint(objects_[0].get(), objects_[1].get());
	// 有効軸設定
	springJoint->EnableSpring(0, false);
	springJoint->EnableSpring(1, false);
	springJoint->EnableSpring(2, false);
	// 各軸の自然長
	springJoint->SetEquilibriumPoint(0, 0.0f);
	springJoint->SetEquilibriumPoint(1, 0.0f);
	springJoint->SetEquilibriumPoint(2, 0.0f);
	// 各軸の剛性
	springJoint->SetStiffness(0, 0.0f);
	springJoint->SetStiffness(1, 0.0f);
	springJoint->SetStiffness(2, 0.0f);
	// 各軸の減衰
	springJoint->SetDamping(0, 0.0f);
	springJoint->SetDamping(1, 0.0f);
	springJoint->SetDamping(2, 0.0f);
	// ワールドに追加
	world_->AddJoint(springJoint.get());

	springJoints_.push_back(std::move(springJoint));

	std::unique_ptr<yunity::PrimitiveDrawer> line = std::make_unique<yunity::PrimitiveDrawer>();
	line.reset(yunity::PrimitiveDrawer::Create());
	line->SetCamera(camera_);
	line->SetLinePoints(objects_[0]->GetWorldTransform(), objects_[1]->GetWorldTransform());
	lines_.push_back(std::move(line));

	camera_->SetTranslate({ 0.0f, 10.0f, -50.0f });
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
}

void DemoScene::Demo5Initialize()
{
	for (int i = 0; i < 3; i++) {
		std::unique_ptr<yunity::Object3D> object = std::make_unique<yunity::Object3D>();
		object->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Cube"), world_.get(), yunity::Collider::Shape::kAABB);
		object->SetCamera(camera_);
		object->SetHitBoxSize({ 2.0f, 2.0f, 2.0f });
		object->SetMass(0.0f);
		object->SetDrag(0.0f);
		object->SetAngularDrag(0.0f);
		object->SetInertiaMoment(0.0f);
		object->SetPosition({ -10.0f + 5.0f * float(i), 8.0f, 0.0f });
		object->GetWorldTransform()->UpdateMatrix();
		object->SetBounciness(0.0f);
		object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		world_->Add(object.get());
		objects_.push_back(std::move(object));
	}

	std::unique_ptr<yunity::SpringJoint> springJoint = std::make_unique<yunity::SpringJoint>();

	springJoint->CreateSpringJoint(objects_[0].get(), objects_[1].get());
	// 有効軸設定
	springJoint->EnableSpring(0, false);
	springJoint->EnableSpring(1, false);
	springJoint->EnableSpring(2, false);
	// 各軸の自然長
	springJoint->SetEquilibriumPoint(0, 0.0f);
	springJoint->SetEquilibriumPoint(1, 0.0f);
	springJoint->SetEquilibriumPoint(2, 0.0f);
	// 各軸の剛性
	springJoint->SetStiffness(0, 0.0f);
	springJoint->SetStiffness(1, 0.0f);
	springJoint->SetStiffness(2, 0.0f);
	// 各軸の減衰
	springJoint->SetDamping(0, 0.0f);
	springJoint->SetDamping(1, 0.0f);
	springJoint->SetDamping(2, 0.0f);
	// ワールドに追加
	world_->AddJoint(springJoint.get());

	springJoints_.push_back(std::move(springJoint));

	std::unique_ptr<yunity::PrimitiveDrawer> line = std::make_unique<yunity::PrimitiveDrawer>();
	line.reset(yunity::PrimitiveDrawer::Create());
	line->SetCamera(camera_);
	line->SetLinePoints(objects_[0]->GetWorldTransform(), objects_[1]->GetWorldTransform());
	lines_.push_back(std::move(line));

	camera_->SetTranslate({ 0.0f, 10.0f, -50.0f });
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
}

void DemoScene::Demo1Update()
{

	for (int i = 0; auto & object : objects_) {
		std::string name = "Obj" + std::to_string(i++);
		ImGui::Begin(name.c_str());

		Vector3 position = object->GetMatWorldTranslation();
		ImGui::DragFloat3("position", &position.x);
		object->SetPosition(position);

		float mass = object->GetMass();
		ImGui::DragFloat("Mass", &mass);
		object->SetMass(mass);

		Vector3 velocity = object->GetVelocity();
		ImGui::DragFloat3("Velocity", &velocity.x);
		object->SetVelocity(velocity);


		if (ImGui::Button("SetVelocity")) {
			object->SetVelocity(velocity_);
		}
		if (ImGui::Button("ResetVelocity")) {
			object->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}

		float bounciness_ = object->GetBounciness();
		ImGui::DragFloat("Bounciness", &bounciness_);
		object->SetBounciness(bounciness_);

		if (ImGui::Button("BounceCombine::Average")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kAverage);
		}
		if (ImGui::Button("BounceCombine::Minimum")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMinimum);
		}
		if (ImGui::Button("BounceCombine::Maximum")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		}
		if (ImGui::Button("BounceCombine::Multiply")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMultiply);
		}


		float miu_ = object->GetMiu();
		ImGui::DragFloat("Miu", &miu_);
		object->SetMiu(miu_);

		if (ImGui::Button("FrictionCombine::Average")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kAverage);
		}
		if (ImGui::Button("FrictionCombine::Minimum")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMinimum);
		}
		if (ImGui::Button("FrictionCombine::Maximum")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMaximum);
		}
		if (ImGui::Button("FrictionCombine::Multiply")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMultiply);
		}
		ImGui::End();
	}
}

void DemoScene::Demo2Update()
{
	for (int i = 0; auto & object : objects_) {
		std::string name = "Obj" + std::to_string(i++);
		ImGui::Begin(name.c_str());

		Vector3 position = object->GetMatWorldTranslation();
		ImGui::DragFloat3("position", &position.x);
		object->SetPosition(position);

		float mass = object->GetMass();
		ImGui::DragFloat("Mass", &mass);
		object->SetMass(mass);

		Vector3 velocity = object->GetVelocity();
		ImGui::DragFloat3("Velocity", &velocity.x);
		object->SetVelocity(velocity);


		if (ImGui::Button("SetVelocity")) {
			object->SetVelocity(velocity_);
		}
		if (ImGui::Button("ResetVelocity")) {
			object->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}

		float bounciness_ = object->GetBounciness();
		ImGui::DragFloat("Bounciness", &bounciness_);
		object->SetBounciness(bounciness_);

		if (ImGui::Button("BounceCombine::Average")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kAverage);
		}
		if (ImGui::Button("BounceCombine::Minimum")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMinimum);
		}
		if (ImGui::Button("BounceCombine::Maximum")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		}
		if (ImGui::Button("BounceCombine::Multiply")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMultiply);
		}


		float miu_ = object->GetMiu();
		ImGui::DragFloat("Miu", &miu_);
		object->SetMiu(miu_);

		if (ImGui::Button("FrictionCombine::Average")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kAverage);
		}
		if (ImGui::Button("FrictionCombine::Minimum")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMinimum);
		}
		if (ImGui::Button("FrictionCombine::Maximum")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMaximum);
		}
		if (ImGui::Button("FrictionCombine::Multiply")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMultiply);
		}
		ImGui::End();
	}
}

void DemoScene::Demo3Update()
{
	for (int i = 0; auto & object : objects_) {
		std::string name = "Obj" + std::to_string(i++);
		ImGui::Begin(name.c_str());

		Vector3 position = object->GetMatWorldTranslation();
		ImGui::DragFloat3("position", &position.x);
		object->SetPosition(position);

		float mass = object->GetMass();
		ImGui::DragFloat("Mass", &mass);
		object->SetMass(mass);

		Vector3 velocity = object->GetVelocity();
		ImGui::DragFloat3("Velocity", &velocity.x);
		object->SetVelocity(velocity);


		if (ImGui::Button("SetVelocity")) {
			object->SetVelocity(velocity_);
		}
		if (ImGui::Button("ResetVelocity")) {
			object->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}

		float bounciness_ = object->GetBounciness();
		ImGui::DragFloat("Bounciness", &bounciness_);
		object->SetBounciness(bounciness_);

		if (ImGui::Button("BounceCombine::Average")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kAverage);
		}
		if (ImGui::Button("BounceCombine::Minimum")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMinimum);
		}
		if (ImGui::Button("BounceCombine::Maximum")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		}
		if (ImGui::Button("BounceCombine::Multiply")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMultiply);
		}


		float miu_ = object->GetMiu();
		ImGui::DragFloat("Miu", &miu_);
		object->SetMiu(miu_);

		if (ImGui::Button("FrictionCombine::Average")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kAverage);
		}
		if (ImGui::Button("FrictionCombine::Minimum")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMinimum);
		}
		if (ImGui::Button("FrictionCombine::Maximum")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMaximum);
		}
		if (ImGui::Button("FrictionCombine::Multiply")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMultiply);
		}
		ImGui::End();
	}

	for (int i = 0; auto & springJoint : springJoints_) {
		std::string name = "SpringJoint" + std::to_string(i++);
		ImGui::Begin(name.c_str());

		std::array<bool, 3> enableSpring = { springJoint->GetEnabledSpring(0), springJoint->GetEnabledSpring(1), springJoint->GetEnabledSpring(2) };
		ImGui::Checkbox("EnableSpringX", &enableSpring[0]);
		ImGui::Checkbox("EnableSpringY", &enableSpring[1]);
		ImGui::Checkbox("EnableSpringZ", &enableSpring[2]);

		for (int j = 0; j < 3; j++) {
			springJoint->EnableSpring(j, enableSpring[j]);
		}

		Vector3 equilibriumPoint = springJoint->GetEquilibriumPoint();
		ImGui::DragFloat3("EquilibriumPoint", &equilibriumPoint.x);
		springJoint->SetEquilibriumPoint(0, equilibriumPoint.x);
		springJoint->SetEquilibriumPoint(1, equilibriumPoint.y);
		springJoint->SetEquilibriumPoint(2, equilibriumPoint.z);

		Vector3 stiffness = springJoint->GetStiffness();
		ImGui::DragFloat3("Stiffness", &stiffness.x);
		springJoint->SetStiffness(0, stiffness.x);
		springJoint->SetStiffness(1, stiffness.y);
		springJoint->SetStiffness(2, stiffness.z);

		Vector3 damping = springJoint->GetDamping();
		ImGui::DragFloat3("Damping", &damping.x);
		springJoint->SetDamping(0, damping.x);
		springJoint->SetDamping(1, damping.y);
		springJoint->SetDamping(2, damping.z);
		

		ImGui::End();
	}
}

void DemoScene::Demo4Update()
{
	for (int i = 0; auto & object : objects_) {
		std::string name = "Obj" + std::to_string(i++);
		ImGui::Begin(name.c_str());

		Vector3 position = object->GetMatWorldTranslation();
		ImGui::DragFloat3("position", &position.x);
		object->SetPosition(position);

		float mass = object->GetMass();
		ImGui::DragFloat("Mass", &mass);
		object->SetMass(mass);

		Vector3 velocity = object->GetVelocity();
		ImGui::DragFloat3("Velocity", &velocity.x);
		object->SetVelocity(velocity);


		if (ImGui::Button("SetVelocity")) {
			object->SetVelocity(velocity_);
		}
		if (ImGui::Button("ResetVelocity")) {
			object->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}

		float bounciness_ = object->GetBounciness();
		ImGui::DragFloat("Bounciness", &bounciness_);
		object->SetBounciness(bounciness_);

		if (ImGui::Button("BounceCombine::Average")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kAverage);
		}
		if (ImGui::Button("BounceCombine::Minimum")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMinimum);
		}
		if (ImGui::Button("BounceCombine::Maximum")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMaximum);
		}
		if (ImGui::Button("BounceCombine::Multiply")) {
			object->SetBounceCombine(yunity::Body::BounceCombine::kMultiply);
		}


		float miu_ = object->GetMiu();
		ImGui::DragFloat("Miu", &miu_);
		object->SetMiu(miu_);

		if (ImGui::Button("FrictionCombine::Average")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kAverage);
		}
		if (ImGui::Button("FrictionCombine::Minimum")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMinimum);
		}
		if (ImGui::Button("FrictionCombine::Maximum")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMaximum);
		}
		if (ImGui::Button("FrictionCombine::Multiply")) {
			object->SetFirictionCombine(yunity::Body::FrictionCombine::kMultiply);
		}
		ImGui::End();
	}

	for (int i = 0; auto & springJoint : springJoints_) {
		std::string name = "SpringJoint" + std::to_string(i++);
		ImGui::Begin(name.c_str());

		std::array<bool, 3> enableSpring = { springJoint->GetEnabledSpring(0), springJoint->GetEnabledSpring(1), springJoint->GetEnabledSpring(2) };
		ImGui::Checkbox("EnableSpringX", &enableSpring[0]);
		ImGui::Checkbox("EnableSpringY", &enableSpring[1]);
		ImGui::Checkbox("EnableSpringZ", &enableSpring[2]);

		for (int j = 0; j < 3; j++) {
			springJoint->EnableSpring(j, enableSpring[j]);
		}

		Vector3 equilibriumPoint = springJoint->GetEquilibriumPoint();
		ImGui::DragFloat3("EquilibriumPoint", &equilibriumPoint.x);
		springJoint->SetEquilibriumPoint(0, equilibriumPoint.x);
		springJoint->SetEquilibriumPoint(1, equilibriumPoint.y);
		springJoint->SetEquilibriumPoint(2, equilibriumPoint.z);

		Vector3 stiffness = springJoint->GetStiffness();
		ImGui::DragFloat3("Stiffness", &stiffness.x);
		springJoint->SetStiffness(0, stiffness.x);
		springJoint->SetStiffness(1, stiffness.y);
		springJoint->SetStiffness(2, stiffness.z);

		Vector3 damping = springJoint->GetDamping();
		ImGui::DragFloat3("Damping", &damping.x);
		springJoint->SetDamping(0, damping.x);
		springJoint->SetDamping(1, damping.y);
		springJoint->SetDamping(2, damping.z);


		ImGui::End();
	}
}

void DemoScene::Demo5Update()
{
}
