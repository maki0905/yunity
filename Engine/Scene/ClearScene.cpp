#include "ClearScene.h"

#include "ModelManager.h"
#include "LevelEditor.h"
#include "CameraManager.h"


void ClearScene::Initialize()
{
	world_ = std::make_unique<yunity::World>();
	world_->Initialize(gravity_);

	floor_ = std::make_unique<yunity::Object3D>();
	floor_->Initialize(yunity::ModelManager::GetInstance()->CreateModel(obj, "Grasslands"), world_, yunity::Collider::Shape::kAABB);
}

void ClearScene::Update()
{
	
}

void ClearScene::DrawBack()
{
}

void ClearScene::Draw3D()
{

}

void ClearScene::DrawFront()
{
}
