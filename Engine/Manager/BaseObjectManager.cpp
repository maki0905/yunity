#include "BaseObjectManager.h"

#include "ModelManager.h"


void yunity::BaseObjectManager::Initialize(Camera* camera)
{
	objects_.clear();
	joints_.clear();
	for (uint32_t i = 0; i < jointNumber_; i++) {
		jointData_[i].objA = nullptr;
		jointData_[i].objB = nullptr;
	}
	springLines_.clear();
	camera_ = camera;

}

void yunity::BaseObjectManager::Update()
{
	for (auto& object : objects_) {
		object->Update();
	}
}

void yunity::BaseObjectManager::Draw()
{

	for (auto& springLine : springLines_) {
		if (springLine->isActive_) {
			springLine->line->Draw(springLine->objA->GetMatWorldTranslation(), springLine->objB->GetMatWorldTranslation(), { 0.0f, 0.0f, 0.0f, 1.0f });
		}
	}

	for (auto& object : objects_) {
		if (object->GetModel() != nullptr) {
			if (object->GetModel()->GetModelName() != "startBox") {
				object->Draw();
			}
		}
	}
}

void yunity::BaseObjectManager::Load(const std::string& objectFileName, Camera* camera, World* world, const std::string& jointFileName)
{
	LevelData* levelData = nullptr;
	levelData = LevelEditor::GetInstance()->LoadFile(objectFileName);
	JointData* jointData = nullptr;
	bool jointDataCheck = false;
	camera_ = camera;
	if (jointFileName.size() != 0) {
		jointData = LevelEditor::GetInstance()->LoadJointFile(jointFileName);
		jointDataCheck = true;
		LoadJoint(*jointData);
	}

	for (auto& object : levelData->objects) {
		CreateBasicObject(object, camera, world);
		if (jointDataCheck) {
			AddJointData(object, objects_.back().get());
		}
	}

	if (jointDataCheck) {
		CreateJoint();
	}
}

void yunity::BaseObjectManager::SetInitalizeData(const LevelData::ObjectData& objectData, Object3D* newObject, Camera* camera)
{
	InitializeCommon(objectData, newObject);
	InitializeCollider(objectData, newObject);
	InitializePhysics(objectData, newObject);
	if (objectData.fileName != "") {
		newObject->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world_, objectData.shape);
	}
	else {
		newObject->Initialize(world_, objectData.shape);
	}
	newObject->SetCamera(camera);
}

void yunity::BaseObjectManager::AddObject(std::unique_ptr<Object3D> newObject)
{
	world_->Add(newObject.get());
	objects_.emplace_back(std::move(newObject));
}



Vector3 yunity::BaseObjectManager::GetPos(const std::string& modelName)
{
	for (auto& obj : objects_) {
		if (obj->GetModel()->GetModelName() == modelName) {
			return obj->GetMatWorldTranslation();
		}
	}
	return Vector3();
}

void yunity::BaseObjectManager::Clear()
{
	for (auto& obj : objects_) {
		obj->GetWorld()->Take(obj.get());
	}
	objects_.clear();

	for (auto& joint : joints_) {
		world_->TakeJoint(joint.get());
	}
	joints_.clear();
}

void yunity::BaseObjectManager::SetDirectionalLight(Model::DirectionalLight directionalLight)
{
	for (auto& obj : objects_) {
		obj->SetEnableLighting(true);
		obj->SetDirectionalLight(directionalLight);
	}
}

void yunity::BaseObjectManager::SetEnableLighting(bool onOff)
{
	for (auto& obj : objects_) {
		obj->SetEnableLighting(onOff);
	}
}

void yunity::BaseObjectManager::CreateBasicObject(const LevelData::ObjectData& objectData, Camera* camera, World* world)
{
	if (objectData.serialNumber != -1) { // イベントトリガー
		std::unique_ptr<EventTrigger> newObject = std::make_unique<EventTrigger>();
		InitializeCommon(objectData, newObject.get());
		InitializeCollider(objectData, newObject.get());
		newObject->Initialize(world, objectData.shape);
		AddObject(std::move(newObject));
	}
	else { // 基本オブジェクト
		std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();
		SetInitalizeData(objectData, newObject.get(), camera);
		if (objectData.empth) {
			newObject->Initialize(world, objectData.shape);
		}
		else {
			newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, objectData.shape);
		}
		newObject->SetCamera(camera);
		if (Length(objectData.size) > 0) {
			world->Add(newObject.get());
		}
		objects_.emplace_back(std::move(newObject));
	}
}

void yunity::BaseObjectManager::InitializeCommon(const LevelData::ObjectData& objectData, Object3D* newObject)
{
	newObject->SetPosition(objectData.translation);
	newObject->SetRotation(objectData.rotation);
	newObject->SetScale(objectData.scaling);
}

void yunity::BaseObjectManager::InitializeCollider(const LevelData::ObjectData& objectData, Object3D* newObject)
{
	newObject->SetHitBoxSize(objectData.size);
	newObject->SetCenter(objectData.center);
	newObject->SetCollisionAttribute(0b1 << objectData.attribute);
	newObject->SetIsTrigger(objectData.isTrigger);
}

void yunity::BaseObjectManager::InitializePhysics(const LevelData::ObjectData& objectData, Object3D* newObject)
{
	newObject->SetMass(objectData.mass);
	newObject->SetDrag(objectData.drag);
	newObject->SetMiu(objectData.miu);
	newObject->SetFirictionCombine(static_cast<Body::FrictionCombine>(objectData.frictionCombine));
	newObject->SetBounciness(objectData.bounciness);
	newObject->SetBounceCombine(static_cast<Body::BounceCombine>(objectData.bounceCombine));
}

void yunity::BaseObjectManager::LoadJoint(const JointData& jointData)
{
	// SpringJoint
	for (auto& joint : jointData.springJoints) {
		jointData_[joint.id].objA = nullptr;
		jointData_[joint.id].objB = nullptr;
		jointData_[joint.id].jointData.id = joint.id;
		jointData_[joint.id].jointData.type = joint.type;
		jointData_[joint.id].springJointData = joint;
	}
	// PulleyJoint
	for (auto& joint : jointData.pulleyJoints) {
		jointData_[joint.id].objA = nullptr;
		jointData_[joint.id].objB = nullptr;
		jointData_[joint.id].jointData.id = joint.id;
		jointData_[joint.id].jointData.type = joint.type;
		jointData_[joint.id].pulleyJointData = joint;
	}
}

void yunity::BaseObjectManager::AddJointData(const LevelData::ObjectData& objectData, Object3D* object)
{
	if (objectData.jointIDs.size() > 0) {
		for (auto& id : objectData.jointIDs) {
			if (jointData_[id].objA == nullptr) {
				jointData_[id].objA = object;
			}
			else {
				jointData_[id].objB = object;
			}
		}
	}

}

void yunity::BaseObjectManager::CreateJoint()
{
	for (auto& joint : jointData_) {
		if (joint.objA == nullptr || joint.objB == nullptr) {
			continue;
		}
		switch (joint.jointData.type)
		{
		case JointType::kDefalt:
			break;
		case JointType::kSpring:
			InitializeSpringJoint(joint);
			InitializeSpringLine(joint);
			break;
		case JointType::kPulley:
			InitializePulleyJoint(joint);
			break;

		}
	}
}

void yunity::BaseObjectManager::InitializeSpringJoint(const JointObject& joint)
{
	std::unique_ptr<SpringJoint> springJoint = std::make_unique<SpringJoint>();
	springJoint->CreateSpringJoint(joint.objA, joint.objB);
	for (uint32_t i = 0; i < 3; i++) {
		springJoint->EnableSpring(i, joint.springJointData.springEnabled[i]);
		springJoint->SetEquilibriumPoint(i, joint.springJointData.equilibriumPoint[i]);
		springJoint->SetStiffness(i, joint.springJointData.stiffness[i]);
		springJoint->SetDamping(i, joint.springJointData.dampingCoefficient[i]);
	}
	joints_.emplace_back(std::move(springJoint));
	world_->AddJoint(joints_.back().get());
}

void yunity::BaseObjectManager::InitializeSpringLine(const JointObject& joint)
{
	std::unique_ptr<SpringJointLine> line = std::make_unique<SpringJointLine>();
	line->objA = joint.objA;
	line->objB = joint.objB;
	line->isActive_ = true;
	line->line = std::make_unique<PrimitiveDrawer>();
	line->line.reset(PrimitiveDrawer::Create());
	line->line->SetCamera(camera_);
	springLines_.push_back(std::move(line));
}

void yunity::BaseObjectManager::InitializePulleyJoint(const JointObject& joint)
{
	std::unique_ptr<PulleyJoint> pulleyJoint = std::make_unique<PulleyJoint>();
	pulleyJoint->CreatePulleyJoint(
		joint.objA, joint.objB,
		joint.pulleyJointData.groundAnchorA, joint.pulleyJointData.groundAnchorB,
		joint.pulleyJointData.anchorA, joint.pulleyJointData.anchorB,
		joint.pulleyJointData.ratio);
	joints_.emplace_back(std::move(pulleyJoint));
	world_->AddJoint(joints_.back().get());
}


