#include "ObjectManager.h"

#include "ModelManager.h"

//ObjectManager* ObjectManager::GetInstance()
//{
//	static ObjectManager instance;
//	return &instance;
//
//}

void ObjectManager::Initialize()
{
	objects_.clear();
	joints_.clear();
	//activeObjects_.clear();
}

void ObjectManager::Update(/*const std::string& fileName*/)
{
	//uint32_t index = 0;
	//for (auto& object : objects_[fileName]) {
	//	object->Update();
	//	/*if (activeObjects_[fileName][index++]) {
	//		object->Update();
	//	}*/
	//}
	for (auto& object : objects_) {
		object->Update();
		/*if (activeObjects_[fileName][index++]) {
			object->Update();
		}*/
	}
}

void ObjectManager::Draw(/*const std::string& fileName*/)
{
	/*uint32_t index = 0;*/
	//for (auto& object : objects_[fileName]) {
	//	object->Draw();
	//	/*if (activeObjects_[fileName][index++]) {
	//		object->Draw();
	//	}*/
	//}
	for (auto& object : objects_) {
		if (object->GetModel() != nullptr) {
			if (object->GetModel()->GetModelName() != "startBox") {
				object->Draw();
			}
		}
		/*if (activeObjects_[fileName][index++]) {
			object->Draw();
		}*/
	}
}

void ObjectManager::Load(const std::string& fileName, Camera* camera, World* world)
{
	std::unique_ptr<LevelData> levelData = std::make_unique<LevelData>();
	levelData.reset(LevelEditor::GetInstance()->LoadFile(fileName));

	//std::array<std::pair<Object3D*, LevelData::ObjectData>, 50> jointPair;

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

			/*if (object.jointPair_ > -1) {
				if (jointPair[object.jointPair_].first == nullptr) {
					jointPair[object.jointPair_].first = newObject;
					jointPair[object.jointPair_].second = object;
				}
				else {
					if (object.jointType_ == JointType::kSpring) {
						SpringJoint* springJoint = new SpringJoint();
						springJoint->CreateSpringJoint(jointPair[object.jointPair_].first, newObject);
						for (uint32_t i = 0; i < 3; i++) {
							springJoint->EnableSpring(i, object.springEnabled_[i]);
							springJoint->SetStiffness(i, object.stiffness_[i]);
							springJoint->SetDamping(i, object.dampingCoefficient_[i]);
							springJoint->SetEquilibriumPoint(i, object.equilibriumPoint_[i]);
						}
						world->AddJoint(springJoint);
						joints_.emplace_back(springJoint);
					}
					else if(object.jointType_ == JointType::kPulley){
						PulleyJoint* pulleyJoint = new PulleyJoint();
						pulleyJoint->CreatePulleyJoint(jointPair[object.jointPair_].first, newObject, jointPair[object.jointPair_].second.groundAnchor_, object.groundAnchor_, jointPair[object.jointPair_].second.anchor_, object.anchor_, object.ratio_);
						world->AddJoint(pulleyJoint);
						joints_.emplace_back(pulleyJoint);
					}
				}
			}*/
		}
	}

}

//std::vector<std::unique_ptr<Object3D>> ObjectManager::GetObjects(const std::string& fileName)
//{
//	return std::move(objects_[fileName]);
//}

//Vector3 ObjectManager::GetPos(const std::string& fileName, const std::string& modelName) {
//	Vector3 result = { 0.0f, 0.0f, 0.0f };
//	for (auto& obj : objects_[fileName]) {
//		Model* model = obj->GetModel();
//		if (model == nullptr) {
//			continue;
//		}
//		if (model->GetModelName() == modelName) {
//			result = obj->GetTranslation();
//		}
//	}
//	return result;
//}

Vector3 ObjectManager::GetPos(const std::string& modelName)
{
	for (auto& obj : objects_) {
		if (obj->GetModel()->GetModelName() == modelName) {
			return obj->GetMatWorldTranslation();
		}
	}
	return Vector3();
}

void ObjectManager::Clear(/*const std::string& fileName*/)
{
	/*for (auto& obj : objects_[fileName]) {
		obj->GetWorld()->Take(obj.get());
	}*/
	//objects_[fileName].clear();
	for (auto& obj : objects_) {
		obj->GetWorld()->Take(obj.get());
	}
	objects_.clear();

	for (auto& joint : joints_) {
		world_->TakeJoint(joint.get());
	}
	joints_.clear();
}

//void ObjectManager::Reset(const std::string& fileName)
//{
//
//
//}

void ObjectManager::SetDirectionalLight(/*const std::string& fileName, */Model::DirectionalLight directionalLight)
{
	/*for (auto& obj : objects_[fileName]) {
		obj->SetEnableLighting(true);
		obj->SetDirectionalLight(directionalLight);
	}*/
	for (auto& obj : objects_) {
		obj->SetEnableLighting(true);
		obj->SetDirectionalLight(directionalLight);
	}
}

void ObjectManager::SetEnableLighting(/*const std::string& fileName,*/ bool onOff)
{
	/*for (auto& obj : objects_[fileName]) {
		obj->SetEnableLighting(onOff);
	}*/
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

//void ObjectManager::SetActive(const std::string& fileName, uint32_t index, bool active)
//{
//	activeObjects_[fileName][index] = active;
//}



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
