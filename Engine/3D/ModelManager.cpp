#include "ModelManager.h"

#include <cassert>
#include <fstream>
#include <sstream>

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
	dataStorage_.clear();
	models_.clear();
}

void ModelManager::Update()
{
	for (auto& model : models_) {
		if (model->IsAnimation()) {
			model->PlayingAnimation();
		}
	}
}

Model* ModelManager::CreateModel(const std::string& fileName, const std::string format, bool have)
{
	Model* model = new Model();
	//LoadInternal(fileName, format);
	auto itr = dataStorage_.find(fileName);
	if (itr == dataStorage_.end()) {
		dataStorage_[fileName].modelData = LoadModelFile(fileName, format);
	}
	if (have) {
		dataStorage_[fileName].animation = LoadAnimationFile(fileName, format);
	}
	model->Initialize(dataStorage_[fileName].modelData, dataStorage_[fileName].animation);
	models_.emplace_back(model);
	return model;
}

//Model::ModelData* ModelManager::LoadInternal(const std::string& fileName, const std::string format)
//{
//
//	auto itr = models_.find(fileName);
//	if (itr == models_.end()) {
//		models_[fileName].modelData = LoadModelFile(fileName, format);
//	}
//	return &models_.at(fileName).modelData;
//	
//}

//Model::ModelData ModelManager::LoadObjFile(const std::string& fileName)
//{
//	Model::ModelData modelData;
//
//	// 1. 変数の宣言
//	std::vector<Vector4> positions;
//	std::vector<Vector3> normals;
//	std::vector<Vector2> texcoords;
//	std::string line;
//
//	std::ifstream file("Resources/Models/" + fileName + "/" + fileName + ".obj");
//	assert(file.is_open());
//
//	while (std::getline(file, line)) {
//		std::string identifier;
//		std::istringstream s(line);
//		s >> identifier;
//
//		if (identifier == "v") {
//			Vector4 position;
//			s >> position.x >> position.y >> position.z;
//			position.w = 1.0f;
//			positions.push_back(position);
//		}
//		else if (identifier == "vt") {
//			Vector2 texcoord;
//			s >> texcoord.x >> texcoord.y;
//			texcoords.push_back(texcoord);
//		}
//		else if (identifier == "vn") {
//			Vector3 normal;
//			s >> normal.x >> normal.y >> normal.z;
//			normals.push_back(normal);
//		}
//		else if (identifier == "mtllib") {
//			std::string materialFilename;
//			s >> materialFilename;
//			modelData.material = LoadMaterialTemplateFile(fileName, materialFilename);
//		}
//		else if (identifier == "f") {
//			Model::VertexData triangle[3];
//			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
//				std::string vertexDefinition;
//				s >> vertexDefinition;
//				std::istringstream v(vertexDefinition);
//				uint32_t elementIndices[3];
//				for (int32_t element = 0; element < 3; ++element) {
//					std::string index;
//					std::getline(v, index, '/'); // 区切りでインデックスを読んでいく
//					elementIndices[element] = std::stoi(index);
//				}
//				// 要素へのIndexから、実際の要素の値を取得して、頂点を構築する
//				Vector4 position = positions[elementIndices[0] - 1];
//				position.z *= -1.0f;
//				Vector2 texcoord = texcoords[elementIndices[1] - 1];
//				texcoord.y = 1.0f - texcoord.y;
//				Vector3 normal = normals[elementIndices[2] - 1];
//				normal.z *= -1.0f;
//				triangle[faceVertex] = { position, texcoord, normal };
//			}
//			modelData.vertices.push_back(triangle[2]);
//			modelData.vertices.push_back(triangle[1]);
//			modelData.vertices.push_back(triangle[0]);
//		}
//	}
//	return modelData;
//}

void ModelManager::LoadInternal(const std::string& fileName, const std::string format)
{
	auto itr = dataStorage_.find(fileName);
		if (itr == dataStorage_.end()) {
			dataStorage_[fileName].modelData = LoadModelFile(fileName, format);
			dataStorage_[fileName].animation = LoadAnimationFile(fileName, format);
		}
}

Model::ModelData ModelManager::LoadModelFile(const std::string& fileName, const std::string format)
{
	Model::ModelData modelData;

	Assimp::Importer importer;
	std::string directoryPath = "Resources/Models/" + fileName + "/";
	std::string filePath = directoryPath + fileName + "." + format;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	assert(scene->HasMeshes()); // メッシュがないのは対応しない

	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		aiMesh* mesh = scene->mMeshes[meshIndex];
		assert(mesh->HasNormals());        // 法線がないMeshは今回は非対応
		assert(mesh->HasTextureCoords(0)); // TexcoordがないMeshは今回は非対応

		// ここからFaceの解析
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			aiFace& face = mesh->mFaces[faceIndex];
			assert(face.mNumIndices == 3); // 三角形のみサポート

			// assimpではaiProcess_Triangulateオプションを指定することで、四角形以上のポリゴンを三角形に自動分割できる

			// ここからVertexの解析
			for (uint32_t element = 0; element < face.mNumIndices; ++element) {
				uint32_t vertexIndex = face.mIndices[element];
				aiVector3D& position = mesh->mVertices[vertexIndex];
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
				Model::VertexData vertex;
				vertex.position = { position.x, position.y, position.z, 1.0f };
				vertex.normal = { normal.x, normal.y, normal.z };
				vertex.texcoord = { texcoord.x, texcoord.y };
				// aiProcess_MakeLeftHandedはz*=-1で、右手->左手に変換するので手動で対処
				vertex.position.x *= -1.0f;
				vertex.normal.x *= -1.0f;
				modelData.vertices.push_back(vertex);
			}
		}

	}

	modelData.rootNode = ReadNode(scene->mRootNode);

	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
		aiMaterial* material = scene->mMaterials[materialIndex];
		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			aiString textureFilePath;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
			modelData.material.textureFilePath = "Models/" + fileName + "/" + textureFilePath.C_Str();
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
