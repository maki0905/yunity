#include "Sprite.h"

#include <cassert>
#include<d3dcompiler.h>

#include "Device.h"
#include "TextureManager.h"
#include "WindowsAPI.h"

#pragma comment(lib, "d3dcompiler.lib")


/// <summary>
/// 静的メンバ変数の実体
/// </summary>
UINT Sprite::descriptorHandleIncrementSize_;
ID3D12GraphicsCommandList* Sprite::commandList_ = nullptr;
RootSignature* Sprite::rootSignature_ = nullptr;
PipelineState* Sprite::pipelineState_ = nullptr;
Matrix4x4 Sprite::matProjection_;


void Sprite::StaticInitialize()
{

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize_ = Device::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	HRESULT result = S_FALSE;
	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;    // 頂点シェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	std::wstring vsFile = L"Resources/Shaders/SpriteVS.hlsl";
	result = D3DCompileFromFile(
		vsFile.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,  // デバッグ用設定
		0, &vsBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	std::wstring psFile = L"Resources/Shaders/SpritePS.hlsl";
	result = D3DCompileFromFile(
		psFile.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	rootSignature_ = new RootSignature(Device::GetInstance()->GetDevice(), 2, 1);
	
	// スタティックサンプラー
	D3D12_STATIC_SAMPLER_DESC staticSamplers = {};
	staticSamplers.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
	staticSamplers.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers.MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers.ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderを使う
	rootSignature_->InitializeStaticSampler(0, staticSamplers, D3D12_SHADER_VISIBILITY_PIXEL);

	rootSignature_->GetParameter(0).InitializeAsConstantBuffer(0);
	rootSignature_->GetParameter(1).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1);

	rootSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	pipelineState_ = new PipelineState(Device::GetInstance()->GetDevice(), rootSignature_);
	
	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		// xy座標
		{"POSITIONT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0 , D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputLayout;
	inputLayoutDesc.NumElements = _countof(inputLayout);

	// グラフィックスパイプラインの流れを設定
	pipelineState_->SetShader(PipelineState::ShaderType::kVS, vsBlob);
	pipelineState_->SetShader(PipelineState::ShaderType::kPS, psBlob);

	// サンプルマスク
	pipelineState_->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	// ラスタライザステート
	pipelineState_->SetRasterizerState(rasterizerDesc);
	pipelineState_->SetCullMode(D3D12_CULL_MODE_NONE);

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthStencilDesc.StencilEnable = FALSE;
	depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp = { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
	depthStencilDesc.FrontFace = defaultStencilOp;
	depthStencilDesc.BackFace = defaultStencilOp;
	// デプスステンシルステート
	pipelineState_->SetDepthStencilState(depthStencilDesc);
	
	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	pipelineState_->SetBlendState(blenddesc);

	// 頂点レイアウトの設定
	pipelineState_->SetInputLayout(inputLayoutDesc);

	// 図形の形状設定（三角形）
	pipelineState_->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

	pipelineState_->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
	
	pipelineState_->Finalize();

	// 射影行列計算
	matProjection_ = MakeOrthographicMatrix(
		0.0f, 0.0f, (float)WindowsAPI::kWindowWidth, (float)WindowsAPI::kWindowHeight, 0.0f, 1.0f);

}

void Sprite::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Sprite::commandList_ == nullptr);
	//assert(pipelineState_ == nullptr);
	// コマンドリストをセット
	commandList_ = commandList;

	// パイプラインステートの設定
	commandList_->SetPipelineState(pipelineState_->GetPipelineStateObject());
	// ルートシグネチャの設定
	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	// プリミティブ形状を設定
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}

void Sprite::PostDraw()
{
	// コマンドリストを解除
	Sprite::commandList_ = nullptr;
}

Sprite* Sprite::Create(uint32_t textureHandle, Vector2 position, Vector4 color, Vector2 anchorpoint, bool isFlipx, bool isFlipY)
{
	// 仮サイズ
	Vector2 size = { 100.0f, 100.0f };

	{
		// テクスチャ情報取得
		const D3D12_RESOURCE_DESC& resDesc = TextureManager::GetInstance()->GetResoureDesc(textureHandle);
		// スプライトのサイズをテクスチャのサイズに設定
		size = { (float)resDesc.Width, (float)resDesc.Height };
	}

	// Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(textureHandle, position, size, color, anchorpoint, isFlipx, isFlipY);
	if (sprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}
	return sprite;
}

Sprite::Sprite()
{
}

Sprite::Sprite(uint32_t textureHandle, Vector2 position, Vector2 size, Vector4 color, Vector2 anchorpoint, bool isFlipx, bool isFlipY)
{
	position_ = position;
	size_ = size;
	anchorPoint_ = anchorpoint;
	matWorld_ = MakeIdentity4x4();
	color_ = color;
	textureHandle_ = textureHandle;
	isFlipX_ = isFlipx;
	isFlipY_ = isFlipY;
	texSize_ = size;
}

void Sprite::Finalize()
{
	if (rootSignature_) {
		delete rootSignature_;
	}
	if (pipelineState_) {
		delete pipelineState_;
	}
}


bool Sprite::Initialize()
{
	// nullptrチェック
	HRESULT result = S_FALSE;
	resourceDesc_ = TextureManager::GetInstance()->GetResoureDesc(textureHandle_);

	{
		// ヒーププロパティ
		D3D12_HEAP_PROPERTIES uploadHeapProperties{};
		uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // uploadHeapを使う
		// リソース設定
		D3D12_RESOURCE_DESC vertexResourceDesc{};
		// バッファリソース。テクスチャの場合はまた別の設定をする
		vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vertexResourceDesc.Width = sizeof(VertexPosUv) * kVertNum;
		// バッファの場合はこれらは1にする決まり
		vertexResourceDesc.Height = 1;
		vertexResourceDesc.DepthOrArraySize = 1;
		vertexResourceDesc.MipLevels = 1;
		vertexResourceDesc.SampleDesc.Count = 1;
		// バッファの場合はこれにする決まり
		vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		// 頂点バッファ生成
		result = Device::GetInstance()->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
			&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&vertBuff_));
		assert(SUCCEEDED(result));

		// 頂点バッファマッピング
		result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
	}

	// 頂点バッファへのデータ転送
	TransferVertices();

	// 頂点バッファビューの作成
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	vbView_.SizeInBytes = sizeof(VertexPosUv) * 4;
	vbView_.StrideInBytes = sizeof(VertexPosUv);

	{
		// ヒーププロパティ
		D3D12_HEAP_PROPERTIES heapProperties{};
		heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // uploadHeapを使う
		// リソースの設定
		D3D12_RESOURCE_DESC resourceDesc{};
		// バッファリソース。テクスチャの場合はまた別の設定をする
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width = ((sizeof(ConstBufferData) + 0xff) & ~0xff);
		// バッファの場合はこれらは1にする決まり
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.SampleDesc.Count = 1;
		// バッファの場合はこれにする決まり
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		// 定数バッファの生成
		result = Device::GetInstance()->GetDevice()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
			&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
			IID_PPV_ARGS(&constBuff_));
		assert(SUCCEEDED(result));
	}

	// 定数バッファマッピング
	result = constBuff_->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));

	return true;
}

void Sprite::SetTextureHandle(uint32_t textureHandle)
{
	textureHandle_ = textureHandle;
	resourceDesc_ = TextureManager::GetInstance()->GetResoureDesc(textureHandle_);
}

void Sprite::SetPosition(const Vector2 position)
{
	position_ = position;
	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetRotation(float rotation)
{
	rotation_ = rotation;
	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetSize(const Vector2& size)
{
	size_ = size;
	// 頂点バッファへのデータ転送
	TransferVertices();

}

void Sprite::SetAnchorPoint(const Vector2& anchorPoint)
{
	anchorPoint_ = anchorPoint;
	// 頂点バッファへのデータ転送
	TransferVertices();

}

void Sprite::SetIsFlipX(bool isFlipX)
{
	isFlipX_ = isFlipX;
	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetIsFlipY(bool isFlipY)
{
	isFlipY_ = isFlipY;
	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::SetTextureRect(const Vector2 texBase, const Vector2& texSize)
{
	texBase_ = texBase;
	texSize_ = texSize;
	// 頂点バッファへのデータ転送
	TransferVertices();
}

void Sprite::Draw()
{
	// ワールド行列の更新
	matWorld_ = MakeIdentity4x4();
	matWorld_ = Multiply(matWorld_, MakeRotateZMatrix(rotation_));
	matWorld_ = Multiply(matWorld_, MakeTranslateMatrix(Vector3{ position_.x, position_.y, 0.0f }));

	// 定数バッファにデータ転送
	constMap->color = color_;
	constMap->mat = Multiply(matWorld_, matProjection_); // 行列の合成

	// 頂点バッファの設定
	commandList_->IASetVertexBuffers(0, 1, &vbView_);

	// 定数バッファビューをセット
	commandList_->SetGraphicsRootConstantBufferView(0, constBuff_->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_, 1, textureHandle_);
	// 描画コマンド
	commandList_->DrawInstanced(4, 1, 0, 0);

}

void Sprite::TransferVertices()
{
	HRESULT result = S_FALSE;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorPoint_.x) * size_.x;
	float right = (1.0f - anchorPoint_.x) * size_.x;
	float top = (0.0f - anchorPoint_.y) * size_.y;
	float bottom = (1.0f - anchorPoint_.y) * size_.y;
	if (isFlipX_) {// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY_) {// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	// 頂点データ
	VertexPosUv vertices[kVertNum];

	vertices[LB].pos = { left, bottom, 0.0f };  // 左下
	vertices[LT].pos = { left, top, 0.0f };	    // 左上
	vertices[RB].pos = { right, bottom, 0.0f }; // 右下
	vertices[RT].pos = { right, top, 0.0f };    // 右上

	// テクスチャ情報取得
	{
		float tex_left = texBase_.x / resourceDesc_.Width;
		float tex_right = (texBase_.x + texSize_.x) / resourceDesc_.Width;
		float tex_top = texBase_.y / resourceDesc_.Height;
		float tex_bottom = (texBase_.y + texSize_.y) / resourceDesc_.Height;
		vertices[LB].uv = { tex_left, tex_bottom }; // 左下
		vertices[LT].uv = { tex_left, tex_top };    // 左上
		vertices[RB].uv = { tex_right, tex_bottom };  // 右下
		vertices[RT].uv = { tex_right, tex_top };   // 右上
	}
	// 頂点バッファへのデータ転送
	memcpy(vertMap, vertices, sizeof(vertices));

}