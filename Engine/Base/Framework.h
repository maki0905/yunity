#pragma once

#include <memory>

#pragma region シーン
#include "IScene.h"
#include "TitleScene.h"
#include "StageScene.h"
#include "ClearScene.h"
#pragma endregion 

#include "DirectXCore.h"
#include "WindowsAPI.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Model.h"
#include "Sprite.h"
#include "PrimitiveDrawer.h"
#include "ParticleDrawer.h"
#include "SphereDrawer.h"
#include "Audio/Audio.h"
#include "SceneManager.h"
#include "ModelManager.h"
#include "CSV.h"
#include "SkyBox.h"

namespace yunity {
	/*
	* @brief フレームワーククラス
	*/
	class Framework {
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// ファイナライザ
		/// </summary>
		virtual void Finalize();

		/// <summary>
		/// 更新
		/// </summary>
		virtual void Update();

		/// <summary>
		/// 描画
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// 終了チェック
		/// </summary>
		/// <returns></returns>
		virtual bool IsEndRequest() { return endRequest_; }

		/// <summary>
		/// 実行
		/// </summary>
		void Run();

		virtual ~Framework() = default;

	protected:
		bool endRequest_;
		
		std::unique_ptr<AbstractSceneFactory> sceneFactory_;

		std::unique_ptr<Model> model_ = nullptr;
		std::unique_ptr<Sprite> sprite_ = nullptr;
		std::unique_ptr<PrimitiveDrawer> primitive_ = nullptr;
		std::unique_ptr<SkyBox> skybox_ = nullptr;
		std::unique_ptr<ParticleDrawer> particleDrawer_ = nullptr;
	};
}


