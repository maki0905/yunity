#pragma once
#include <list>
#include <vector>
#include <memory>

#include "Camera.h"
#include "DebugCamera.h"

/*
* @brief カメラクラス
*/
class CameraManager {
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static CameraManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// ファイナライザ
	/// </summary>
	void Finalize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	Camera* GetCamera() { return camera_.get(); }
private:
	std::unique_ptr<Camera> camera_;
	DebugCamera* debugCamera_;
	bool isDebug_;

};