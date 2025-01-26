#include "ObjectManager.h"

#include "ModelManager.h"
#include "CommonData.h"

void ObjectManager::CreatePlayer(yunity::Camera* camera, yunity::World* world)
{
	std::unique_ptr<Player> newObject = std::make_unique<Player>();
	newObject->SetTag(Tag::kPlayer);
	newObject->Initialize(camera, world);
	objects_.push_back(std::move(newObject));

}

void ObjectManager::Load(const std::string& objectFileName, yunity::Camera* camera, yunity::World* world, const std::string& jointFileName) {
	yunity::LevelData* levelData = nullptr;
	levelData = yunity::LevelEditor::GetInstance()->LoadFile(objectFileName);
	yunity::JointData* jointData = nullptr;
	bool jointDatacheck = false;
	if (jointFileName.size() != 0) {
		jointData = yunity::LevelEditor::GetInstance()->LoadJointFile(jointFileName);
		jointDatacheck = true;
		LoadJoint(*jointData);
	}

	world_ = world;
	camera_ = camera;

	for (auto& objectData : levelData->objects) {
		if (objectData.tag_ == Tag::kMoveFloor_L || objectData.tag_ == Tag::kMoveFloor_R) { // 移動床
			std::unique_ptr<MoveFloor> newObject = std::make_unique<MoveFloor>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->InitializeDirection(objectData.tag_);
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kTV) { // TV
			std::unique_ptr<TV> newObject = std::make_unique<TV>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->InitializeTexture();
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kCoin) { // コイン
			std::unique_ptr<Coin> newObject = std::make_unique<Coin>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize();
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kForceField) { // 力場
			std::unique_ptr<ForceField> newObject = std::make_unique<ForceField>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize();
			AddObject(std::move(newObject));
		}
		else if (objectData.tag_ == Tag::kClockwise) { // 時計回りの回転キューブ
			std::unique_ptr<RotationCube> newObject = std::make_unique<RotationCube>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->SetCamera(camera);
			newObject->Initialize(1.0);
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kCounterClockwise) { // 反時計回りの回転キューブ
			std::unique_ptr<RotationCube> newObject = std::make_unique<RotationCube>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize(-1.0f);
			AddObject(std::move(newObject));

		}
		else if (objectData.tag_ == Tag::kSpringBoard) { // スプリングボード
			std::unique_ptr<SpringBoard> newObject = std::make_unique<SpringBoard>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize();
			AddObject(std::move(newObject));
		}
		else if (objectData.tag_ == Tag::kSignboard) { // 看板
			std::unique_ptr<Signboard> newObject = std::make_unique<Signboard>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize();
			AddObject(std::move(newObject));
		}
		else if (objectData.tag_ == Tag::kSelectTV) { // ステージ選択TV
			std::unique_ptr<SelectTV> newObject = std::make_unique<SelectTV>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize();
			AddObject(std::move(newObject));
		}
		else if (objectData.tag_ == Tag::kCheckPoint) { // チェックポイント
			std::unique_ptr<CheckPoint> newObject = std::make_unique<CheckPoint>();
			SetInitalizeData(objectData, newObject.get(), camera);
			newObject->Initialize(camera);
			AddObject(std::move(newObject));
		}
		else if (objectData.tag_ == Tag::kStartObject) { // スタート地点
			CommonData::GetInstance()->start_.translation_ = objectData.translation;
			CommonData::GetInstance()->start_.UpdateMatrix();
		}
		else { // その他
			CreateBasicObject(objectData, camera, world);
		}
		if (jointDatacheck) { // ジョイントデータがある場合
			if (objects_.size() != 0) {
				AddJointData(objectData, objects_.back().get());
			}
		}
	}
	if (jointDatacheck) { // 溜めたジョイントデータがある場合
		CreateJoint();
	}
}