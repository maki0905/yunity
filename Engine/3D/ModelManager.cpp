#include "ModelManager.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "MathFunction.h"

//Model::ModelData* ModelManager::Load(const std::string& fileName, const std::string format)
//{
//	return ModelManager::GetInstance()->LoadInternal(fileName, format);
//}

ModelManager* ModelManager::GetInstance()
{
	static ModelManager instance;
	return &instance;
}

void ModelManager::Initialize()
{
	modelDataStorage_.clear();
	animationDataStorage_.clear();
	models_.clear();
}

void ModelManager::Finalize()
{
	/*for (auto& model : models_) {
		delete model;
	}*/
	//models_.clear();

}

void ModelManager::Update()
{
	for (auto& model : models_) {
		if (model->IsAnimation()) {
			//model->PlayingAnimation();
			model->ApplyAnimation();
		}
	}
}

void ModelManager::Reset()
{
	models_.clear();
}

Model* ModelManager::CreateModel(Format format, const std::string& folderName, const std::string& fileName, ModelType modelType)
{
	/*std::unique_ptr<Model> model;
	model = std::make_unique<Model>();*/
	/*std::unique_ptr<Model> model = std::make_unique<Model>();*/
	Model* model = new Model();
	//LoadInternal(fileName, format);
	//std::string path = folderName + "/" + fileName;
	std::string path = folderName;
	if (fileName.size() != 0) {
		path = path + "/" + fileName;
	}
	auto itr = modelDataStorage_.find(path);
	if (itr == modelDataStorage_.end()) {
		modelDataStorage_[path] = LoadModelFile(format, folderName, fileName);
	}
	model->Initialize(path, modelType, modelDataStorage_[path]);
	//Model* modePtr = model.get();
	models_.emplace_back(model);
	return models_.back();
}

//std::unique_ptr<Model> ModelManager::CreateModel(Format format, const std::string& folderName, const std::string& fileName, ModelType modelType)
//{
//	/*std::unique_ptr<Model> model;
//	model = std::make_unique<Model>();*/
//	std::unique_ptr<Model> model = std::make_unique<Model>();
//	//Model* model = new Model();
//	//LoadInternal(fileName, format);
//	//std::string path = folderName + "/" + fileName;
//	std::string path = folderName;
//	if (fileName.size() != 0) {
//		path = path + "/" + fileName;
//	}
//	auto itr = modelDataStorage_.find(path);
//	if (itr == modelDataStorage_.end()) {
//		modelDataStorage_[path] = LoadModelFile(format, folderName, fileName);
//	}
//	model->Initialize(path, modelType, modelDataStorage_[path]);
//	Model* modePtr = model.get();
//	models_.emplace_back(std::move(model));
//	return models_.back().get();
//}

Model::ModelData& ModelManager::GetModelData(const std::string& modelName)
{
	Model::ModelData modelData;
	std::string path = modelName;
	modelData = modelDataStorage_[path];
	return modelData;
}

Animation ModelManager::GetAnimation(Format format, const std::string& folderName, const std::string& fileName)
{
	std::string path = folderName + "/" + fileName;
	auto itr = animationDataStorage_.find(path);
	if (itr == animationDataStorage_.end()) {
		animationDataStorage_[path] = LoadAnimationFile(format, folderName, fileName);
	}
	return animationDataStorage_[path];
}

Model::ModelData ModelManager::LoadModelFile(Format format, const std::string& folderName, const std::string& fileName)
{
	Model::ModelData modelData;

	Assimp::Importer importer;
	std::string directoryPath = "Resources/Models/";
	std::string filePath = directoryPath + folderName + "/" + folderName;
	if (fileName.size()) {
		filePath = directoryPath + folderName + "/" + fileName;
	}

	filePath = AddToFormat(filePath, format);

	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());        // 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応
		modelData.vertices.resize(mesh->mNumVertices); // 最初に頂点数分のメモリを確保しておく

		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			aiVector3D& position = mesh->mVertices[vertexIndex];
			aiVector3D& normal = mesh->mNormals[vertexIndex];
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
			// 右手系->左手系
			modelData.vertices[vertexIndex].position = { -position.x, position.y, position.z, 1.0f };
			modelData.vertices[vertexIndex].normal = { -normal.x, normal.y, normal.z };
			modelData.vertices[vertexIndex].texcoord = { texcoord.x, texcoord.y };
		}
		// Indexの解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3);
			modelData.indices.push_back(face.mIndices[0]);
			modelData.indices.push_back(face.mIndices[1]);
			modelData.indices.push_back(face.mIndices[2]);
		}
		// SkinCluster構築用データ取得
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			aiBone* bone = mesh->mBones[boneIndex];
			std::string jointName = bone->mName.C_Str();
			Model::JointWeightData& jointWeightData = modelData.skinClusterData[jointName];

			aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse(); // BindPoseMatrixに戻す
			aiVector3D scale;
			aiQuaternion rotate;
			aiVector3D translate;
			bindPoseMatrixAssimp.Decompose(scale, rotate, translate); // 成分を抽出
			// 左手系のBindPoseMatrixを作る
			Matrix4x4 bindPoseMatrix = MakeAffineMatrix(Vector3(scale.x, scale.y, scale.z), Quaternion(rotate.x, -rotate.y, -rotate.z, rotate.w), Vector3(-translate.x, translate.y, translate.z)); // 
			// InverseBindPoseMatrixにする
			jointWeightData.inverseBindPoseMatrix = Inverse(bindPoseMatrix);

			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; ++weightIndex) {
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
			}
		}
	}

	modelData.rootNode = ReadNode(scene->mRootNode);

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = "Models/" + folderName + "/" + textureFilePath.C_Str();

		}
	}

	return modelData;
}

Model::Node ModelManager::ReadNode(aiNode* node)
{
	Model::Node result;
	aiVector3D scale;
	aiVector3D translate;
	aiQuaternion rotate;

	node->mTransformation.Decompose(scale, rotate, translate); // assimpの行列からSRTを抽出する関数を利用
	result.transform.scale = { scale.x, scale.y, scale.z }; // scaleはそのまま 
	result.transform.rotate = { rotate.x, -rotate.y, -rotate.z, rotate.w }; // x軸を反転、さらに回転方向が逆なので軸を反転させる
	result.transform.translate = { -translate.x, translate.y, translate.z }; // x軸を反転
	result.localMatrix = MakeAffineMatrix(result.transform.scale, result.transform.rotate, result.transform.translate);
	/*aiMatrix4x4 aiLocalMatrix = node->mTransformation;
	aiLocalMatrix.Transpose();

	for (uint32_t i = 0; i < 4; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			result.localMatrix.m[i][j] = aiLocalMatrix[i][j];
		}
	}*/

	result.name = node->mName.C_Str();
	result.children.resize(node->mNumChildren);
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex) {
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	return result;
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
