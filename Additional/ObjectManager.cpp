#include "ObjectManager.h"

#include "ModelManager.h"

void ObjectManager::Load(const std::string& fileName, yunity::Camera* camera, yunity::World* world) {
	std::unique_ptr<yunity::LevelData> levelData = std::make_unique<yunity::LevelData>();
	levelData.reset(yunity::LevelEditor::GetInstance()->LoadFile(fileName));

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
			newObject->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, objectData.shape);
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else {
			CreateBasicObject(objectData, camera, world);
		}

		if (objectData.jointPair_ != -1) {
			int parentNumber = objectData.jointPair_;
			if (jointData_[parentNumber].objA == nullptr) {
				jointData_[parentNumber].objA = objects_.back().get();
				jointData_[parentNumber].objectDataA = objectData;
			}
			else {
				jointData_[parentNumber].objB = objects_.back().get();
				jointData_[parentNumber].objectDataB = objectData;
			}
		}
	}

	for (uint32_t index = 0; auto & joint : jointData_) {
		if (joint.objectDataA.jointType_ == yunity::JointType::kSpring && joint.objectDataB.jointType_ == yunity::JointType::kSpring) {
			yunity::SpringJoint* springJoint = new yunity::SpringJoint();
			springJoint->CreateSpringJoint(joint.objA, joint.objB);
			for (uint32_t i = 0; i < 3; i++) {
				springJoint->EnableSpring(i, joint.objectDataA.springEnabled_[i]);
				springJoint->SetStiffness(i, joint.objectDataA.stiffness_[i]);
				springJoint->SetDamping(i, joint.objectDataA.dampingCoefficient_[i]);
			}
			springJoint->SetEquilibriumPoint();
			world->AddJoint(springJoint);
			joints_.emplace_back(springJoint);

			springLines_[index].objA = joint.objA;
			springLines_[index].objB = joint.objB;
			springLines_[index].isActive_ = true;
			index++;
		}
		else if (joint.objectDataA.jointType_ == yunity::JointType::kPulley && joint.objectDataB.jointType_ == yunity::JointType::kPulley) {
			yunity::PulleyJoint* pulleyJoint = new yunity::PulleyJoint();
			pulleyJoint->CreatePulleyJoint(joint.objA, joint.objB, joint.objectDataA.groundAnchor_, joint.objectDataB.groundAnchor_, joint.objectDataA.anchor_, joint.objectDataB.anchor_, (joint.objectDataA.ratio_ + joint.objectDataB.ratio_) / 2.0f);
			world->AddJoint(pulleyJoint);
			joints_.emplace_back(pulleyJoint);

		}
	}
}