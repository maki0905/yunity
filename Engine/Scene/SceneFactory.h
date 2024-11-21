#pragma once

#include "AbstractSceneFactory.h"

namespace yunity {
	class SceneFactory : public AbstractSceneFactory {
	public:
		/// <summary>
		/// シーン生成
		/// </summary>
		/// <param name="sceneName">シーン名</param>
		/// <returns>生成したシーン</returns>
		IScene* CreateScene(const std::string& sceneName) override;

	};
}
