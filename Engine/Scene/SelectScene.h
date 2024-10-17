#pragma once

#include <memory>

#include "IScene.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Model.h"
#include "World.h"
#include "Player.h"
#include "Skydome.h"

/*
* @brief セレクトシーンクラス
*/
class SelectScene : public IScene
{
	struct Easing {
		bool flag;
		float t;
		Vector3 scale;
	};
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	
	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawBack() override;
	
	/// <summary>
	/// 3D描画
	/// </summary>
	void Draw3D() override;
	
	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawFront() override;

private:
	/*std::unique_ptr<Camera>*/ Camera* camera_;

	XINPUT_STATE pad_;
	XINPUT_STATE prePad_;

	WorldTransform worldTransform_[3];
	std::unique_ptr<Sprite> sprite_ = nullptr;
	std::unique_ptr<Model> models_[3];
	bool isActiveTV_[3];
	int preNum_;
	Easing grow_[3];
	Easing shrink_[3];
	uint32_t textureTV_[3];
	std::unique_ptr<World> world_;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Skydome> skydome_;
};