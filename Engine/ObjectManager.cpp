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
		/*Model* model = nullptr;
		model = ModelManager::GetInstance()->CreateModel(obj, object.fileName);
		model->SetCamera(camera);*/
		/*std::unique_ptr<Model> model = std::make_unique<Model>();
		model.reset(ModelManager::GetInstance()->CreateModel(obj, object.fileName));*/

		Object3D* newObject = new Object3D();
		newObject->SetPosition(object.translation);
		newObject->SetRotation(object.rotation);
		newObject->SetScale(object.scaling);
		newObject->SetSize(object.size);
		newObject->SetCenter(object.center);
		newObject->SetIsTrigger(object.isTrigger);
		newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, object.fileName), world, object.shape);
		newObject->SetCamera(camera);
		world->Add(newObject);
		objects_[fileName].push_back(newObject);
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
