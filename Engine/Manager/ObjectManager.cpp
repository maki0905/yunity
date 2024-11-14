#include "ObjectManager.h"

#include "ModelManager.h"

void ObjectManager::Initialize()
{
	objects_.clear();
	joints_.clear();
}

void ObjectManager::Update()
{
	for (auto& object : objects_) {
		object->Update();
	}
}

void ObjectManager::Draw()
{
	
	for (auto& object : objects_) {
		if (object->GetModel() != nullptr) {
			if (object->GetModel()->GetModelName() == "GoalPost") {
				object->Draw();
			}
			else if (object->GetModel()->GetModelName() != "startBox") {
				object->Draw();
			}
		}
	}
}

void ObjectManager::Load(const std::string& fileName, Camera* camera, World* world)
{
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();
	levelData.reset(LevelEditor::GetInstance()->LoadFile(fileName));

	world_ = world;

	for (auto& object : levelData->objects) {
		if (object.tag_ == Tag::kMoveFloor_L || object.tag_ == Tag::kMoveFloor_R) {

			MoveFloor* newObject = new MoveFloor();
			InitializeCommon(object, newObject);
			InitializeCollider(object, newObject);
			InitializePhysics(object, newObject);
			newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, object.fileName), world, object.shape);
			newObject->InitializeDirection(object.tag_);
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else if (object.tag_ == Tag::kTV) {
			TV* newObject = new TV();
			InitializeCommon(object, newObject);
			InitializeCollider(object, newObject);
			InitializePhysics(object, newObject);
			newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, object.fileName), world, object.shape);
			newObject->InitializeTexture();
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else if (object.tag_ == Tag::kCoin) {
			Coin* newObject = new Coin();
			InitializeCommon(object, newObject);
			InitializeCollider(object, newObject);
			InitializePhysics(object, newObject);
			newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, object.fileName), world, object.shape);
			newObject->SetCamera(camera);
			world->Add(newObject);
			objects_.emplace_back(newObject);

		}
		else if (object.serialNumber != -1) {
			EventTrigger* newObject = new EventTrigger();
			InitializeCommon(object, newObject);
			InitializeCollider(object, newObject);
			newObject->Initialize(world, object.shape);
			world->Add(newObject);
			objects_.emplace_back(newObject);
		}
		else {
			Object3D* newObject = new Object3D();
			if (object.fileName == "GoalPost") {
				Vector3 pos = object.center;
			}
			InitializeCommon(object, newObject);
			InitializeCollider(object, newObject);
			InitializePhysics(object, newObject);
			if (object.empth) {
				newObject->Initialize(world, object.shape);
			}
			else {
				if (object.fileName == "Wood") {
					newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, object.fileName), world, object.shape);
				}
				else {
					newObject->Initialize(ModelManager::GetInstance()->CreateModel(obj, object.fileName), world, object.shape);
				}
			}
			newObject->SetCamera(camera);
			if (Length(object.size) > 0) {
				world->Add(newObject);
			}
			objects_.emplace_back(newObject);
		}
	}

}

Vector3 ObjectManager::GetPos(const std::string& modelName)
{
	for (auto& obj : objects_) {
		if (obj->GetModel()->GetModelName() == modelName) {
			return obj->GetMatWorldTranslation();
		}
	}
	return Vector3();
}

void ObjectManager::Clear()
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

void ObjectManager::SetDirectionalLight(Model::DirectionalLight directionalLight)
{
	for (auto& obj : objects_) {
		obj->SetEnableLighting(true);
		obj->SetDirectionalLight(directionalLight);
	}
}

void ObjectManager::SetEnableLighting(bool onOff)
{
	for (auto& obj : objects_) {
		obj->SetEnableLighting(onOff);
	}
}

void ObjectManager::InitializeCommon(const LevelData::ObjectData& objectData, Object3D* newObject)
{
	newObject->SetPosition(objectData.translation);
	newObject->SetRotation(objectData.rotation);
	newObject->SetScale(objectData.scaling);
}

void ObjectManager::InitializeCollider(const LevelData::ObjectData& objectData, Object3D* newObject)
{
	newObject->SetHitBoxSize(objectData.size);
	newObject->SetCenter(objectData.center);
	newObject->SetCollisionAttribute(0b1 << objectData.attribute);
	newObject->SetIsTrigger(objectData.isTrigger);
}

void ObjectManager::InitializePhysics(const LevelData::ObjectData& objectData, Object3D* newObject)
{
	newObject->SetMass(objectData.mass);
	newObject->SetDrag(objectData.drag);
	newObject->SetMiu(objectData.miu);
	newObject->SetFirictionCombine(static_cast<Body::FrictionCombine>(objectData.frictionCombine));
	newObject->SetBounciness(objectData.bounciness);
	newObject->SetBounceCombine(static_cast<Body::BounceCombine>(objectData.bounceCombine));
}

