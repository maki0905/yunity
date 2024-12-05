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
			std::unique_ptr<MoveFloor> newObject = std::make_unique<MoveFloor>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->InitializeDirection(objectData.tag_);
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kTV) {
			std::unique_ptr<TV> newObject = std::make_unique<TV>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->InitializeTexture();
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kCoin) {
			std::unique_ptr<Coin> newObject = std::make_unique<Coin>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize();
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kForceField) {
			std::unique_ptr<ForceField> newObject = std::make_unique<ForceField>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize();
			AddObject(std::move(newObject));
		}
		else if (objectData.tag_ == Tag::kPillar) {
			std::unique_ptr<Pillar> newObject = std::make_unique<Pillar>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize();
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kClockwise) {
			std::unique_ptr<RotationCube> newObject = std::make_unique<RotationCube>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->SetCamera(camera);
			newObject->Initialize(1.0);
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kCounterClockwise) {
			std::unique_ptr<RotationCube> newObject = std::make_unique<RotationCube>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize(-1.0f);
			AddObject(std::move(newObject));

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