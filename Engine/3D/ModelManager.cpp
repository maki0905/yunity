#include "ModelManager.h"

#include <cassert>
#include <fstream>
#include <sstream>

Model::ModelData* ModelManager::Load(const std::string& fileName)
{
	return ModelManager::GetInstance()->LoadInternal(fileName);
}

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::Initialize()
{
	models_.clear();
	model = new Model();
	model->StaticInitialize();
}

Model::ModelData* ModelManager::LoadInternal(const std::string& fileName)
{

	auto itr = models_.find(fileName);
	if (itr == models_.end()) {
		models_[fileName] = LoadObjFile(fileName);
	}
	return &models_.at(fileName);
	
}

Model::ModelData ModelManager::LoadObjFile(const std::string& fileName)
{
	Model::ModelData modelData;

	// 1. 変数の宣言
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file("Resources/Models/" + fileName + "/" + fileName + ".obj");
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "v") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "mtllib") {
			std::string materialFilename;
			s >> materialFilename;
			modelData.material = LoadMaterialTemplateFile(fileName, materialFilename);
		}
		else if (identifier == "f") {
			Model::VertexData triangle[3];
			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;
				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/'); // 区切りでインデックスを読んでいく
					elementIndices[element] = std::stoi(index);
				}
				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
				Vector4 position = positions[elementIndices[0] - 1];
				position.z *= -1.0f;
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				texcoord.y = 1.0f - texcoord.y;
				Vector3 normal = normals[elementIndices[2] - 1];
				normal.z *= -1.0f;
				triangle[faceVertex] = { position, texcoord, normal };
			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);
		}
	}
	return modelData;
}

Model::MaterialData ModelManager::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	// 1. 変数の宣言
	Model::MaterialData materialData;
	std::string line;
	// 2. ファイルを開く
	std::ifstream file("Resources/Models/" + directoryPath + "/" + filename);
	assert(file.is_open());
	// 3. 実際にファイルを読み、MaterialDataを構築していく
	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifierに応じた処理
		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			// 連結してファイルパスにする
			materialData.textureFilePath = "Models/" + directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}
