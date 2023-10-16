﻿#include "Sphere.h"

#include "externals/DirectXTex/DirectXTex.h"
#include <d3dcompiler.h>

#include <dxcapi.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "externals/imgui/imgui.h"

#define _USE_MATH_DEFINES
#include<math.h>



ID3D12Device* Sphere::device_ = nullptr;
ID3D12GraphicsCommandList* Sphere::commandList_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> Sphere::rootSignature_;
Microsoft::WRL::ComPtr<ID3D12PipelineState> Sphere::graphicsPipelineState_;


void Sphere::InitializeRootSignature()
{
	HRESULT result_ = S_FALSE;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//// デスクリプタレンジ
	//D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	//descriptorRange[0].BaseShaderRegister = 0;
	//descriptorRange[0].NumDescriptors = 1;
	//descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//D3D12_ROOT_PARAMETER rootParameters[4] = {};

	//rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//rootParameters[0].Descriptor.ShaderRegister = 0;

	//rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//rootParameters[1].Descriptor.ShaderRegister = 1;

	//rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	//rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//rootParameters[2].Descriptor.ShaderRegister = 2;

	//rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	//rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	//rootParameters[3].DescriptorTable.pDescriptorRanges = descriptorRange;
	//rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

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
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
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

void Sphere::InitializeGraphicsPipeline()
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

ID3D12Resource* Sphere::CreateBufferResource(size_t sizeInBytes)
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

void Sphere::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(Sphere::commandList_ == nullptr);

	commandList_ = commandList;

	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(graphicsPipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

void Sphere::PostDraw()
{
	commandList_ = nullptr;
}

void Sphere::StaticInitialize(ID3D12Device* device)
{
	assert(device);

	device_ = device;


	InitializeGraphicsPipeline();

}

Sphere* Sphere::Create()
{
	Sphere* sphere = new Sphere();
	assert(sphere);

	sphere->Initialize();

	return sphere;
}

void Sphere::Initialize()
{
	// nullptrチェック
	assert(device_);

	// メッシュ生成
	CreateMesh();
}

void Sphere::Update()
{
#ifdef _DEBUG
	ImGui::Begin("Color");

	ImGui::End();

#endif // _DEBUG



}

void Sphere::Draw(const WorldTransform& worldTransform, const ViewProjection& viewProjection, uint32_t textureHandle)
{
	assert(device_);
	assert(commandList_);
	assert(worldTransform.constBuff_.Get());

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);

	// インデックスバッファの設定
	commandList_->IASetIndexBuffer(&indexBufferView_);

	// CBVをセット(マテリアル)
	//commandList_->SetGraphicsRootConstantBufferView(/*static_cast<UINT>(RoomParameter::kMaterial)*/0, materialResource_->GetGPUVirtualAddress());

	// CBVをセット(ワールド行列)
	commandList_->SetGraphicsRootConstantBufferView(/*static_cast<UINT>(RoomParameter::kWorldTransform)*/0, worldTransform.constBuff_->GetGPUVirtualAddress());

	// CBVをセット(ビュープロジェクション行列)
	commandList_->SetGraphicsRootConstantBufferView(/*static_cast<UINT>(RoomParameter::kViewProjection)*/1, viewProjection.constBuff_->GetGPUVirtualAddress());

	// SRVをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, /*static_cast<UINT>(RoomParameter::kTexture)*/2, textureHandle);

	commandList_->DrawIndexedInstanced(16 * 16 * 6, 1, 0, 0, 0);

}

void Sphere::CreateMesh()
{
	HRESULT result = S_FALSE;
	vertexResource_ = CreateBufferResource(sizeof(VertexData) * 16 * 16 * 4);
	indexResource_ = CreateBufferResource(sizeof(uint32_t) * 16 * 16 * 6);

	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 16 * 16 * 4;
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 16 * 16 * 6;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
	
	result = vertexResource_->Map(0, nullptr, (void**)&vertexData_);
	
	// 経度分割1つ分の角度φ
	const float kLonEvery = float(M_PI) * 2.0f / 16.0f;
	// 緯度分割1つ分の角度θ
	const float kLatEvery = float(M_PI) / 16.0f;
	// 緯度の方向に分割
	for (uint32_t latIndex = 0; latIndex < 16; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;
		float v = 1.0f - float(latIndex) / 16.0f;
		// 経度の方向に分割しながら線を描く
		for (uint32_t lonIndex = 0; lonIndex < 16; ++lonIndex) {
			uint32_t start = (latIndex * 16 + lonIndex) * 4;
			float lon = lonIndex * kLonEvery;
			float u = float(lonIndex) / 16.0f;

			// 基準点a
			vertexData_[start].position.x = cos(lat) * cos(lon);
			vertexData_[start].position.y = sin(lat);
			vertexData_[start].position.z = cos(lat) * sin(lon);
			vertexData_[start].position.w = 1.0f;
			vertexData_[start].texcoord = { u, v + 1.0f / 16.0f };
			vertexData_[start].normal.x = vertexData_[start].position.x;
			vertexData_[start].normal.y = vertexData_[start].position.y;
			vertexData_[start].normal.z = vertexData_[start].position.z;
			// b
			vertexData_[start + 1].position.x = cos(lat + kLatEvery) * cos(lon);
			vertexData_[start + 1].position.y = sin(lat + kLatEvery);
			vertexData_[start + 1].position.z = cos(lat + kLatEvery) * sin(lon);
			vertexData_[start + 1].position.w = 1.0f;
			vertexData_[start + 1].texcoord = { u, v };
			vertexData_[start + 1].normal.x = vertexData_[start + 1].position.x;
			vertexData_[start + 1].normal.y = vertexData_[start + 1].position.y;
			vertexData_[start + 1].normal.z = vertexData_[start + 1].position.z;
			
			// c
			vertexData_[start + 2].position.x = cos(lat) * cos(lon + kLonEvery);
			vertexData_[start + 2].position.y = sin(lat);
			vertexData_[start + 2].position.z = cos(lat) * sin(lon + kLonEvery);
			vertexData_[start + 2].position.w = 1.0f;
			vertexData_[start + 2].texcoord = { u + 1.0f / 16.0f, v + 1.0f / 16.0f };
			vertexData_[start + 2].normal.x = vertexData_[start + 2].position.x;
			vertexData_[start + 2].normal.y = vertexData_[start + 2].position.y;
			vertexData_[start + 2].normal.z = vertexData_[start + 2].position.z;
			
			// d
			vertexData_[start + 3].position.x = cos(lat + kLatEvery) * cos(lon + kLonEvery);
			vertexData_[start + 3].position.y = sin(lat + kLatEvery);
			vertexData_[start + 3].position.z = cos(lat + kLatEvery) * sin(lon + kLonEvery);
			vertexData_[start + 3].position.w = 1.0f;
			vertexData_[start + 3].texcoord = { u + 1.0f / 16.0f, v };
			vertexData_[start + 3].normal.x = vertexData_[start + 3].position.x;
			vertexData_[start + 3].normal.y = vertexData_[start + 3].position.y;
			vertexData_[start + 3].normal.z = vertexData_[start + 3].position.z;
			
		}
	}

	indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

	for (uint32_t latIndex = 0; latIndex < 16; ++latIndex) {
		for (uint32_t lonIndex = 0; lonIndex < 16; ++lonIndex) {
			uint32_t start = (latIndex * 16 + lonIndex) * 6;
			uint32_t i = (latIndex * 16 + lonIndex) * 4;
			indexData_[start] = i;
			indexData_[start + 1] = i + 1;
			indexData_[start + 2] = i + 2;
			indexData_[start + 3] = i + 1;
			indexData_[start + 4] = i + 3;
			indexData_[start + 5] = i + 2;
		}
	}

	// マテリアル用のリソース
	materialResource_ = CreateBufferResource(sizeof(Vector4));
	// マテリアルにデータを書き込む
	//Vector4* materialData = nullptr;
	// アドレスを取得
	materialResource_->Map(0, nullptr, (void**)&materialData_);
	*materialData_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

}

void Sphere::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}








