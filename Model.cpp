#include "Model.h"

#include "externals/DirectXTex/DirectXTex.h"
#include <d3dcompiler.h>
#include <dxcapi.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "externals/imgui/imgui.h"

#include <cassert>
#include <fstream>
#include <sstream>


ID3D12Device* Model::device_ = nullptr;
ID3D12GraphicsCommandList* Model::commandList_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> Model::rootSignature_;
Microsoft::WRL::ComPtr<ID3D12PipelineState> Model::graphicsPipelineState_;


void Model::InitializeRootSignature()
{
	HRESULT result_ = S_FALSE;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// デスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameters[3] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 1;

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = &descriptorRange[0];
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderを使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// シリアライズしてバイナリにする
	Microsoft::WRL::ComPtr<ID3DBlob> signatureBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;
	result_ = D3D12SerializeRootSignature(
		&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&signatureBlob, &errorBlob);
	if (FAILED(result_)) {
		assert(false);
	}

	// ルートシグネチャー作成
	rootSignature_ = nullptr;
	result_ = device_->CreateRootSignature(
		0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

}

void Model::InitializeGraphicsPipeline()
{
	HRESULT result_ = S_FALSE;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;

	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	//dxcCompilerを初期化
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	result_ = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(result_));
	result_ = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(result_));

	IDxcIncludeHandler* includeHandler = nullptr;
	result_ = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(result_));

	// 頂点シェーダー
	//hlslファイルを読む
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource_vs = nullptr;
	result_ = dxcUtils->LoadFile(L"Object3d.VS.hlsl", nullptr, &shaderSource_vs);
	//読めなかったら止める
	assert(SUCCEEDED(result_));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer_vs;
	shaderSourceBuffer_vs.Ptr = shaderSource_vs->GetBufferPointer();
	shaderSourceBuffer_vs.Size = shaderSource_vs->GetBufferSize();
	shaderSourceBuffer_vs.Encoding = DXC_CP_UTF8; //UTF8の文字コードであることを通知

	LPCWSTR arguments_vs[] = {
		L"Object3d.VS.hlsl", //コンパイル対象のhlslファイル名
		L"-E", L"main", //エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", L"vs_6_0", //ShaderProfileの設定
		L"-Zi", L"-Qembed_debug", //デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr", //メモリレイアウトは行優先
	};
	//実際にShaderをコンパイルする
	Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
	result_ = dxcCompiler->Compile(
		&shaderSourceBuffer_vs, //読み込んだファイル
		arguments_vs, //コンパイルオプション
		_countof(arguments_vs), //コンパイルオプションの数
		includeHandler, //includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult) //コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(result_));

	//警告・エラーが出てたらログに出して止める
	Microsoft::WRL::ComPtr<IDxcBlobUtf8>  shaderError_vs = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError_vs), nullptr);
	if (shaderError_vs != nullptr && shaderError_vs->GetStringLength() != 0) {
		Log(shaderError_vs->GetStringPointer());
		//警告・エラーダメゼッタイ
		assert(false);
	}


	result_ = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&vsBlob), nullptr);
	assert(SUCCEEDED(result_));

	// ピクセルシェーダー
	//hlslファイルを読む
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource_ps = nullptr;
	result_ = dxcUtils->LoadFile(L"Object3d.PS.hlsl", nullptr, &shaderSource_ps);
	//読めなかったら止める
	assert(SUCCEEDED(result_));
	DxcBuffer shaderSourceBuffer_ps;
	shaderSourceBuffer_ps.Ptr = shaderSource_ps->GetBufferPointer();
	shaderSourceBuffer_ps.Size = shaderSource_ps->GetBufferSize();
	shaderSourceBuffer_ps.Encoding = DXC_CP_UTF8; //UTF8の文字コードであることを通知
	LPCWSTR arguments_ps[] = {
		L"Object3d.PS.hlsl", //コンパイル対象のhlslファイル名
		L"-E", L"main", //エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", L"ps_6_0", //ShaderProfileの設定
		L"-Zi", L"-Qembed_debug", //デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr", //メモリレイアウトは行優先
	};
	//実際にShaderをコンパイルする
	shaderResult = nullptr;
	result_ = dxcCompiler->Compile(
		&shaderSourceBuffer_ps, //読み込んだファイル
		arguments_ps, //コンパイルオプション
		_countof(arguments_ps), //コンパイルオプションの数
		includeHandler, //includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult) //コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(result_));

	//警告・エラーが出てたらログに出して止める
	Microsoft::WRL::ComPtr<IDxcBlobUtf8>  shaderError_ps = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError_ps), nullptr);
	if (shaderError_ps != nullptr && shaderError_ps->GetStringLength() != 0) {
		Log(shaderError_ps->GetStringPointer());
		//警告・エラーダメゼッタイ
		assert(false);
	}

	result_ = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&psBlob), nullptr);
	assert(SUCCEEDED(result_));

	InitializeRootSignature();

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = true;
	// 書き込みします
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	// 比較関数はLessEqual。つまり、近ければ描画される
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//PSOを作成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_.Get(); //RootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc; //InputLayout
	graphicsPipelineStateDesc.VS = { vsBlob->GetBufferPointer(),
	vsBlob->GetBufferSize() }; //VertexShader
	graphicsPipelineStateDesc.PS = { psBlob->GetBufferPointer(),
	psBlob->GetBufferSize() }; //PixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc; //BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc; //RasterizerState
	// DepthStencilの設定
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するトポロジ(形状)のタイプ。三角形
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面にいろを打ち込むかの設定(気にしなくて良い)
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	graphicsPipelineState_ = nullptr;
	result_ = device_->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(result_));
}

ID3D12Resource* Model::CreateBufferResource(size_t sizeInBytes)
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
	ID3D12Resource* resource = nullptr;
	result = device_->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));
	return resource;
}

void Model::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(Model::commandList_ == nullptr);

	commandList_ = commandList;

	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(graphicsPipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

void Model::PostDraw()
{
	commandList_ = nullptr;
}

Model* Model::Create(const std::string& modelname)
{
	Model* model = new Model();
	model->LoadObjFile(modelname);
	model->Initialize();
	return model;
}

void Model::StaticInitialize(ID3D12Device* device)
{
	assert(device);

	device_ = device;


	InitializeGraphicsPipeline();

}

void Model::Initialize()
{
	// nullptrチェック
	assert(device_);
	CreateMesh();
}


void Model::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHandle)
{
	assert(device_);
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// CBVをセット(ワールド行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kViewProjection), viewProjection.constBuff_->GetGPUVirtualAddress());

	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(RoomParameter::kTexture), textureHandle);

	commandList_->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

}

void Model::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection)
{

	assert(device_);
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// CBVをセット(ワールド行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kWorldTransform), worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RoomParameter::kViewProjection), viewProjection.constBuff_->GetGPUVirtualAddress());

	// SRVをセット
	if (textureHandle == 0) {
		textureHandle = TextureManager::Load(modelData.material.textureFilePath);
	}
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, static_cast<UINT>(RoomParameter::kTexture), textureHandle);

	commandList_->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

}

void Model::LoadObjFile(const std::string& filename)
{
	// 1. 変数の宣言
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file("Resources/Models/" + filename + "/" + filename + ".obj");
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
			modelData.material = LoadMaterialTemplateFile(filename, materialFilename);
		}
		else if (identifier == "f") {
			VertexData triangle[3];
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
}

void Model::CreateMesh()
{
	indexResource_ = CreateBufferResource(sizeof(uint32_t) * modelData.vertices.size());
	// 頂点リソース
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress(); // リソースの先頭のアドレスから使う
	vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size()); // 使用するリソースのサイズは頂点サイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData); // 1頂点当たりのサイズ

	// 頂点リソースにデータを書き込む
	vertexResource_->Map(0, nullptr, (void**)&vertexData_); // 書き込むためのアドレスを取得
	std::memcpy(vertexData_, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size()); // 頂点データをリソースにコピー
	

}

Model::MaterialData Model::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	// 1. 変数の宣言
	MaterialData materialData;
	std::string line;
	// 2. ファイルを開く
	std::ifstream file("Resources/Models/"+ directoryPath + "/" + filename);
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
			materialData.textureFilePath =  "Models/" + directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}

void Model::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}








