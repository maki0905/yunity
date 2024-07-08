#include "CameraManager.h"

CameraManager* CameraManager::GetInstance()
{
    static CameraManager instance;
    return &instance;
}

void CameraManager::Initialize()
{
    /*camera_ = std::make_unique<Camera>();
    debugCamera_ = std::make_unique<DebugCamera>();*/
	camera_ = new Camera();
	debugCamera_ = new DebugCamera();
	isDebug_ = false;
}

void CameraManager::Finalize()
{
	if (camera_) {
		delete camera_;
	}
	if (debugCamera_) {
		delete debugCamera_;
	}
}

void CameraManager::Update()
{
	if (Input::GetInstance()->TriggerKey(DIK_LSHIFT)) {
		isDebug_ ^= true;
	}

	if (isDebug_) {
		debugCamera_->Update(camera_);
		camera_->Update();
	}
	else {
		camera_->Update();
	}
}
