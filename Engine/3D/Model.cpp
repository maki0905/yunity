#include "Model.h"

#include <cassert>
#include <fstream>
#include <sstream>

#include "Device.h"
#include "DirectXCore.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "ShaderCompiler.h"
#include "ModelManager.h"

#include "RootBindingsCommon.h"
#include "ModelManager.h"

ID3D12GraphicsCommandList* yunity::Model::commandList_ = nullptr;
yunity::PipelineType yunity::Model::pipelineType_ = yunity::PipelineType::kCount;


void yunity::Model::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(commandList_ == nullptr);
	commandList_ = commandList;
}

void yunity::Model::PreDraw(ID3D12GraphicsCommandList* commandList, const PipelineType& pipelineType)
{
	assert(commandList_ == nullptr);
	commandList_ = commandList;
	pipelineType_ = pipelineType;
}

void yunity::Model::PostDraw()
{
	commandList_ = nullptr;
	if (pipelineType_ != PipelineType::kCount) {
		pipelineType_ = PipelineType::kCount;
	}
}

yunity::Model::~Model()
{
	ModelManager::GetInstance()->Take(this);
}


void yunity::Model::Initialize(const std::string& name, const ModelType& modelType, const ModelData& modelData)
{
	// 共通
	modelName_ = name;
	modelType_ = modelType;
	modelData_ = modelData;
	CreateMesh();
	CreateIndex();
	InitializeDirectionalLight();
	InitializeMaterial();
	animationNames_.clear();
	animations_.clear();
	switch (modelType_)
	{
	case kRigid:
		break;
	case kKeyframe:
		InitializeNode();
		break;
	case kSkin:
		skeleton_ = CreateSkelton(modelData_.rootNode);
		SkeletonUpdate();
		skinCluster_ = CreateSkinCluster();
		SkinClusterUpdate();
		break;
	}
}

void yunity::Model::Draw(const WorldTransform& worldTransform, uint32_t textureHandle)
{
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	
	if (modelType_ == ModelType::kSkin) {
		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, PipelineType::kSkinning, blendModeType_);
		commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点バッファの設定
		D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
			vertexBufferView_,               // VertexDataのVBV
			skinCluster_.influenceBufferView // InfluenceのVBV
		};
		// 配列を渡す(開始Slot番号、使用Slot数、VBV配列へのポインタである)
		commandList_->IASetVertexBuffers(0, 2, vbvs);

		// デスクリプタヒープの配列をセットするコマンド
		ID3D12DescriptorHeap* ppHeaps[] = { DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV)->GetHeapPointer() };
		commandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		commandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(SkinningRootBindings::kMatrixPalette), skinCluster_.paletteSrvHandle.second);
		uint32_t textureCubeHandle_ = TextureManager::Load("rostock_laage_airport_4k.dds");
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(SkinningRootBindings::kEnvironmentMap), textureCubeHandle_);
	}
	else {

		
		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, PipelineType::kObject3d, blendModeType_);
		commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点バッファの設定
		commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
		//uint32_t textureCubeHandle_ = TextureManager::Load("rostock_laage_airport_4k.dds");
		//TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(Object3dRootBindings::kEnvironmentMap), textureCubeHandle_);
	}

	if (pipelineType_ != PipelineType::kCount) {
		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, pipelineType_, blendModeType_);
	}

	//インデックスバッファの設定
	commandList_->IASetIndexBuffer(&indexBufferView_);

	if (pipelineType_ == PipelineType::kShadowMap) {
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kViewProjection), lightVPBuff_->GetGPUVirtualAddress());

		if (materialData_->enableLighting) {
			if (modelType_ == ModelType::kSkin) {
				commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), UINT(skeleton_.joints.size()), 0, 0, 0);
			}
			else {
				commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), 1, 0, 0, 0);
			}
		}
	}
	else {
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kMaterial), materialResource_->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kLight), directionalLightResource_->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kCamera), camera_->GetCameraForGPU()->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kPointLight), pointLightResource_->GetGPUVirtualAddress());

		// SRVをセット
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(Object3dRootBindings::kTexture), textureHandle);

		if (modelType_ == ModelType::kSkin) {
			commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), UINT(skeleton_.joints.size()), 0, 0, 0);
		}
		else {
			commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), 1, 0, 0, 0);
		}
	}
}

void yunity::Model::Draw(const WorldTransform& worldTransform)
{
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	lightVP->view = camera_->GetViewMatrix();

	if (modelType_ == ModelType::kSkin) {
		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, PipelineType::kSkinning, blendModeType_);
		commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点バッファの設定
		D3D12_VERTEX_BUFFER_VIEW vbvs[2] = {
			vertexBufferView_,               // VertexDataのVBV
			skinCluster_.influenceBufferView // InfluenceのVBV
		};
		// 配列を渡す(開始Slot番号、使用Slot数、VBV配列へのポインタである)
		commandList_->IASetVertexBuffers(0, 2, vbvs);

		// デスクリプタヒープの配列をセットするコマンド
		ID3D12DescriptorHeap* ppHeaps[] = { DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV)->GetHeapPointer() };
		commandList_->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		commandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(SkinningRootBindings::kMatrixPalette), skinCluster_.paletteSrvHandle.second);
		uint32_t textureCubeHandle_ = TextureManager::Load("rostock_laage_airport_4k.dds");
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(SkinningRootBindings::kEnvironmentMap), textureCubeHandle_);
	}
	else {
		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, PipelineType::kObject3d, blendModeType_);
		if (materialData_->enableLighting) {
			GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, PipelineType::kObject3dShadowMap, blendModeType_);
		}
		commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// 頂点バッファの設定
		commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
		//uint32_t textureCubeHandle_ = TextureManager::Load("rostock_laage_airport_4k.dds");
		//TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(Object3dRootBindings::kEnvironmentMap), textureCubeHandle_);
	}

	if (pipelineType_ != PipelineType::kCount) {
		GraphicsPipelineManager::GetInstance()->SetCommandList(commandList_, pipelineType_, blendModeType_);
	}

	//インデックスバッファの設定
	commandList_->IASetIndexBuffer(&indexBufferView_);

	if (pipelineType_ == PipelineType::kShadowMap) {
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kViewProjection), lightVPBuff_->GetGPUVirtualAddress());
		//commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());

		if (materialData_->enableLighting) {
			if (modelType_ == ModelType::kSkin) {
				commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), UINT(skeleton_.joints.size()), 0, 0, 0);
			}
			else {
				commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), 1, 0, 0, 0);
			}
		}
	}
	else {
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kMaterial), materialResource_->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kLight), directionalLightResource_->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kCamera), camera_->GetCameraForGPU()->GetGPUVirtualAddress());
		commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dRootBindings::kPointLight), pointLightResource_->GetGPUVirtualAddress());

		if (materialData_->enableLighting) {
			commandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(Object3dShadowMapRootBindings::kShadowMap), DirectXCore::GetInstance()->GetShdowHandle());
			commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dShadowMapRootBindings::kLightViewProjection), lightVPBuff_->GetGPUVirtualAddress());
			//commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(Object3dShadowMapRootBindings::kLightViewProjection), camera_->GetConstBuff()->GetGPUVirtualAddress());
		}

		// SRVをセット
		if (textureHandle_ != TextureManager::Load(modelData_.material.textureFilePath)) {
			textureHandle_ = TextureManager::Load(modelData_.material.textureFilePath);
		}
		TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(Object3dRootBindings::kTexture), textureHandle_);

		if (modelType_ == ModelType::kSkin) {
			commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), UINT(skeleton_.joints.size()), 0, 0, 0);
		}
		else {
			commandList_->DrawIndexedInstanced((UINT)modelData_.indices.size(), 1, 0, 0, 0);
		}
	}
	
}

void yunity::Model::SetPointLight(const PointLight& pointLight)
{
	pointLightData_->color = pointLight.color;
	pointLightData_->position = pointLight.position;
	pointLightData_->intensity = pointLight.intensity;
}

void yunity::Model::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	directionalLightData_->color = directionalLight.color;
	directionalLightData_->direction = directionalLight.direction;
	directionalLightData_->intensity = directionalLight.intensity;
	EulerTransform transform = { .scale = {1.0f, 1.0f, 1.0f}, .rotate = {0.0f, 0.0f, 0.0f}, .translate = {0.0f, 5.0f, 0.0f}};
	Matrix4x4 worldMatrix = MakeAffineMatrix(transform);
	lightVP->view = Inverse(worldMatrix);
	lightVP->view = MakeMatrixLookAt({ 0.0f, 10.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
	//lightVP->projection = MakeOrthographicMatrix(-10.0f, 10.0f, 10.0f, -5.0f, 1.0f, 10.0f);
	//float yscale = 1.0f / (1280.0f / 720.0f);
	lightVP->projection = MakeOrthographicMatrix(40.0f,40.0f, 1.0f, 20.0f);
	//lightVP->projection = MakePerspectiveFovMatrix(45.0f * DegToRad(), 1.0f, 1.0f, 15.0f);
}

void yunity::Model::SetAnimation(std::string name, const Animation& animation, AnimationCommon::AnimationMode mode)
{
	std::string path = name;
	auto itr = animations_.find(path);
	if (itr == animations_.end()) {
		animationNames_.emplace_back(name);
		animations_[path].animation = animation;
		animations_[path].animationCommon.state = mode;
	}
}

void yunity::Model::PlayAnimation(std::string name, AnimationCommon::AnimationMode mode)
{
	isAnimation_ = true;
	if (animations_[name].animationCommon.state == AnimationCommon::kStopped) {
		animations_[name].animationCommon.state = mode;
	}
}

void yunity::Model::StopAnimation()
{
	isAnimation_ = false;

	

	for (auto& name : animationNames_) {
		if (animations_[name].animationCommon.state == AnimationCommon::kStopped) {
			continue;
		}
		animations_[name].animationCommon.state = AnimationCommon::kStopped;
	}
}

void yunity::Model::StopAnimation(std::string name)
{
	isAnimation_ = true;
	animations_[name].animationCommon.state = AnimationCommon::kStopped;
}

void yunity::Model::TransitionAnimation(const std::string& from, const std::string& to, float time)
{
	isTransition_ = true;
	from_ = from;
	to_ = to;
	transitionSecound_ = 1.0f / (60.0f * time);
	transitionTime_ = 0.0f;
}


void yunity::Model::SkeletonUpdate()
{
	// すべてのJointを更新。親が若いので通常ループで処理可能になっている
	for (Joint& joint : skeleton_.joints) {
		joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		if (joint.parent) { // 親がいれば親の行列を掛ける
			joint.skeletonSpaceMatrix = Multiply(joint.localMatrix, skeleton_.joints[*joint.parent].skeletonSpaceMatrix);
		}
		else { // 親がいないのでlocalMatrixとskeletonSpaceMatrixは一致する
			joint.skeletonSpaceMatrix = joint.localMatrix;
		}
	}
}

void yunity::Model::ApplyAnimation()
{
	if (isTransition_) {
		animations_[from_].animationCommon.time += 1.0f / 60.0f;
		animations_[from_].animationCommon.time = std::fmod(animations_[from_].animationCommon.time, animations_[from_].animation.duration);
		animations_[to_].animationCommon.time += 1.0f / 60.0f;
		animations_[to_].animationCommon.time = std::fmod(animations_[to_].animationCommon.time, animations_[to_].animation.duration);
		for (Joint& joint : skeleton_.joints) {
			// 対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文
			if (auto itA = animations_[from_].animation.nodeAnimations.find(joint.name); itA != animations_[from_].animation.nodeAnimations.end()) {
				if (auto itB = animations_[to_].animation.nodeAnimations.find(joint.name); itB != animations_[to_].animation.nodeAnimations.end()) {
					const NodeAnimation& rootNodeAnimationA = (*itA).second;
					Vector3 translateA = CalculateValue(rootNodeAnimationA.translate, animations_[from_].animationCommon.time);
					Quaternion rotateA = CalculateQuaternion(rootNodeAnimationA.rotate, animations_[from_].animationCommon.time);
					Vector3 scaleA = Vector3(1.0f, 1.0f, 1.0f);
					if (rootNodeAnimationA.scale.size() != 0) {
						scaleA = CalculateValue(rootNodeAnimationA.scale, animations_[from_].animationCommon.time);
					}

					const NodeAnimation& rootNodeAnimationB = (*itB).second;
					Vector3 translateB = CalculateValue(rootNodeAnimationB.translate, animations_[to_].animationCommon.time);
					Quaternion rotateB = CalculateQuaternion(rootNodeAnimationB.rotate, animations_[to_].animationCommon.time);
					Vector3 scaleB = Vector3(1.0f, 1.0f, 1.0f);
					if (rootNodeAnimationB.scale.size() != 0) {
						scaleB = CalculateValue(rootNodeAnimationB.scale, animations_[to_].animationCommon.time);
					}


					joint.transform.translate = Lerp(translateA, translateB, transitionTime_);
					joint.transform.rotate = Slerp(rotateA, rotateB, transitionTime_);
					joint.transform.scale = { 1.0f, 1.0f, 1.0f };
					if (rootNodeAnimationA.scale.size() != 0 || rootNodeAnimationB.scale.size() != 0) {
						joint.transform.scale = Lerp(scaleA, scaleB, transitionTime_);
					}
				}
			}
		}

		// 現在の骨ごとのLocal情報を基にSkeletonSpaceの情報を更新
		SkeletonUpdate();
		// SkeletonSpaceの情報を基に、SkinClusterのMatrixPaletteを更新する
		SkinClusterUpdate();

		transitionTime_ += transitionSecound_;

		if (transitionTime_ > 1.0f) {
			animations_[from_].animationCommon.state = AnimationCommon::kStopped;
			animations_[to_].animationCommon.state = AnimationCommon::kLooping;
			isTransition_ = false;
		}


	}
	else {
		for (auto& name : animationNames_) {
			if (animations_[name].animationCommon.state == AnimationCommon::kStopped) {
				continue;
			}

			presentAnimation_ = name;

			animations_[name].animationCommon.time += 1.0f / 60.0f;

			if (animations_[name].animationCommon.state == AnimationCommon::kLooping) {
				animations_[name].animationCommon.time = std::fmod(animations_[name].animationCommon.time, animations_[name].animation.duration);

			}
			else if (animations_[name].animationCommon.time > animations_[name].animation.duration) {
				animations_[name].animationCommon.state = AnimationCommon::kStopped;
				animations_[name].animationCommon.time = 0.0f;
			}

			for (Joint& joint : skeleton_.joints) {
				// 対象のJointのAnimationがあれば、値の適用を行う。下記のif文はC++17から可能になった初期化付きif文
				if (auto it = animations_[name].animation.nodeAnimations.find(joint.name); it != animations_[name].animation.nodeAnimations.end()) {
					const NodeAnimation& rootNodeAnimation = (*it).second;
					joint.transform.translate = CalculateValue(rootNodeAnimation.translate, animations_[name].animationCommon.time);
					joint.transform.rotate = CalculateQuaternion(rootNodeAnimation.rotate, animations_[name].animationCommon.time);
					joint.transform.scale = { 1.0f, 1.0f, 1.0f };
					if (rootNodeAnimation.scale.size() != 0) {
						joint.transform.scale = CalculateValue(rootNodeAnimation.scale, animations_[name].animationCommon.time);
					}
				}
			}

			// 現在の骨ごとのLocal情報を基にSkeletonSpaceの情報を更新
			SkeletonUpdate();
			// SkeletonSpaceの情報を基に、SkinClusterのMatrixPaletteを更新する
			SkinClusterUpdate();


		}

	}
}

void yunity::Model::SkinClusterUpdate()
{
	for (size_t jointIndex = 0; jointIndex < skeleton_.joints.size(); ++jointIndex) {
		assert(jointIndex < skinCluster_.inverseBindPoseMatrices.size());
		skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix = Multiply(skinCluster_.inverseBindPoseMatrices[jointIndex], skeleton_.joints[jointIndex].skeletonSpaceMatrix);
		skinCluster_.mappedPalette[jointIndex].skeletonSpaceInverseTransposeMatrix = Transpose(Inverse(skinCluster_.mappedPalette[jointIndex].skeletonSpaceMatrix));
	}
}

void yunity::Model::CreateMesh()
{	
	// 頂点リソース
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size()); // 使用するリソースのサイズは頂点サイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_->Map(0, nullptr, (void**)&vertexData_); // 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData_.vertices.data(), sizeof(VertexData) * modelData_.vertices.size()); // 頂点データをリソースにコピー
	vertexResource_->Unmap(0, nullptr);
}

void yunity::Model::CreateIndex()
{
	// インデックスリソース
	indexResource_ = CreateBufferResource(sizeof(uint32_t) * modelData_.indices.size());
	// インデックスバッファビュー
	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = UINT(sizeof(uint32_t) * modelData_.indices.size());
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	// インデックスリソースにデータを書き込む
	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedIndex_));
	std::memcpy(mappedIndex_, modelData_.indices.data(), sizeof(uint32_t) * modelData_.indices.size());

}

void yunity::Model::InitializeDirectionalLight()
{
	// 
	directionalLightResource_ = CreateBufferResource(sizeof(DirectionalLight));
	directionalLightData_ = nullptr;
	directionalLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData_));
	// デフォルト値
	directionalLightData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLightData_->direction = Vector3(0.0f, -1.0f, 0.0f);
	directionalLightData_->intensity = 1.0f;

	pointLightResource_ = CreateBufferResource(sizeof(PointLight));
	pointLightData_ = nullptr;
	pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

	pointLightData_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	pointLightData_->position = { 0.0f, 2.0f, 0.0f };
	pointLightData_->intensity = 1.0f;

	lightVPBuff_ = CreateBufferResource(sizeof(ConstBufferDataViewProjection));
	lightVP = nullptr;
	lightVPBuff_->Map(0, nullptr, (void**)&lightVP);

}

void yunity::Model::InitializeMaterial()
{
	materialResource_ = CreateBufferResource(sizeof(MaterialData));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData_->enableLighting = 0;
	materialData_->shininess = 50.0f;
}

void yunity::Model::InitializeNode()
{
	nodeResource_ = CreateBufferResource(sizeof(Matrix4x4));
	nodeData_ = nullptr;
	nodeResource_->Map(0, nullptr, reinterpret_cast<void**>(&nodeData_));
	*nodeData_ = modelData_.rootNode.localMatrix;
	
}


Microsoft::WRL::ComPtr<ID3D12Resource> yunity::Model::CreateBufferResource(size_t sizeInBytes)
{
	HRESULT result = S_FALSE;
	// リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // uploadHeapを使う
	// リソースの設定
	D3D12_RESOURCE_DESC ResourceDesc{};
	// バッファリソース
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResourceDesc.Width = sizeInBytes; // リソースのサイズ
	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	result = Device::GetInstance()->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));
	return resource;
}

yunity::Model::Skeleton yunity::Model::CreateSkelton(const Node& rootNode)
{
	Skeleton skeleton;
	skeleton.root = CreateJoint(rootNode, {}, skeleton.joints);

	// 名前とindexのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skeleton.joints) {
		skeleton.jointMap.emplace(joint.name, joint.index);
	}



	return skeleton;
}

int32_t yunity::Model::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	Joint joint;
	joint.name = node.name;
	joint.localMatrix = node.localMatrix;
	joint.skeletonSpaceMatrix = MakeIdentity4x4();
	joint.transform = node.transform;
	joint.index = int32_t(joints.size()); // 現在要録されている数をIndexに
	joint.parent = parent;
	joints.push_back(joint);
	for (const Node& child : node.children) {
		// 子Jointを作成し、そのIndexを登録
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}
	// 自身のIndexを返す
	return joint.index;
}

yunity::Model::SkinCluster yunity::Model::CreateSkinCluster()
{
	SkinCluster skinCluster;
	// palette用のResourceを確保
	skinCluster.paletteResource = CreateBufferResource(sizeof(WellForGPU) * skeleton_.joints.size());
	WellForGPU* mappedPalette = nullptr;
	skinCluster.paletteResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedPalette));
	skinCluster.mappedPalette = { mappedPalette, skeleton_.joints.size() }; // spanを使ってアクセスするようにする
	DescriptorHandle srvDescriptorHandle = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV)->Alloc();
	skinCluster.paletteSrvHandle.first = srvDescriptorHandle.GetCPUHandle();
	skinCluster.paletteSrvHandle.second = srvDescriptorHandle.GetGPUHandle();

	// palette用のsrvを作成。StructuredBufferでアクセスできるようにする。
	D3D12_SHADER_RESOURCE_VIEW_DESC paletteSrvDesc{};
	paletteSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	paletteSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	paletteSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	paletteSrvDesc.Buffer.FirstElement = 0;
	paletteSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	paletteSrvDesc.Buffer.NumElements = UINT(skeleton_.joints.size());
	paletteSrvDesc.Buffer.StructureByteStride = sizeof(WellForGPU);
	Device::GetInstance()->GetDevice()->CreateShaderResourceView(skinCluster.paletteResource.Get(), &paletteSrvDesc, skinCluster.paletteSrvHandle.first);

	// influence用のResourceを確保。頂点ごとにinfluence情報を追加できるようにする
	skinCluster.influenceResource = CreateBufferResource(sizeof(VertexInfluence) * modelData_.vertices.size());
	VertexInfluence* mappedInfluence = nullptr;
	skinCluster.influenceResource->Map(0, nullptr, reinterpret_cast<void**>(&mappedInfluence));
	std::memset(mappedInfluence, 0, sizeof(VertexInfluence) * modelData_.vertices.size()); // 0埋め。weightを0にしておく。
	skinCluster.mappedInfluence = { mappedInfluence, modelData_.vertices.size() };

	// influence用のVBVを作成
	skinCluster.influenceBufferView.BufferLocation = skinCluster.influenceResource->GetGPUVirtualAddress();
	skinCluster.influenceBufferView.SizeInBytes = UINT(sizeof(VertexInfluence) * modelData_.vertices.size());
	skinCluster.influenceBufferView.StrideInBytes = sizeof(VertexInfluence);

	// InverseBindPoseMatrixを格納する場所を作成して、単位行列で埋める
	skinCluster.inverseBindPoseMatrices.resize(skeleton_.joints.size());
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), MakeIdentity4x4);

	// ModelDataのSkinCluster情報を解析してInfluenceの中身を埋める
	for (const auto& jointWeight : modelData_.skinClusterData) {
		auto it = skeleton_.jointMap.find(jointWeight.first); // jointWeight.firstはjoint名なので、skeletonに対象となるjointが含まれているか判断
		if (it == skeleton_.jointMap.end()) { // そんな名前のJointは存在しない。次に回す
			continue;
		}
		// (*it).secondにはjointのindexが入っているので、該当のindexのinverseBindPoseMatrixを代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			auto& currentInfluence = skinCluster.mappedInfluence[vertexWeight.vertexIndex]; // 該当のvertexIndexのinfluence情報を参照しておく
			for (uint32_t index = 0; index < kNumMaxInfluence; ++index) { // 空いているところに入れる
				if (currentInfluence.weights[index] == 0.0f) { // weight == 0が空いている状態なので、その場所にweightとjointのindexを代入
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;
					break;
				}
			}
		}
	}

	return skinCluster;
}
