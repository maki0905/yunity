#include "ObjectManager.h"

#include "ModelManager.h"

void ObjectManager::Load(const std::string& objectFileName, yunity::Camera* camera, yunity::World* world, const std::string& jointFileName) {
	std::unique_ptr<yunity::LevelData> levelData = std::make_unique<yunity::LevelData>();
	levelData.reset(yunity::LevelEditor::GetInstance()->LoadFile(objectFileName));
	std::unique_ptr<yunity::JointData> jointData = std::make_unique<yunity::JointData>();
	bool jointDatacheck = false;
	if (jointFileName.size() != 0) {
		jointData.reset(yunity::LevelEditor::GetInstance()->LoadJointFile(jointFileName));
		jointDatacheck = true;
		LoadJoint(*jointData.get());
	}

	world_ = world;

	for (auto& objectData : levelData->objects) {
		if (objectData.tag_ == Tag::kMoveFloor_L || objectData.tag_ == Tag::kMoveFloor_R) {

			MoveFloor* newObject = new MoveFloor();
			InitializeCommon(objectData, newObject);
			InitializeCollider(objectData, newObject);
			InitializePhysics(objectData, newObject);
			newObject->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, objectData.shape);
			newObject->InitializeDirection(objectData.tag_);
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else if (objectData.tag_ == Tag::kTV) {
			TV* newObject = new TV();
			InitializeCommon(objectData, newObject);
			InitializeCollider(objectData, newObject);
			InitializePhysics(objectData, newObject);
			newObject->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, objectData.shape);
			newObject->InitializeTexture();
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else if (objectData.tag_ == Tag::kCoin) {
			Coin* newObject = new Coin();
			InitializeCommon(objectData, newObject);
			InitializeCollider(objectData, newObject);
			InitializePhysics(objectData, newObject);
			newObject->Object3D::Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, objectData.shape);
			newObject->Initialize();
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else if (objectData.tag_ == Tag::kForceField) {
			ForceField* newObject = new ForceField();
			InitializeCommon(objectData, newObject);
			InitializeCollider(objectData, newObject);
			InitializePhysics(objectData, newObject);
			newObject->Object3D::Initialize(world, objectData.shape);
			newObject->Initialize();
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_.emplace_back(newObject);
		}
		else if (objectData.tag_ == Tag::kPillar) {
			Pillar* newObject = new Pillar();
			InitializeCommon(objectData, newObject);
			InitializeCollider(objectData, newObject);
			InitializePhysics(objectData, newObject);
			newObject->Object3D::Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, objectData.shape);
			newObject->SetCamera(camera);
			newObject->Initialize();
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else if (objectData.tag_ == Tag::kClockwise) {
			RotationCube* newObject = new RotationCube();
			InitializeCommon(objectData, newObject);
			InitializeCollider(objectData, newObject);
			InitializePhysics(objectData, newObject);
			newObject->Object3D::Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, yunity::Collider::kOBB);
			newObject->SetCamera(camera);
			newObject->Initialize(1.0f);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else if (objectData.tag_ == Tag::kCounterClockwise) {
			RotationCube* newObject = new RotationCube();
			InitializeCommon(objectData, newObject);
			InitializeCollider(objectData, newObject);
			InitializePhysics(objectData, newObject);
			newObject->Object3D::Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, yunity::Collider::kOBB);
			newObject->SetCamera(camera);
			newObject->Initialize(-1.0f);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else {
			CreateBasicObject(objectData, camera, world);
		}
		if (jointDatacheck) {
			AddJointData(objectData, objects_.back().get());
		}
	}
	if (jointDatacheck) {
		CreateJoint();
	}
}