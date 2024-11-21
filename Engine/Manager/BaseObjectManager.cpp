#include "BaseObjectManager.h"

#include "ModelManager.h"

void yunity::BaseObjectManager::Initialize(Camera* camera)
{
	objects_.clear();
	joints_.clear();
	for (uint32_t i = 0; i < jointNumber_; i++) {
		jointData_[i].objA = nullptr;
		jointData_[i].objB = nullptr;
		springLines_[i].objA = nullptr;
		springLines_[i].objB = nullptr;
		springLines_[i].line = std::make_unique<PrimitiveDrawer>();
		springLines_[i].line.reset(PrimitiveDrawer::Create());
		springLines_[i].line->SetCamera(camera);
		springLines_[i].isActive_ = false;
	}

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
		if (springLine.isActive_) {
			springLine.line->Draw(springLine.objA->GetMatWorldTranslation(), springLine.objB->GetMatWorldTranslation(), { 0.0f, 0.0f, 0.0f, 1.0f });
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


void yunity::BaseObjectManager::Load(const std::string& fileName, Camera* camera, World* world)
{
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();
	levelData.reset(LevelEditor::GetInstance()->LoadFile(fileName));



	for (auto& object : levelData->objects) {
		CreateBasicObject(object, camera, world);
	}

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
		EventTrigger* newObject = new EventTrigger();
		InitializeCommon(objectData, newObject);
		InitializeCollider(objectData, newObject);
		newObject->Initialize(world, objectData.shape);
		world->Add(newObject);
		objects_.emplace_back(newObject);
	}
	else { // 基本オブジェクト
		Object3D* newObject = new Object3D();
		InitializeCommon(objectData, newObject);
		InitializeCollider(objectData, newObject);
		InitializePhysics(objectData, newObject);
		if (objectData.empth) {
			newObject->Initialize(world, objectData.shape);
		}
		else {
			newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, objectData.fileName), world, objectData.shape);
		}
		newObject->SetCamera(camera);
		if (Length(objectData.size) > 0) {
			world->Add(newObject);
		}
		objects_.emplace_back(newObject);
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

