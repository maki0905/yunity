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

void ObjectManager::Load(const std::string& fileName, Camera* camera, World* world)
{
	LevelData* levelData = LevelEditor::GetInstance()->LoadFile(fileName);

	for (auto& object : levelData->objects) {
		if (object.fileName == "startBox") {
			Model* model = nullptr;
			model = ModelManager::GetInstance()->CreateModel(obj, object.fileName);
			model->SetCamera(camera);

			MoveFloor* newObject = new MoveFloor();
			newObject->SetPosition(object.translation);
			newObject->SetRotation(object.rotation);
			newObject->SetScale(object.scaling);
			newObject->SetSize(object.size);
			newObject->SetCenter(object.center);
			newObject->SetIsTrigger(object.isTrigger);
			newObject->Initialize(model, world, object.shape);
			world->Add(newObject);
			objects_[fileName].push_back(newObject);

		}
		else {
			Model* model = nullptr;
			model = ModelManager::GetInstance()->CreateModel(obj, object.fileName);
			model->SetCamera(camera);

			Object3D* newObject = new Object3D();
			newObject->SetPosition(object.translation);
			newObject->SetRotation(object.rotation);
			newObject->SetScale(object.scaling);
			newObject->SetSize(object.size);
			newObject->SetCenter(object.center);
			newObject->SetIsTrigger(object.isTrigger);
			newObject->Initialize(model, world, object.shape);
			world->Add(newObject);
			objects_[fileName].push_back(newObject);
		}
	}
}

std::vector<Object3D*> ObjectManager::GetObjects(const std::string& fileName)
{
	return objects_[fileName];
}

Vector3 ObjectManager::GetPos(const std::string& fileName, const std::string& modelName) {
	Vector3 result;
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
