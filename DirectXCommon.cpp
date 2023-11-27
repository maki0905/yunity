#include "DirectXCommon.h"
#include "SafeDelete.h"
#include <algorithm>
#include <cassert>
#include <vector>
#include <wrl.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;

DirectXCommon* DirectXCommon::GetInstance() {
	static DirectXCommon instance;
	return &instance;
}

void DirectXCommon::Initialize(WinApp* winApp, int32_t backBufferWidth, int32_t backBufferHeight) {
	// nullptrチェック
	assert(winApp);
	assert(4 <= backBufferWidth && backBufferWidth <= 4096);
	assert(4 <= backBufferHeight && backBufferHeight <= 4096);

	

	winApp_ = winApp;
	backBufferWidth_ = backBufferWidth;
	backBufferHeight_ = backBufferHeight;

	// DXGIデバイス初期化
	InitializeDXGIDevice();

	// コマンド関連初期化
	InitializeCommand();

	// スワップチェーンの生成
	CreateSwapChain();

	// レンダーターゲット生成
	CreateFinalRenderTargets();

	// 深度バッファ生成
	CreateDepthBuffer();

	// フェンス生成
	CreateFence();
}

void DirectXCommon::PreDraw() {
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// リソースバリアを変更（表示状態→描画対象）
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	hr_ = swapChain_->GetBuffer(bbIndex, IID_PPV_ARGS(&barrier.Transition.pResource));
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList_->ResourceBarrier(1, &barrier);

	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// レンダーターゲットをセット
	commandList_->OMSetRenderTargets(1, &rtvHandles_[bbIndex], false, &dsvHandle);

	// 全画面クリア
	ClearRenderTarget();
	// 深度バッファクリア
	ClearDepthBuffer();

	// ビューポートの設定
	D3D12_VIEWPORT viewport =
		D3D12_VIEWPORT(0.0f, 0.0f, float(backBufferWidth_), float(backBufferHeight_), D3D12_MIN_DEPTH, D3D12_MAX_DEPTH);
	commandList_->RSSetViewports(1, &viewport);
	// シザリング矩形の設定
	D3D12_RECT rect = D3D12_RECT(0, 0, backBufferWidth_, backBufferHeight_);
	commandList_->RSSetScissorRects(1, &rect);
}

void DirectXCommon::PostDraw() {
	HRESULT result;

	// リソースバリアを変更（描画対象→表示状態）
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	hr_ = swapChain_->GetBuffer(bbIndex, IID_PPV_ARGS(&barrier.Transition.pResource));
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList_->ResourceBarrier(1, &barrier);

	// 命令のクローズ
	commandList_->Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { commandList_.Get() }; // コマンドリストの配列
	commandQueue_->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ
	result = swapChain_->Present(1, 0);
#ifdef _DEBUG
	if (FAILED(result)) {
		ComPtr<ID3D12DeviceRemovedExtendedData> dred;

		result = device_->QueryInterface(IID_PPV_ARGS(&dred));
		assert(SUCCEEDED(result));

		// 自動パンくず取得
		D3D12_DRED_AUTO_BREADCRUMBS_OUTPUT autoBreadcrumbsOutput{};
		result = dred->GetAutoBreadcrumbsOutput(&autoBreadcrumbsOutput);
		assert(SUCCEEDED(result));
	}
#endif

	// コマンドリストの実行完了を待つ
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	if (fence_->GetCompletedValue() != fenceVal_) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence_->SetEventOnCompletion(fenceVal_, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	commandAllocator_->Reset(); // キューをクリア
	commandList_->Reset(commandAllocator_.Get(),
		nullptr); // 再びコマンドリストを貯める準備
}

void DirectXCommon::ClearRenderTarget() {
	UINT bbIndex = swapChain_->GetCurrentBackBufferIndex();

	// 全画面クリア        Red   Green Blue  Alpha
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 0.0f }; // 青っぽい色
	commandList_->ClearRenderTargetView(rtvHandles_[bbIndex], clearColor, 0, nullptr);
}

void DirectXCommon::ClearDepthBuffer() {
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

int32_t DirectXCommon::GetBackBufferWidth() const { return backBufferWidth_; }

int32_t DirectXCommon::GetBackBufferHeight() const { return backBufferHeight_; }

void DirectXCommon::InitializeDXGIDevice() {
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		debugController->EnableDebugLayer();
	}
	// DREDレポートをオンに
	ComPtr<ID3D12DeviceRemovedExtendedDataSettings> dredSettings;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&dredSettings)))) {
		dredSettings->SetAutoBreadcrumbsEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
		dredSettings->SetPageFaultEnablement(D3D12_DRED_ENABLEMENT_FORCED_ON);
	}
#endif

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] = {
	  D3D_FEATURE_LEVEL_12_1,
	  D3D_FEATURE_LEVEL_12_0,
	  D3D_FEATURE_LEVEL_11_1,
	  D3D_FEATURE_LEVEL_11_0,
	};

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory_));
	assert(SUCCEEDED(result));

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter;
	// パフォーマンスが高いものから順に、全てのアダプターを列挙する
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(
		i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) !=
		DXGI_ERROR_NOT_FOUND;
		i++) {
		// 動的配列に追加する
		adapters.push_back(tmpAdapter);
	}

	// ハードウェアアダプタを優先的に処理
	std::stable_sort(
		adapters.begin(), adapters.end(),
		[](const ComPtr<IDXGIAdapter4>& lhs, const ComPtr<IDXGIAdapter4>& rhs) {
			DXGI_ADAPTER_DESC3 lhsDesc;
			lhs->GetDesc3(&lhsDesc); // アダプターの情報を取得
			DXGI_ADAPTER_DESC3 rhsDesc;
			rhs->GetDesc3(&rhsDesc); // アダプターの情報を取得
			return (lhsDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE) <
				(rhsDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE);
		});

	result = S_FALSE;
	D3D_FEATURE_LEVEL featureLevel;
	for (int i = 0; i < adapters.size(); i++) {
		// デバイスを生成
		for (int levelIndex = 0; levelIndex < _countof(levels); levelIndex++) {
			result =
				D3D12CreateDevice(adapters[i].Get(), levels[levelIndex], IID_PPV_ARGS(&device_));
			if (SUCCEEDED(result)) {
				// デバイスを生成できた時点でループを抜ける
				featureLevel = levels[levelIndex];
				break;
			}
		}

		// このアダプタで生成できてたら完了
		if (SUCCEEDED(result)) {
			break;
		}
	}

	assert(!!device_);
	assert(SUCCEEDED(result));

#ifdef _DEBUG
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// 抑制するエラー
		D3D12_MESSAGE_ID denyIds[] = {
			/*
			 * Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			 * https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			 */
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE };
		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したエラーの表示を抑制する
		infoQueue->PushStorageFilter(&filter);
		// エラー時にブレークを発生させる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}
#endif
}

void DirectXCommon::CreateSwapChain() {
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = backBufferWidth_;
	swapChainDesc.Height = backBufferHeight_;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式を一般的なものに
	swapChainDesc.SampleDesc.Count = 1;                // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファとして使えるように
	swapChainDesc.BufferCount = 2;                      // バッファ数を２つに設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は速やかに破棄
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; // ティアリングサポート
	ComPtr<IDXGISwapChain1> swapChain1;
	//HWND hwnd = winApp_->GetHwnd();
	result = dxgiFactory_->CreateSwapChainForHwnd(
		commandQueue_.Get(), winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(result));

	// SwapChain4を得る
	swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));
	assert(SUCCEEDED(result));

	// OSが行うAlt+Enterのフルスクリーンは制御不能なので禁止
	dxgiFactory_->MakeWindowAssociation(
		winApp_->GetHwnd(), DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
}

void DirectXCommon::InitializeCommand() {
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = device_->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateFinalRenderTargets() {
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapChain_->GetDesc(&swcDesc);
	assert(SUCCEEDED(result));

	// 各種設定をしてディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = device_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeap_));
	assert(SUCCEEDED(result));

	//const uint32_t desriptorSizeSRV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	const uint32_t desriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//const uint32_t desriptorSizeDSV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	// 裏表の２つ分について
	backBuffers_.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers_.size(); i++) {
		// スワップチェーンからバッファを取得
		result = swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i]));
		assert(SUCCEEDED(result));

		// ディスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += (desriptorSizeRTV * i);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		// シェーダーの計算結果をSRGBに変換して書き込む
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		renderTargetViewDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &renderTargetViewDesc, handle);
	}

	// レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	rtvHandles_[0] = rtvHeap_->GetCPUDescriptorHandleForHeapStart();
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

void DirectXCommon::CreateDepthBuffer() {
	// DepthStemcilTextureをウィンドウのサイズで作成
	depthBuffer_ = CreateDepthStencilTextureResource();

	// 深度ビュー用デスクリプタヒープ作成。DSV用のヒープでディスクリプタの数は1。
	dsvHeap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false);

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture
	// DSVHeapの先頭にDSVをつくる
	device_->CreateDepthStencilView(depthBuffer_.Get(), &dsvDesc, dsvHeap_->GetCPUDescriptorHandleForHeapStart());
}

Microsoft::WRL::ComPtr<ID3D12Resource> DirectXCommon::CreateDepthStencilTextureResource()
{
	HRESULT hr = S_FALSE;
	Microsoft::WRL::ComPtr<ID3D12Resource> result = nullptr;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Width = backBufferWidth_; // Textureの幅
	depthResourceDesc.Height = backBufferHeight_; // Textureの高さ
	depthResourceDesc.MipLevels = 1; // mipmapの数
	depthResourceDesc.DepthOrArraySize = 1; // 奥行き or 配列Textureの配列数
	depthResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // DepthStencilとして利用可能なフォーマット
	depthResourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	D3D12_CLEAR_VALUE depthClearValue;
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット。Resourceと合わせる

	hr = device_->CreateCommittedResource(
		&heapProps, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定
		&depthResourceDesc, // Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&result)
	);
	assert(SUCCEEDED(hr));

	return result;

}

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DirectXCommon::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	HRESULT hr = S_FALSE;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> result = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeap{};
	dsvDescriptorHeap.NumDescriptors = numDescriptors;                    // 深度ビューは1つ
	dsvDescriptorHeap.Type = heapType; // デプスステンシルビュー
	dsvDescriptorHeap.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = device_->CreateDescriptorHeap(&dsvDescriptorHeap, IID_PPV_ARGS(&result));
	assert(SUCCEEDED(hr));
	return result;
}

void DirectXCommon::CreateFence() {
	HRESULT result = S_FALSE;

	// フェンスの生成
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	assert(SUCCEEDED(result));
}

