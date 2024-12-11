#include "PostEffect.h"

#include <cassert>

#include "WindowsAPI.h"
#include "Device.h"
#include "DirectXCore.h"
#include "DescriptorHeap.h"
#include "RootSignature.h"
#include "PipelineState.h"
#include "TextureManager.h"
#include "CameraManager.h"
#include "ImGuiManager.h"

yunity::RootSignature* yunity::PostEffect::rootSignature_[static_cast<uint32_t>(yunity::PostEffects::kCount)];
yunity::PipelineState* yunity::PostEffect::pipelineState_[static_cast<uint32_t>(yunity::PostEffects::kCount)];

void yunity::PostEffect::InitializeGraphicsPipeline()
{

	for (uint32_t index = 0; index < static_cast<uint32_t>(yunity::PostEffects::kCount); index++) {

		rootSignature_[index] = new yunity::RootSignature(Device::GetInstance()->GetDevice(), static_cast<int>(RootBindings::kCount), 2);

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


		rootSignature_[index]->InitializeStaticSampler(0, staticSamplers[0], D3D12_SHADER_VISIBILITY_PIXEL);

		rootSignature_[index]->InitializeStaticSampler(1, staticSamplers[1], D3D12_SHADER_VISIBILITY_PIXEL);

		rootSignature_[index]->GetParameter(static_cast<size_t>(RootBindings::kTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);

		rootSignature_[index]->GetParameter(static_cast<size_t>(RootBindings::kDepthTexture)).InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1, D3D12_SHADER_VISIBILITY_PIXEL);

		rootSignature_[index]->GetParameter(static_cast<size_t>(RootBindings::kMaterial)).InitializeAsConstantBuffer(0, D3D12_SHADER_VISIBILITY_PIXEL);

		rootSignature_[index]->Finalize(D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		pipelineState_[index] = new yunity::PipelineState(Device::GetInstance()->GetDevice(), rootSignature_[index]);

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

		pipelineState_[index]->SetInputLayout(inputLayoutDesc);
		pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kVS, yunity::ShaderCompiler::GetInstance()->Get("Fullscreen", yunity::ShaderCompiler::ShaderType::kVS));
		switch (static_cast<yunity::PostEffects>(index))
		{
		case yunity::PostEffects::kGrayscale:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("Grayscale", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		case yunity::PostEffects::kVignetting:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("Vignette", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		case yunity::PostEffects::kSmoothing:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("BoxFilter", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		case yunity::PostEffects::kGaussianFilter:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("GaussianFilter", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		case yunity::PostEffects::kOutline:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("DepthBasedOutline", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		case yunity::PostEffects::kRadialBlur:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("RadialBlur", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		case yunity::PostEffects::kDissolve:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("CopyImage", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		case yunity::PostEffects::kRandom:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("CopyImage", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		case yunity::PostEffects::kHSVFilter:
			pipelineState_[index]->SetShader(yunity::PipelineState::ShaderType::kPS, yunity::ShaderCompiler::GetInstance()->Get("HSVFilter", yunity::ShaderCompiler::ShaderType::kPS));
			break;
		default:
			break;
		}

		pipelineState_[index]->SetBlendState(blendDesc);
		pipelineState_[index]->SetRasterizerState(rasterizerDesc);
		pipelineState_[index]->SetDepthStencilState(depthStencilDesc);
		pipelineState_[index]->SetRenderTargetFormat(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, DXGI_FORMAT_D24_UNORM_S8_UINT);
		pipelineState_[index]->SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		pipelineState_[index]->SetSampleMask(D3D12_DEFAULT_SAMPLE_MASK);
		pipelineState_[index]->Finalize();

	}



}

void yunity::PostEffect::Initalize()
{
	depthBuffe_ = std::make_unique<yunity::DepthBuffer>();
	depthBuffe_->Initialize(DXGI_FORMAT_D24_UNORM_S8_UINT);
	CreateResorce();
	CreateRTV();
	CreateSRV();
	InitializeMaterial();
}

void yunity::PostEffect::Finalize()
{
	for (uint32_t index = 0; index <= static_cast<uint32_t>(yunity::PostEffects::kCount); index++) {
		if (pipelineState_[index]) {
			delete pipelineState_[index];
			pipelineState_[index] = nullptr;
		}
		if (rootSignature_[index]) {
			delete rootSignature_[index];
			rootSignature_[index] = nullptr;
		}

	}
}

void yunity::PostEffect::SetGraphicsRootSignature(uint32_t index)
{
	yunity::DirectXCore::GetInstance()->GetCommandList()->SetGraphicsRootSignature(rootSignature_[index]->GetSignature());
}

void yunity::PostEffect::SetPipelineState(uint32_t index)
{
	yunity::DirectXCore::GetInstance()->GetCommandList()->SetPipelineState(pipelineState_[index]->GetPipelineStateObject());
}

void yunity::PostEffect::CRTV(uint32_t index)
{
	float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
	yunity::DirectXCore::GetInstance()->GetCommandList()->ClearRenderTargetView(cpuDescHandleRTV_[index], clearColor, 0, nullptr);
}



void yunity::PostEffect::ClearRenderTargetView(uint32_t index)
{
	float clearColor[] = { kRenderTargetClearValue.x, kRenderTargetClearValue.y, kRenderTargetClearValue.z, kRenderTargetClearValue.w };
	yunity::DirectXCore::GetInstance()->GetCommandList()->ClearRenderTargetView(cpuDescHandleRTV_[index], clearColor, 0, nullptr);
}

void yunity::PostEffect::OMSetRenderTargets(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(depthBuffe_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
	yunity::DirectXCore::GetInstance()->GetCommandList()->OMSetRenderTargets(1, &cpuDescHandleRTV_[index], false, &dsvHandle);
}

void yunity::PostEffect::ClearDepthStencilView()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(depthBuffe_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	yunity::DirectXCore::GetInstance()->GetCommandList()->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void yunity::PostEffect::SetGraphicsRootDescriptorTable(RootBindings binding, uint32_t index)
{
	switch (binding)
	{
	case yunity::PostEffect::RootBindings::kTexture:
		yunity::DirectXCore::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(static_cast<UINT>(binding), gpuDescHandleSRV_[index]);
		break;
	case yunity::PostEffect::RootBindings::kDepthTexture:
		yunity::DirectXCore::GetInstance()->GetCommandList()->SetGraphicsRootDescriptorTable(static_cast<UINT>(RootBindings::kDepthTexture), depthTextureGpuDescHandleSRV_);
		break;
	}
}

void yunity::PostEffect::SetMaterial(uint32_t index)
{
	SetMaterialData(index);
	DirectXCore::GetInstance()->GetCommandList()->SetGraphicsRootConstantBufferView(static_cast<UINT>(RootBindings::kMaterial), materialResource_[index]->GetGPUVirtualAddress());
}

void yunity::PostEffect::SetMaterialData(uint32_t index)
{
	materialData_[index]->projectionInverse = Inverse(CameraManager::GetInstance()->GetCamera()->GetProjectionMatrix());
}

void yunity::PostEffect::Adjustment()
{
#ifdef _DEBUG
	ImGui::Begin("PostEffect");
	ImGui::DragFloat ("Hue", &materialData_[static_cast<uint32_t>(PostEffects::kHSVFilter)]->hue, 0.01f, -1.0f, 1.0f);
	ImGui::DragFloat("Saturation", &materialData_[static_cast<uint32_t>(PostEffects::kHSVFilter)]->saturation, 0.01f, -1.0f, 1.0f);
	ImGui::DragFloat("Value", &materialData_[static_cast<uint32_t>(PostEffects::kHSVFilter)]->value, 0.01f, -1.0f, 1.0f);
	ImGui::End();
#endif
}

void yunity::PostEffect::CreateResorce()
{

	for (uint32_t index = 0; index < static_cast<uint32_t>(PostEffects::kCount); index++) {
		renderTextureResource_[index] = CreateRenderTextureResource(WindowsAPI::kWindowWidth, WindowsAPI::kWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);
	}

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

void yunity::PostEffect::CreateRTV()
{
	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	DescriptorHeap* rtvDescriptorHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kRTV);
	// ディスクリプタヒープのハンドルを取得
	for (uint32_t index = 0; index < static_cast<uint32_t>(PostEffects::kCount); index++) {
		cpuDescHandleRTV_[index] = rtvDescriptorHeap_->Alloc().GetCPUHandle();
		Device::GetInstance()->GetDevice()->CreateRenderTargetView(renderTextureResource_[index].Get(), &renderTargetViewDesc, cpuDescHandleRTV_[index]);
	}
}

void yunity::PostEffect::CreateSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;

	DescriptorHeap* srvDescriptorHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kSRV);
	for (uint32_t index = 0; index < static_cast<uint32_t>(PostEffects::kCount); index++) {
		DescriptorHandle descriptorHandleSRV_ = srvDescriptorHeap_->Alloc();
		cpuDescHandleSRV_[index] = descriptorHandleSRV_.GetCPUHandle();
		gpuDescHandleSRV_[index] = descriptorHandleSRV_.GetGPUHandle();
		Device::GetInstance()->GetDevice()->CreateShaderResourceView(renderTextureResource_[index].Get(), &renderTextureSrvDesc, cpuDescHandleSRV_[index]);
	}

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

void yunity::PostEffect::InitializeMaterial()
{
	for (uint32_t index = 0; index < static_cast<uint32_t>(PostEffects::kCount); index++) {
		materialResource_[index] = CreateBufferResource(sizeof(Material));
		materialData_[index] = nullptr;
		materialResource_[index]->Map(0, nullptr, reinterpret_cast<void**>(&materialData_[index]));
		materialData_[index]->projectionInverse = MakeIdentity4x4();
	}

}

Microsoft::WRL::ComPtr<ID3D12Resource> yunity::PostEffect::CreateRenderTextureResource(uint32_t width, uint32_t hight, DXGI_FORMAT format, const Vector4& clearColor)
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

Microsoft::WRL::ComPtr<ID3D12Resource> yunity::PostEffect::CreateBufferResource(size_t sizeInBytes)
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
	//ID3D12Resource* resource = nullptr;
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	result = Device::GetInstance()->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));
	return resource;
}
