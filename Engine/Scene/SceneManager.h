#pragma once

#include <memory>

#include "SceneFactory.h"


class SceneManager {
public:

	static SceneManager* GetInstance();

	void Initialize();

	void Update();

	void DrawBack();
	void Draw3D();
	void DrawFront();

	// 次のシーン予約
	void ChangeScene(const std::string& sceneName);

private:
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;
private:
	// シーンファクトリー
	SceneFactory* sceneFactory_ = nullptr;

	// 今のシーン
	std::unique_ptr<IScene> scene_ = nullptr;
	//次のシーン
	IScene* nextScene_ = nullptr;

};