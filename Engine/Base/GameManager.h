#pragma once

#include "Framework.h"
#include "CameraManager.h"

class GameManager : public Framework
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameManager();

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	void Run();
private:
	std::unique_ptr<CameraManager> cameraManager_;
	
};

