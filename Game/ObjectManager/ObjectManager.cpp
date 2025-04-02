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
			CreateObject<MoveFloor>(objectData, camera);
		}
		else if (objectData.tag_ == Tag::kTV) { // TV
			CreateObject<TV>(objectData, camera);

		}
		else if (objectData.tag_ == Tag::kCoin) { // コイン
			CreateObject<Coin>(objectData, camera);

		}
		else if (objectData.tag_ == Tag::kForceField) { // 力場
			CreateObject<ForceField>(objectData, camera);
		}
		else if (objectData.tag_ == Tag::kClockwise || objectData.tag_ == Tag::kCounterClockwise) { // 回転キューブ
			CreateObject<RotationCube>(objectData, camera);

		}
		else if (objectData.tag_ == Tag::kSpringBoard) { // スプリングボード
			CreateObject<SpringBoard>(objectData, camera);
		}
		else if (objectData.tag_ == Tag::kSignboard) { // 看板
			CreateObject<Signboard>(objectData, camera);
		}
		else if (objectData.tag_ == Tag::kSelectTV) { // ステージ選択TV
			CreateObject<SelectTV>(objectData, camera);
		}
		else if (objectData.tag_ == Tag::kCheckPoint) { // チェックポイント
			CreateObject<CheckPoint>(objectData, camera);
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