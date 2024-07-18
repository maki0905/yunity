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
		if (object.fileName == "startBox") {
			Model* model = nullptr;
			model = ModelManager::GetInstance()->CreateModel(obj, object.fileName);
			model->SetCamera(camera);

		//std::unique_ptr<Object3D> newObject = std::make_unique<Object3D>();
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
		objects_[fileName].emplace_back(newObject);
	}
}
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
