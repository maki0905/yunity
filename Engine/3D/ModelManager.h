#pragma once


#include <string>
#include <unordered_map>
#include <wrl.h>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "Animation.h"
#include "Format.h"

/// <summary>
/// モデルマネージャー
/// </summary>
class ModelManager {
	struct Data {
		Model::ModelData modelData;
		Animation animation;
	};
public:
	//static Model::ModelData* Load(const std::string& fileName, const std::string format);

	static ModelManager* GetInstance();

	void Initialize();

	void Update();

	Model* CreateModel(Format format, const std::string& folderName, const std::string& fileName = "", ModelType modelType = kRigid);
	Model::ModelData& GetModelData(const std::string& modelName);
	Animation GetAnimation(Format format, const std::string& folderName, const std::string& fileName = "");

private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
private:
	void LoadInternal(const std::string& fileName, const std::string format);
	//Model::ModelData* LoadInternal(const std::string& fileName, const std::string format);
	Model::ModelData LoadModelFile(Format format, const std::string& folderName, const std::string& fileName = "");
	Model::Node ReadNode(aiNode* node);
	Model::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
private:
	//std::unordered_map<std::string, Model::ModelData> models_;
	std::unordered_map<std::string, Model::ModelData> modelDataStorage_;
	std::unordered_map<std::string, Animation> animationDataStorage_;
	//std::vector<Model*>
	std::vector<Model*> models_;
};