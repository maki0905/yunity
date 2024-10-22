#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "Object3D.h"
#include "Camera.h"
#include "World.h"
#include "MoveFloor.h"
#include "EventTrigger.h"
#include "TV.h"
#include "Coin.h"
#include "GiftWall.h"

/*
* @brief オブジェクトマネージャークラス
*/
class ObjectManager {
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	//static ObjectManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="fileName"></param>
	void Update(const std::string& fileName);
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="fileName"></param>
	void Draw(const std::string& fileName);
	
	/// <summary>
	/// マップ読み込み
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="camera"></param>
	/// <param name="world"></param>
	void Load(const std::string& fileName, Camera* camera, World* world);
	
	/// <summary>
	/// 指定オブジェクト群取得
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns></returns>
	//std::vector<std::unique_ptr<Object3D>> GetObjects(const std::string& fileName);
	//std::vector<Object3D*> GetObj(const std::string& fileName, const std::string& modelName);
	
	/// <summary>
	/// 指定オブジェクト群
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="modelName"></param>
	/// <returns></returns>
	//Vector3 GetPos(const std::string& fileName, const std::string& modelName);
	
	/// <summary>
	/// クリア処理
	/// </summary>
	/// <param name="fileName"></param>
	void Clear(const std::string& fileName);

	/// <summary>
	/// リセット処理
	/// </summary>
	/// <param name="fileName"></param>
	void Reset(const std::string& fileName);
	
	/// <summary>
	/// ディレクションライト設定
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="directionalLight"></param>
	void SetDirectionalLight(const std::string& fileName, Model::DirectionalLight directionalLight);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="onOff"></param>
	void SetEnableLighting(const std::string& fileName, bool onOff);
	
	/// <summary>
	/// アクティブ設定
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="index"></param>
	/// <param name="active"></param>
	//void SetActive(const std::string& fileName, uint32_t index, bool active);
private:
	/*ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager&) = delete;
	const ObjectManager& operator=(const ObjectManager&) = delete;*/
private:
	//std::vector<Object3D*> objectVector_;
	/*std::unordered_map<std::string, std::vector<std::unique_ptr<Object3D>>>objects_;
	
	std::unordered_map<std::string, std::vector<bool>>activeObjects_;*/
	//std::map<std::string, Object3D*> objects_;

	std::vector<std::unique_ptr<Object3D>> objects_;
};