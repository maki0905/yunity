#include "SceneFactory.h"

#pragma region シーン
#include "TitleScene.h"
#include "SelectScene.h"
#include "StageScene.h"
#include "ClearScene.h"
#pragma endregion 

std::unique_ptr<yunity::IScene> yunity::SceneFactory::CreateScene(const std::string& sceneName)
{
    if (sceneName == "TITLE") {
        return std::make_unique<TitleScene>();
    }
    else if (sceneName == "SELECT") {
        return std::make_unique<SelectScene>();
    }
    else if (sceneName == "GAMESTAGE") {
        return std::make_unique<StageScene>();
    }
    else if (sceneName == "CLEAR") {
        return std::make_unique<ClearScene>();
    }

    return nullptr;
}
