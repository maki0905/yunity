#include "DirectXCore.h"

#include <iostream>
#include <cassert>
#include <thread>

#include "WindowsAPI.h"
#include "Device.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"
#include "DepthBuffer.h"
#include "BackBuffer.h"


#pragma comment(lib, "d3d12.lib")

yunity::DirectXCore* yunity::DirectXCore::GetInstance()
{
	static yunity::DirectXCore instance;

	return &instance;
}

void yunity::DirectXCore::Initialize()
{
	// FPS固定初期化
	InitializeFixFPS();

	windowWidth_ = WindowsAPI::GetInstance()->kWindowWidth;
	windowHeight_ = WindowsAPI::GetInstance()->kWindowHeight;

	// デバイス
	Device::GetInstance()->Initialize();

	// コマンド関連
	commandList_ = std::make_unique<CommandList>();
	commandList_->Create();
	commandQueue_ = std::make_unique<CommandQueue>();
	commandQueue_->Create();

	// スワップチェーン
	swapChain_ = std::make_unique<SwapChain>();
	swapChain_->Create(commandQueue_->GetCommandQueue());

	// 各種デスクリプタヒープ
	for (int i = 0; i < static_cast<int>(HeapType::kCount); i++) {
		descriptorHeaps_[i] = std::make_unique<DescriptorHeap>();
	}

	descriptorHeaps_[static_cast<int>(HeapType::kRTV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 12, false);
	descriptorHeaps_[static_cast<int>(HeapType::kSRV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 256, true);
	descriptorHeaps_[static_cast<int>(HeapType::kDSV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 3, false);

	backBuffer_ = std::make_unique<BackBuffer>();
	backBuffer_->Create(swapChain_->GetSwapChain());

	// Shader
	ShaderCompiler::GetInstance()->Initialize();

	RenderTexture::GetInstance()->InitializeGraphicsPipeline();
	RenderTexture::GetInstance()->Initalize();
	RenderTexture::GetInstance()->Create();


	GraphicsCommon::GetInstance()->Initialize();

	GraphicsPipelineManager::GetInstance()->Initialize();

	shadowBuffer_ = std::make_unique<DepthBuffer>();
	shadowBuffer_->Initialize(DXGI_FORMAT_D32_FLOAT, D3D12_RESOURCE_STATE_GENERIC_READ);
	D3D12_SHADER_RESOURCE_VIEW_DESC depthTextureSrvDesc{};
	depthTextureSrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	depthTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	depthTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	depthTextureSrvDesc.Texture2D.MipLevels = 1;
	DescriptorHandle descriptorHandleSRV_ = descriptorHeaps_[static_cast<int>(HeapType::kSRV)]->Alloc();
	shadowCpuDescHandleSRV_ = descriptorHandleSRV_.GetCPUHandle();
	shadowGpuDescHandleSRV_ = descriptorHandleSRV_.GetGPUHandle();
	Device::GetInstance()->GetDevice()->CreateShaderResourceView(shadowBuffer_->GetDepthStencil(), &depthTextureSrvDesc, shadowCpuDescHandleSRV_);
}



void yunity::DirectXCore::PreDrawRenderTexture()
{
	RenderTexture::GetInstance()->OMSetRenderTargets();
	RenderTexture::GetInstance()->ClearRenderTargetView();
	RenderTexture::GetInstance()->ClearDepthStencilView();
	commandList_->RSSetViewports(float(windowWidth_), float(windowHeight_));
	commandList_->RSSetScissorRects(windowWidth_, windowHeight_);
}

void yunity::DirectXCore::PostDrawRenderTexture()
{
	commandList_->BarrierChange(RenderTexture::GetInstance()->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	RenderTexture::GetInstance()->Machining();
}

void yunity::DirectXCore::PreDrawSwapchain()
{
	commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	commandList_->OMSetRenderTargets(&backBuffer_->GetCpuDescHandleRTV()[swapChain_->GetSwapChain()->GetCurrentBackBufferIndex()]);
	commandList_->ClearRenderTargetView(Vector4(0.1f, 0.25f, 0.5f, 0.0f), backBuffer_->GetCpuDescHandleRTV()[swapChain_->GetSwapChain()->GetCurrentBackBufferIndex()]);
	commandList_->ClearDepthStencilView(RenderTexture::GetInstance()->GetDepthBuffer()->GetDescriptorHeap());
	commandList_->RSSetViewports(float(windowWidth_), float(windowHeight_));
	commandList_->RSSetScissorRects(windowWidth_, windowHeight_);
	RenderTexture::GetInstance()->Copy();
	commandList_->BarrierChange(RenderTexture::GetInstance()->GetResource(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

void yunity::DirectXCore::PostDrawSwapchain()
{
	HRESULT result = S_FALSE;
	commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList_->GetCommandList()->Close();


	// コマンドリストの実行
	Microsoft::WRL::ComPtr<ID3D12CommandList> cmdLists[] = { commandList_->GetCommandList() };
	ID3D12CommandList* rawCmdLists[] = { cmdLists[0].Get() };
	commandQueue_->GetCommandQueue()->ExecuteCommandLists(1, rawCmdLists);

	// バッファをフリップ
	result = swapChain_->GetSwapChain()->Present(1, 0);
#ifdef _DEBUG
	if (FAILED(result)) {
		Microsoft::WRL::ComPtr<ID3D12DeviceRemovedExtendedData> dred;

		result = Device::GetInstance()->GetDevice()->QueryInterface(IID_PPV_ARGS(&dred));
		assert(SUCCEEDED(result));

		// 自動パンくず取得
		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT autoBreadcrumbsOutput{};
		result = dred->GetAutoBreadcrumbsOutput(&autoBreadcrumbsOutput);
		assert(SUCCEEDED(result));
	}
#endif

	commandQueue_->WaitForCommandsToFinish();
	// FPS固定
	UpdateFixFPS();
	commandList_->CommandClear();
	/*for (auto& list : cmdLists) {
		list = nullptr;
	}*/
}

void yunity::DirectXCore::PreDrawShadow()
{
	commandList_->BarrierChange(shadowBuffer_->GetDepthStencil(), D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	commandList_->ClearDepthStencilView(shadowBuffer_->GetDescriptorHeap());
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(shadowBuffer_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
	commandList_->OMSetRenderTargets(0, nullptr, &dsvHandle);
	commandList_->RSSetViewports(float(windowWidth_), float(windowHeight_));
	commandList_->RSSetScissorRects(windowWidth_, windowHeight_);
}

void yunity::DirectXCore::PostDrawShadow()
{
	commandList_->BarrierChange(shadowBuffer_->GetDepthStencil(), D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_GENERIC_READ);
}

void yunity::DirectXCore::InitializeFixFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void yunity::DirectXCore::UpdateFixFPS()
{
	timeBeginPeriod(1);
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	static const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 62.0f));
	static const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	std::chrono::microseconds check = kMinCheckTime - elapsed;
	if (check > std::chrono::microseconds(0)) {
		std::chrono::microseconds waitTime = kMinTime - elapsed;

		auto waitStart = std::chrono::steady_clock::now();
		do {
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		} while (std::chrono::steady_clock::now() - waitStart < waitTime);
	}

	// 再計算して referenceTime_ を更新
	now = std::chrono::steady_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);
	reference_ = now;

	// microseconds から seconds に変換
	std::chrono::duration<float> deltaTime = std::chrono::duration<float>(elapsed.count() / 1000000.0f);

	timeEndPeriod(1);
}
