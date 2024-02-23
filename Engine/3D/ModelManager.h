#pragma once


#include <string>
#include <unordered_map>
#include <wrl.h>
#include <memory>

#include "Model.h"

/// <summary>
/// モデルマネージャー
/// </summary>
class ModelManager {
public:
	static Model::ModelData* Load(const std::string& fileName);

	static ModelManager* GetInstance();

	void Initialize();

private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;
private:
	Model::ModelData* LoadInternal(const std::string& fileName);
	Model::ModelData LoadObjFile(const std::string& fileName);
	Model::MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
private:
	Model* model = nullptr;
	std::unordered_map<std::string, Model::ModelData> models_;
};