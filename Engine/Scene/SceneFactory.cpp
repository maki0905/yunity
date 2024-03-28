#include "SceneFactory.h"

#pragma region シーン
#include "TitleScene.h"
#include "StageScene.h"
#include "ClearScene.h"
#pragma endregion 

IScene* SceneFactory::CreateScene(const std::string& sceneName)
{
    // 次のシーンを生成
    IScene* newScene = nullptr;

    if (sceneName == "TITLE") {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAMESTAGE") {
        newScene = new StageScene();
    }
   

    return newScene;
}
