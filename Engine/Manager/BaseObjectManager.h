#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "Object3D.h"
#include "Joint.h"
#include "SpringJoint.h"
#include "PulleyJoint.h"
#include "Camera.h"
#include "World.h"
#include "MoveFloor.h"
#include "EventTrigger.h"
#include "TV.h"
#include "Coin.h"
#include "LevelEditor.h"

namespace yunity {
	/*
	* @brief オブジェクトマネージャークラス
	*/
	class BaseObjectManager {
	protected:
		struct JointData {
			Object3D* objA;
			Object3D* objB;
			LevelData::ObjectData objectDataA;
			LevelData::ObjectData objectDataB;
		};

		struct SpringJointLine {
			Object3D* objA;
			Object3D* objB;
			std::unique_ptr<PrimitiveDrawer> line;
			bool isActive_;
		};

	public:
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="camera">カメラ</param>
		void Initialize(Camera* camera = nullptr);

		/// <summary>
		/// 更新
		/// </summary>
		/// <param name="fileName"></param>
		void Update(/*const std::string& fileName*/);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="fileName"></param>
		void Draw(/*const std::string& fileName*/);

		/// <summary>
		/// マップ読み込み
		/// </summary>
		/// <param name="fileName"></param>
		/// <param name="camera"></param>
		/// <param name="world"></param>
		virtual void Load(const std::string& fileName, Camera* camera, World* world);

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
		Vector3 GetPos(const std::string& modelName);

		/// <summary>
		/// クリア処理
		/// </summary>
		/// <param name="fileName"></param>
		void Clear(/*const std::string& fileName*/);

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
		void SetDirectionalLight(/*const std::string& fileName, */Model::DirectionalLight directionalLight);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="fileName"></param>
		/// <param name="onOff"></param>
		void SetEnableLighting(/*const std::string& fileName,*/ bool onOff);

		/// <summary>
		/// アクティブ設定
		/// </summary>
		/// <param name="fileName"></param>
		/// <param name="index"></param>
		/// <param name="active"></param>
		//void SetActive(const std::string& fileName, uint32_t index, bool active);

	protected:
		/// <summary>
		/// 基本オブジェクト生成
		/// </summary>
		void CreateBasicObject(const LevelData::ObjectData& objectData, Camera* camera, World* world);

		/// <summary>
		/// 共通データ初期化
		/// </summary>
		/// <param name="objectData"></param>
		/// <param name="newObject"></param>
		void InitializeCommon(const LevelData::ObjectData& objectData, Object3D* newObject);

		/// <summary>
		/// コライダーデータ初期化
		/// </summary>
		/// <param name="objectData"></param>
		/// <param name="newObject"></param>
		void InitializeCollider(const LevelData::ObjectData& objectData, Object3D* newObject);

		/// <summary>
		/// 物理データ初期化
		/// </summary>
		/// <param name="objectData"></param>
		/// <param name="newObject"></param>
		void InitializePhysics(const LevelData::ObjectData& objectData, Object3D* newObject);

	protected:
		std::vector<std::unique_ptr<Object3D>> objects_;
		std::vector<std::unique_ptr<Joint>> joints_;
		World* world_;
		static const uint32_t jointNumber_ = 100;
		std::array<JointData, jointNumber_> jointData_;
		std::array<SpringJointLine, jointNumber_> springLines_;
	};

}