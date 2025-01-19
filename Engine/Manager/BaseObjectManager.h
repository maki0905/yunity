#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <string>

#include "Object3D.h"
#include "Joint.h"
#include "SpringJoint.h"
#include "PulleyJoint.h"
#include "Camera.h"
#include "World.h"
#include "EventTrigger.h"
#include "LevelEditor.h"

namespace yunity {
	/*
	* @brief オブジェクトマネージャークラス
	*/
	class BaseObjectManager {
	protected:
		struct JointObject {
			Object3D* objA;
			Object3D* objB;
			JointData::BaseJointParameter jointData;
			JointData::SpringJointParmameter springJointData;
			JointData::PulleyJointParmameter pulleyJointData;
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
		virtual void Load(const std::string& objectFileName, Camera* camera, World* world, const std::string& jointFileName = "");


		void SetInitalizeData(const LevelData::ObjectData& objectData, Object3D* newObject, Camera* camera);

		void AddObject(std::unique_ptr<Object3D> newObject);

		/// <summary>
		/// 指定オブジェクト群
		/// </summary>
		/// <param name="fileName"></param>
		/// <param name="modelName"></param>
		/// <returns></returns>
		Vector3 GetPos(const std::string& modelName);

		std::vector<Object3D*> GetObj(const Tag& tag);

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

		/// <summary>
		/// 
		/// </summary>
		/// <param name="directionLight"></param>
		void SetDirectionLight(DirectionLight* directionLight);

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

		/// <summary>
		/// ジョイントを読み込む
		/// </summary>
		/// <param name="jointData"></param>
		void LoadJoint(const JointData& jointData);

		/// <summary>
		/// ジョイントオブジェクト収集
		/// </summary>
		/// <param name="objectData"></param>
		/// <param name="object"></param>
		void AddJointData(const LevelData::ObjectData& objectData, Object3D* object);

		/// <summary>
		/// ジョイント生成
		/// </summary>
		void CreateJoint();

		/// <summary>
		/// SpringJoint初期化
		/// </summary>
		/// <param name="joint"></param>
		void InitializeSpringJoint(const JointObject& joint);

		/// <summary>
		/// Spring1用のLine初期化
		/// </summary>
		/// <param name="joint"></param>
		/// <param name="camera"></param>
		void InitializeSpringLine(const JointObject& joint);

		/// <summary>
		/// PulleyJoint初期化
		/// </summary>
		/// <param name="joint"></param>
		void InitializePulleyJoint(const JointObject& joint);

	protected:
		std::vector<std::unique_ptr<Object3D>> objects_;
		std::vector<std::unique_ptr<Joint>> joints_;
		World* world_;
		Camera* camera_;
		static const uint32_t jointNumber_ = 100;
		std::array<JointObject, jointNumber_> jointData_;
		std::vector<std::unique_ptr<SpringJointLine>> springLines_;
	};

}