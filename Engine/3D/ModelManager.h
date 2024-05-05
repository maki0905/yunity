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

	Model* CreateModel(const std::string& fileName, const std::string format, bool have);

private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
private:
	void LoadInternal(const std::string& fileName, const std::string format);
	//Model::ModelData* LoadInternal(const std::string& fileName, const std::string format);
	Model::ModelData LoadModelFile(const std::string& fileName, const std::string format);
	Model::Node ReadNode(aiNode* node);
	Model::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
private:
	//std::unordered_map<std::string, Model::ModelData> models_;
	std::unordered_map<std::string, Data> dataStorage_;
	std::vector<Model*> models_;
};