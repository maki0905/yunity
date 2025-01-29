#include "CheckPoint.h"

#include "ModelManager.h"

void CheckPoint::Initialize()
{

	models_["OFF"] = std::make_unique<yunity::Model>();
	models_["OFF"].reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "TV"));
	models_["OFF"]->SetCamera(camera_);

	models_["ON"] = std::make_unique<yunity::Model>();
	models_["ON"].reset(yunity::ModelManager::GetInstance()->CreateModel(obj, "CheckPoint"));
	models_["ON"]->SetCamera(camera_);

	isActive_ = false;
}

void CheckPoint::Draw()
{
	if (isActive_) {
		models_["ON"]->Draw(worldTransform_);
	}
	else {
		models_["OFF"]->Draw(worldTransform_);
	}

}

void CheckPoint::OnTriggerEvent()
{
	if (GetHitBody()->GetCollisionAttribute() == kCollisionAttributePlayer) {
		isActive_ = true;
	}
}
