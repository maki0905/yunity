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

DirectXCore* DirectXCore::GetInstance()
{
	static DirectXCore instance;

	return &instance;
}

void DirectXCore::Initialize()
{
	// FPS固定初期化
	InitializeFixFPS();

	windowWidth_ = WindowsAPI::GetInstance()->kWindowWidth;
	windowHeight_ = WindowsAPI::GetInstance()->kWindowHeight;

	// デバイス
	/*device_ = Device::GetInstance();
	device_->Initialize();*/
	Device::GetInstance()->Initialize();

	// コマンド関連
	//commandList_ = std::make_unique<CommandList>();
	commandList_ = new CommandList();
	commandList_->Create();
	//commandQueue_ = std::make_unique<CommandQueue>();
	commandQueue_ = new CommandQueue();
	commandQueue_->Create();

	// スワップチェーン
	//swapChain_ = std::make_unique<SwapChain>();
	swapChain_ = new SwapChain();
	swapChain_->CreateSwapChain(commandQueue_->GetCommandQueue());

	// 各種デスクリプタヒープ
	for (int i = 0; i < static_cast<int>(HeapType::kCount); i++) {
		descriptorHeaps_[i] = std::make_unique<DescriptorHeap>();
	}

	descriptorHeaps_[static_cast<int>(HeapType::kRTV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 11, false);
	descriptorHeaps_[static_cast<int>(HeapType::kSRV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, true);
	descriptorHeaps_[static_cast<int>(HeapType::kDSV)]->Create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 2, false);

	//// バッファ
	///*depthBuffer_ = new DepthBuffer();
	//depthBuffer_->Create();*/

	//backBuffer_ = std::make_unique<BackBuffer>();
	backBuffer_ = new BackBuffer();
	backBuffer_->Create(swapChain_->GetSwapChain());

	// Shader
	ShaderCompiler::GetInstance()->Initialize();

	RenderTexture::GetInstance()->InitializeGraphicsPipeline();
	RenderTexture::GetInstance()->Initalize();
	RenderTexture::GetInstance()->Create();


	GraphicsCommon::GetInstance()->Initialize();

	GraphicsPipelineManager::GetInstance()->Initialize();
}

void DirectXCore::Finalize()
{
	backBuffer_->Finalize();
	if (backBuffer_) {
		delete backBuffer_;
		backBuffer_ = nullptr;
	}

	if (swapChain_) {
		delete swapChain_;
		swapChain_ = nullptr;
	}

	commandQueue_->Finalize();
	if (commandQueue_) {
		delete commandQueue_;
		commandQueue_ = nullptr;
	}

	commandList_->Finalize();
	if (commandList_) {
		delete commandList_;
		commandList_ = nullptr;
	}
	/*if (commandList_) {
		delete commandList_;
	}
	if (commandQueue_) {
		delete commandQueue_;
	}
	if (swapChain_) {
		delete swapChain_;
	}
	if (backBuffer_) {
		delete backBuffer_;
	}
	if (depthBuffer_) {
		delete depthBuffer_;
	}*/
}

void DirectXCore::PreDrawRenderTexture()
{
	//commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//commandList_->OMSetRenderTargets(renderTexture_->GetCpuDescHandleRTV(), depthBuffer_->GetDescriptorHeap());
	RenderTexture::GetInstance()->OMSetRenderTargets();
	RenderTexture::GetInstance()->ClearRenderTargetView();
	RenderTexture::GetInstance()->ClearDepthStencilView();
	//commandList_->OMSetRenderTargets(renderTexture_->GetCpuDescHandleRTV(), renderTexture_->GetDepthBuffer()->GetDescriptorHeap());
	//commandList_->ClearRenderTargetView(renderTexture_->GetRenderTargetClearValue(), *renderTexture_->GetCpuDescHandleRTV());
	//commandList_->ClearDepthStencilView(/*depthBuffer_->GetDescriptorHeap()*/renderTexture_->GetDepthBuffer()->GetDescriptorHeap());
	commandList_->RSSetViewports(float(windowWidth_), float(windowHeight_));
	commandList_->RSSetScissorRects(windowWidth_, windowHeight_);
}

void DirectXCore::PostDrawRenderTexture()
{

	commandList_->BarrierChange(RenderTexture::GetInstance()->GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	RenderTexture::GetInstance()->Machining();

}

void DirectXCore::PreDrawSwapchain()
{
	commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	commandList_->OMSetRenderTargets(&backBuffer_->GetCpuDescHandleRTV()[swapChain_->GetSwapChain()->GetCurrentBackBufferIndex()]);
	commandList_->ClearRenderTargetView(Vector4(0.1f, 0.25f, 0.5f, 0.0f), backBuffer_->GetCpuDescHandleRTV()[swapChain_->GetSwapChain()->GetCurrentBackBufferIndex()]);
	commandList_->RSSetViewports(float(windowWidth_), float(windowHeight_));
	commandList_->RSSetScissorRects(windowWidth_, windowHeight_);
	RenderTexture::GetInstance()->Copy();
	commandList_->BarrierChange(RenderTexture::GetInstance()->GetResource(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	//backBuffer_->SetRenderTarget(swapChain_->GetSwapChain(), depthBuffer_->GetDescriptorHeap());
	//backBuffer_->ClearRenderTarget(swapChain_->GetSwapChain());
	//depthBuffer_->ClearDepthView();
}

void DirectXCore::PostDrawSwapchain()
{
	HRESULT result = S_FALSE;
	commandList_->BarrierChange(swapChain_->GetSwapChain(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	commandList_->GetCommandList()->Close();


	// コマンドリストの実行
	//ID3D12CommandList* cmdLists[] = { commandList_->GetCommandList() }; // コマンドリストの配列
	Microsoft::WRL::ComPtr<ID3D12CommandList> cmdLists[] = { commandList_->GetCommandList() };
	ID3D12CommandList* rawCmdLists[] = { cmdLists[0].Get() };
	commandQueue_->GetCommandQueue()->ExecuteCommandLists(1, rawCmdLists);
	//commandQueue_->GetCommandQueue()->ExecuteCommandLists(1, commandList_->GetCommandLists());

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



void DirectXCore::InitializeFixFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCore::UpdateFixFPS()
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
