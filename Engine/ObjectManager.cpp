#include "ObjectManager.h"

#include "LevelEditor.h"
#include "ModelManager.h"

ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager instance;
	return &instance;

}

void ObjectManager::Initialize()
{
	objects_.clear();
}

void ObjectManager::Update(const std::string& fileName)
{
	for (auto& object : objects_[fileName]) {
		object->Update();
	}
}

void ObjectManager::Draw(const std::string& fileName)
{
	for (auto& object : objects_[fileName]) {
		object->Draw();
	}
}

void ObjectManager::Load(const std::string& fileName, Camera* camera, World* world)
{
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();
	levelData.reset(LevelEditor::GetInstance()->LoadFile(fileName));

	for (auto& object : levelData->objects) {
		if (object.fileName == "MoveFloor") {

			MoveFloor* newObject = new MoveFloor();
			newObject->SetPosition(object.translation);
			newObject->SetRotation(object.rotation);
			newObject->SetScale(object.scaling);
			newObject->SetSize(object.size);
			newObject->SetCenter(object.center);
			newObject->SetIsTrigger(object.isTrigger);
			newObject->SetMass(object.mass);
			newObject->SetDrag(object.drag);
			newObject->SetMiu(object.miu);
			newObject->SetFirictionCombine(static_cast<Body::FrictionCombine>(object.frictionCombine));
			newObject->SetBounciness(object.bounciness);
			newObject->SetBounceCombine(static_cast<Body::BounceCombine>(object.bounceCombine));
			newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, object.fileName), world, object.shape);
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_[fileName].emplace_back(newObject);

		}
		else if (object.serialNumber != -1) {
			EventTrigger* newObject = new EventTrigger();
			newObject->SetPosition(object.translation);
			newObject->SetRotation(object.rotation);
			newObject->SetScale(object.scaling);
			newObject->SetSize(object.size);
			newObject->SetCenter(object.center);
			newObject->SetIsTrigger(object.isTrigger);
			newObject->SetSerialNumber(object.serialNumber);
			newObject->SetCollisionAttribute(0b1 << object.attribute);
			newObject->Initialize(world, object.shape);
			world->Add(newObject);
			objects_[fileName].emplace_back(newObject);
		}
		else {
			Object3D* newObject = new Object3D();
			newObject->SetPosition(object.translation);
			newObject->SetRotation(object.rotation);
			newObject->SetScale(object.scaling);
			newObject->SetSize(object.size);
			newObject->SetCenter(object.center);
			newObject->SetIsTrigger(object.isTrigger);
			newObject->SetMass(object.mass);
			newObject->SetDrag(object.drag);
			newObject->SetMiu(object.miu);
			newObject->SetFirictionCombine(static_cast<Body::FrictionCombine>(object.frictionCombine));
			newObject->SetBounciness(object.bounciness);
			newObject->SetBounceCombine(static_cast<Body::BounceCombine>(object.bounceCombine));
			newObject->SetCollisionAttribute(0b1 << object.attribute);
			if (object.empth) {
				newObject->Initialize(world, object.shape);
			}
			else {
				newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, object.fileName), world, object.shape);
			}
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_[fileName].emplace_back(newObject);
		}
	}
}

//std::vector<std::unique_ptr<Object3D>> ObjectManager::GetObjects(const std::string& fileName)
//{
//	return std::move(objects_[fileName]);
//}

Vector3 ObjectManager::GetPos(const std::string& fileName, const std::string& modelName) {
	Vector3 result = { 0.0f, 0.0f, 0.0f };
	for (auto& obj : objects_[fileName]) {
		if (obj->GetModel()->GetModelName() == modelName) {
			result = obj->GetTranslation();
		}
	}
	return result;
}

//std::vector<Object3D*> ObjectManager::GetObj(const std::string& fileName, const std::string& modelName)
//{
//	std::vector<Object3D*> result;
//	for (auto& obj : objects_[fileName]) {
//		if (obj->GetModel()->GetModelName() == modelName) {
//			result.push_back(obj);
//		}
//	}
//	return result;
//}
