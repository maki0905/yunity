#pragma once

#include <string>

#include "IScene.h"
#include <memory>

namespace yunity {
	/// <summary>
	/// シーン工場
	/// </summary>
	class AbstractSceneFactory {
	public:
		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~AbstractSceneFactory() = default;
		/// <summary>
		/// シーン生成
		/// </summary>
		/// <param name="sceneName"></param>
		/// <returns></returns>
		virtual std::unique_ptr<IScene> CreateScene(const std::string& sceneName) = 0;

	};
}