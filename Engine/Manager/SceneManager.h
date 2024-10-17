#pragma once

#include <memory>

#include "SceneFactory.h"

/*
* @brief シーンマネージャークラス
*/
class SceneManager {
public:

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static SceneManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 背景描画
	/// </summary>
	void DrawBack();
	
	/// <summary>
	/// 3Dオブジェクト描画
	/// </summary>
	void Draw3D();
	
	/// <summary>
	/// 前面描画
	/// </summary>
	void DrawFront();

	/// <summary>
	/// 次のシーン予約
	/// </summary>
	/// <param name="sceneName"></param>
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