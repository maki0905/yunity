#pragma once


#include <string>
#include <unordered_map>
#include <wrl.h>
#include <memory>
#include <list>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "Animation.h"
#include "Format.h"

namespace yunity {
	/*
	* @brief モデルマネージャークラス
	*/
	class ModelManager {
		struct Data {
			Model::ModelData modelData;
			Animation animation;
		};
	public:

		/// <summary>
		/// インスタンス取得
		/// </summary>
		/// <returns></returns>
		static ModelManager* GetInstance();

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新
		/// </summary>
		void Update();

		/// <summary>
		/// リセット
		/// </summary>
		void Reset();

		/// <summary>
		/// 消去
		/// </summary>
		/// <param name="model"></param>
		void Take(Model* model);

		/// <summary>
		/// モデル生成
		/// </summary>
		/// <param name="format"></param>
		/// <param name="folderName"></param>
		/// <param name="fileName"></param>
		/// <param name="modelType"></param>
		/// <returns></returns>
		Model* CreateModel(Format format, const std::string& folderName, const std::string& fileName = "", ModelType modelType = kRigid);
		//std::unique_ptr<Model> CreateModel(Format format, const std::string& folderName, const std::string& fileName = "", ModelType modelType = kRigid);

		/// <summary>
		/// getter
		/// </summary>
		/// <returns></returns>

		Model::ModelData& GetModelData(const std::string& modelName);
		Animation GetAnimation(Format format, const std::string& folderName, const std::string& fileName = "");

	private:
		ModelManager() = default;
		~ModelManager() = default;
		ModelManager(const ModelManager&) = delete;
		ModelManager& operator=(const ModelManager&) = delete;
	private:
		void LoadInternal(const std::string& fileName, const std::string format);

		/// <summary>
		/// モデルファイル読み込み
		/// </summary>
		/// <param name="format"></param>
		/// <param name="folderName"></param>
		/// <param name="fileName"></param>
		/// <returns></returns>
		Model::ModelData LoadModelFile(Format format, const std::string& folderName, const std::string& fileName = "");

		/// <summary>
		/// ノード読み込み
		/// </summary>
		/// <param name="node"></param>
		/// <returns></returns>
		Model::Node ReadNode(aiNode* node);

		/// <summary>
		/// マテリアル読み込み
		/// </summary>
		/// <param name="directoryPath"></param>
		/// <param name="filename"></param>
		/// <returns></returns>
		Model::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	private:
		std::unordered_map<std::string, Model::ModelData> modelDataStorage_;
		std::unordered_map<std::string, Animation> animationDataStorage_;
		std::list<Model*> models_;
	};
}