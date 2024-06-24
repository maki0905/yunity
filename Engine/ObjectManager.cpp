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
		Model* model = nullptr;
		model = ModelManager::GetInstance()->CreateModel(obj, object.fileName);
		model->SetCamera(camera);

		Object3D* newObject = new Object3D();
		newObject->SetPosition(object.translation);
		newObject->SetRotation(object.rotation);
		newObject->SetScale(object.scaling);
		newObject->SetSize(object.size);
		newObject->SetCenter(object.center);
		newObject->Create(model, world, object.ColliderType);
		

		objects_[fileName].push_back(newObject);
	}
}

std::vector<Object3D*> ObjectManager::GetObjects(const std::string& fileName)
{
	return objects_[fileName];
}
