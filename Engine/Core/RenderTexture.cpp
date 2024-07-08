#include "RenderTexture.h"

#include <cassert>

#include "WindowsAPI.h"
#include "Device.h"
#include "DirectXCore.h"
#include "DescriptorHeap.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "TextureManager.h"
#include "CameraManager.h"

RootSignature* RenderTexture::rootSignature_ = nullptr;
PipelineState* RenderTexture::pipelineState_ = nullptr;

void RenderTexture::InitializeGraphicsPipeline()
{
	//rootSignature_ = new RootSignature(Device::GetInstance()->GetDevice(), static_cast<int>(RootBindings::kCount), 2);
	rootSignature_ = new RootSignature(Device::GetInstance()->GetDevice(), static_cast<int>(RootBindings::kCount), 1);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[2];
	staticSamplers[0] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0; //レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderを使う

	staticSamplers[1] = {};
	staticSamplers[1].Filter = D3D12_FILTER_MIN_MAG_MIP_POINT; // ポイントフィルタ
	staticSamplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; //0~1の範囲外をリピート
	staticSamplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; //比較しない
	staticSamplers[1].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[1].ShaderRegister = 1; //レジスタ番号1を使う
	staticSamplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //PixelShaderを使う


	rootSignature_->InitializeStaticSampler(0, staticSamplers[0], D3D12_SHADER_VISIBILITY_PIXEL);

	//rootSignature_->InitializeStaticSampler(1, staticSamplers[1], D3D12_SHADER_VISIBILITY_PIXEL);

	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kDepthTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, D3D12_SHADER_VISIBILITY_PIXEL);

	rootSignature_->GetParameter(static_cast<size_t>(RootBindings::kMaterial)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);

	rootSignature_->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	pipelineState_ = new PipelineState(Device::GetInstance()->GetDevice(), rootSignature_);

	// InputLayout
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = nullptr;
	inputLayoutDesc.NumElements = 0;

	// BlendState
	D3D12_BLEND_DESC blendDesc{};
	// すべての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	//RasiterzerStateの設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面(時計回り)を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	// DepthStencilStateの設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	// Depthの機能を有効化する
	depthStencilDesc.DepthEnable = false;

	pipelineState_->SetInputLayout(inputLayoutDesc);
	pipelineState_->SetShader(PipelineState::ShaderType::kVS, ShaderCompiler::GetInstance()->Get("Fullscreen", ShaderCompiler::ShaderType::kVS));
	pipelineState_->SetShader(PipelineState::ShaderType::kPS, ShaderCompiler::GetInstance()->Get("CopyImage", ShaderCompiler::ShaderType::kPS));
	pipelineState_->SetBlendState(blendDesc);
	pipelineState_->SetRasterizerState(rasterizerDesc);
	pipelineState_->SetDepthStencilState(depthStencilDesc);
	pipelineState_->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
	pipelineState_->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	pipelineState_->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
	pipelineState_->Finalize();
}

RenderTexture* RenderTexture::GetInstance()
{
	static RenderTexture instance;

	return &instance;
}

void RenderTexture::Initalize()
{
	depthBuffe_ = std::make_unique<DepthBuffer>();
	depthBuffe_->Create();
	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Initalize();
	for (uint32_t index = 0; index < static_cast<uint32_t>(PostEffects::kCount); index++) {
		postEffectFlag_[index] = false;
	}
}

void RenderTexture::Finalize()
{
	/*postEffect_->Finalize();*/
	if (rootSignature_) {
		delete rootSignature_;
		rootSignature_ = nullptr;
	}

	if (pipelineState_) {
		delete pipelineState_;
		pipelineState_ = nullptr;
	}
}

void RenderTexture::Create()
{
	CreateResorce();
	CreateRTV();
	CreateSRV();
	InitializeMaterial();
	commandList_ = DirectXCore::GetInstance()->GetCommandList();
}

void RenderTexture::Copy()
{
	materialData_->projectionInverse = Inverse(CameraManager::GetInstance()->GetCamera()->GetProjectionMatrix());

	commandList_->SetGraphicsRootSignature(rootSignature_->GetSignature());
	commandList_->SetPipelineState(pipelineState_->GetPipelineStateObject());
	commandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(RootBindings::kTexture), gpuDescHandleSRV_);
	commandList_->SetGraphicsRootDescriptorTable(static_cast<UINT>(RootBindings::kDepthTexture), depthTextureGpuDescHandleSRV_);
	commandList_->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kMaterial), materialResource_->GetGPUVirtualAddress());
	//TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(commandList_.Get(), static_cast<UINT>(RootBindings::kTexture), 0);
	commandList_->DrawInstanced(3, 1, 0, 0);
}

void RenderTexture::PreDraw()
{
	selectedFlag_ = false;
	for (auto& flag : postEffectFlag_) {
		if (flag) {
			selectedFlag_ = true;
			break;
		}
	}



}

void RenderTexture::PostDraw()
{
}

//void RenderTexture::OMSetREnderTargets(ID3D12DescriptorHeap* dsvHeap_)
//{
//	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
//	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
//		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
//	// レンダーターゲットをセット
//	commandList_->OMSetRenderTargets(1, &depthTextureCpuDescHandleSRV_, false, &dsvHandle);
//}

void RenderTexture::ClearRenderTargetView()
{
	if (postEffectFlag_[static_cast<uint32_t>(PostEffects::kOutline)]) {
		postEffect_->ClearRenderTargetView();
	}
	else {
		float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
		commandList_->ClearRenderTargetView(cpuDescHandleRTV_, clearColor, 0, nullptr);
	}
}

void RenderTexture::OMSetRenderTargets()
{
	if (postEffectFlag_[static_cast<uint32_t>(PostEffects::kOutline)]) {
		postEffect_->OMSetRenderTargets();
	}
	else {
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
			D3D12_CPU_DESCRIPTOR_HANDLE(depthBuffe_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
		commandList_->OMSetRenderTargets(1, &cpuDescHandleRTV_, false, &dsvHandle);
	}
}

void RenderTexture::ClearDepthStencilView()
{
	if (postEffectFlag_[static_cast<uint32_t>(PostEffects::kOutline)]) {
		postEffect_->ClearDepthStencilView();
	}
	else {
		// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
			D3D12_CPU_DESCRIPTOR_HANDLE(depthBuffe_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
		// 深度バッファのクリア
		commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
}



void RenderTexture::CreateResorce()
{
	renderTextureResource_ = CreateRenderTextureResource(WindowsAPI::kWindowWidth, WindowsAPI::kWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);

	HRESULT hr = S_FALSE;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Width = WindowsAPI::GetInstance()->kWindowWidth;
	depthResourceDesc.Height = WindowsAPI::GetInstance()->kWindowHeight;
	depthResourceDesc.MipLevels = 1;
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	D3D12_CLEAR_VALUE depthClearValue;
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる

	hr = Device::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定
		&depthResourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&depthStencilResource_)
	);
	assert(SUCCEEDED(hr));
}

void RenderTexture::CreateRTV()
{
	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	DescriptorHeap* rtvDescriptorHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kRTV);
	// ディスクリプタヒープのハンドルを取得
	cpuDescHandleRTV_ = rtvDescriptorHeap_->Alloc().GetCPUHandle();

	Device::GetInstance()->GetDevice()->CreateRenderTargetView(renderTextureResource_.Get(), &renderTargetViewDesc, cpuDescHandleRTV_);
}

void RenderTexture::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;

	DescriptorHeap* srvDescriptorHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV);
	DescriptorHandle descriptorHandleSRV_ = srvDescriptorHeap_->Alloc();
	cpuDescHandleSRV_ = descriptorHandleSRV_.GetCPUHandle();
	gpuDescHandleSRV_ = descriptorHandleSRV_.GetGPUHandle();
	Device::GetInstance()->GetDevice()->CreateShaderResourceView(renderTextureResource_.Get(), &renderTextureSrvDesc, cpuDescHandleSRV_);

	// DepthTexture用のSRV
	D3D12_SHADER_RESOURCE_VIEW_DESC depthTextureSrvDesc{};
	// DXGI_FORMAT_D24_UNORM_S8_UINTのDepthを読むときはDXGI_FORMAT_UNORM_X8_TYPELESS
	depthTextureSrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	depthTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	depthTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	depthTextureSrvDesc.Texture2D.MipLevels = 1;
	DescriptorHeap* depthTextureSrvDescriptorHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV);
	DescriptorHandle depthTextureDescriptorHandleSRV_ = depthTextureSrvDescriptorHeap_->Alloc();
	depthTextureCpuDescHandleSRV_ = depthTextureDescriptorHandleSRV_.GetCPUHandle();
	depthTextureGpuDescHandleSRV_ = depthTextureDescriptorHandleSRV_.GetGPUHandle();
	Device::GetInstance()->GetDevice()->CreateShaderResourceView(depthBuffe_->GetDepthStencil(), &depthTextureSrvDesc, depthTextureCpuDescHandleSRV_);

}

void RenderTexture::InitializeMaterial()
{
	materialResource_ = CreateBufferResource(sizeof(Material));
	materialData_ = nullptr;
	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->projectionInverse = MakeIdentity4x4();
}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderTexture::CreateRenderTextureResource(uint32_t width, uint32_t hight, DXGI_FORMAT format, const Vector4& clearColor)
{
	HRESULT result = S_FALSE;

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Width = width; // リソースのサイズ
	resourceDesc.Height = hight;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	resourceDesc.Format = format;
	
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = clearColor.x;
	clearValue.Color[1] = clearColor.y;
	clearValue.Color[2] = clearColor.z;
	clearValue.Color[3] = clearColor.w;

	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	result = Device::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET, // これから描画することを前提としたTextureなのでRenderTargetとして使うことから始める
		&clearValue, // Clear最適値。ClearRenderTargetをこの色でClearするようにする。最適化されているので高速である。
		IID_PPV_ARGS(&resource));


	assert(SUCCEEDED(result));

	return resource;
}

Microsoft::WRL::ComPtr<ID3D12Resource> RenderTexture::CreateBufferResource(size_t sizeInBytes)
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
