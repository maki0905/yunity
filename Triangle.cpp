#include "Triangle.h"

#include "externals/DirectXTex/DirectXTex.h"
#include <d3dcompiler.h>

#include <dxcapi.h>
#pragma comment(lib, "d3dcompiler.lib")

ID3D12Device* Triangle::device_ = nullptr;
ID3D12GraphicsCommandList* Triangle::commandList_ = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> Triangle::rootSignature_;
Microsoft::WRL::ComPtr<ID3D12PipelineState> Triangle::graphicsPipelineState_;


void Triangle::InitializeRootSignature()
{
	HRESULT result_ = S_FALSE;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
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

	rootSignature_ = nullptr;
	result_ = device_->CreateRootSignature(
		0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

}

void Triangle::InitializeGraphicsPipeline()
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
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
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

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	// バッファリソース
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeof(Vector4) * 3;
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際にリソースを作る
	result_ = device_->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource_));
	assert(SUCCEEDED(result_));



	// 頂点バッファビュー
	vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
	vertexBufferView_.SizeInBytes = sizeof(Vector4) * 3;
	vertexBufferView_.StrideInBytes = sizeof(Vector4);

	vertexData_ = {
		{-0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.0f, 0.5f, 0.0f, 1.0f },
		{ 0.5f, -0.5f, 0.0f, 1.0f }
	};
	
	Vector4* vertexData = nullptr;
	result_ = vertexResource_->Map(0, nullptr, (void**)&vertexData);
	if (SUCCEEDED(result_)) {
		std::copy(vertexData_.begin(), vertexData_.end(), vertexData);
		vertexResource_->Unmap(0, nullptr);
	}
	
	
}

void Triangle::CreateVertexResource()
{
	//HRESULT result_ = S_FALSE;
	

}

void Triangle::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	assert(Triangle::commandList_ == nullptr);

	commandList_ = commandList;

	commandList->SetGraphicsRootSignature(rootSignature_.Get());
	commandList->SetPipelineState(graphicsPipelineState_.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


}

void Triangle::PostDraw()
{
	commandList_ = nullptr;
}

void Triangle::StaticInitialize(ID3D12Device* device)
{
	assert(device);

	device_ = device;


	InitializeGraphicsPipeline();

}

void Triangle::Draw()
{
	assert(device_);
	assert(commandList_);
	
	commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
	commandList_->DrawInstanced(3, 1, 0, 0);

}

void Triangle::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}







