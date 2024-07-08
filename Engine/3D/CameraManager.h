#pragma once
#include <list>
#include <vector>
#include <memory>

#include "Camera.h"
#include "DebugCamera.h"

class CameraManager {
public:
	static CameraManager* GetInstance();

	void Initialize();
	void Finalize();

	void Update();

	Camera* GetCamera() { return camera_; }
private:
	/*std::unique_ptr<Camera> camera_;
	std::unique_ptr<DebugCamera> debugCamera_;*/
	Camera* camera_;
	DebugCamera* debugCamera_;
	bool isDebug_;

};