#include "DepthBuffer.h"

#include <cassert>

#include "WindowsAPI.h"
#include "DirectXCore.h"
#include "DescriptorHeap.h"
#include "Device.h"

#pragma comment(lib, "d3d12.lib")

yunity::DepthBuffer::DepthBuffer()
{
	commandList_ = DirectXCore::GetInstance()->GetCommandList();
}

void yunity::DepthBuffer::Initialize(DXGI_FORMAT format, D3D12_RESOURCE_STATES state)
{
	// DepthStemcilTextureをウィンドウのサイズで作成
	depthStencilResource_ = CreateDepthStencilTextureResource(format, state);

	// 深度ビュー用デスクリプタヒープ作成。DSV用のヒープでディスクリプタの数は1。
	dsvHeap_ = DirectXCore::GetInstance()->GetDescriptorHeap(DirectXCore::HeapType::kDSV);

	DescriptorHandle descriptorHandle = dsvHeap_->Alloc();

	cpuHandle_ = descriptorHandle.GetCPUHandle();
	gpuHandle_ = descriptorHandle.GetGPUHandle();

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = format; // Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // 2dTexture
	// DSVHeapの先頭にDSVをつくる
	Device::GetInstance()->GetDevice()->CreateDepthStencilView(depthStencilResource_.Get(), &dsvDesc,descriptorHandle.GetCPUHandle());
}

void yunity::DepthBuffer::ClearDepthView()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetHeapPointer()->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}


Microsoft::WRL::ComPtr<ID3D12Resource> yunity::DepthBuffer::CreateDepthStencilTextureResource(DXGI_FORMAT format, D3D12_RESOURCE_STATES state)
{
	HRESULT hr = S_FALSE;
	Microsoft::WRL::ComPtr<ID3D12Resource> result = nullptr;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に作る

	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Width = WindowsAPI::GetInstance()->kWindowWidth; // Textureの幅
	depthResourceDesc.Height = WindowsAPI::GetInstance()->kWindowHeight; // Textureの高さ
	depthResourceDesc.MipLevels = 1; // mipmapの数
	depthResourceDesc.DepthOrArraySize = 1; // 奥行き or 配列Textureの配列数
	depthResourceDesc.Format = format; // DepthStencilとして利用可能なフォーマット
	depthResourceDesc.SampleDesc.Count = 1; // サンプリングカウント。1固定
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; // 2次元
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // DepthStencilとして使う通知

	D3D12_CLEAR_VALUE depthClearValue;
	depthClearValue.DepthStencil.Depth = 1.0f; // 1.0f(最大値)でクリア
	depthClearValue.Format = format; // フォーマット。Resourceと合わせる

	hr = Device::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProps, // Heapの設定
		D3D12_HEAP_FLAG_NONE, // Heapの特殊な設定
		&depthResourceDesc, // Resourceの設定
		state, // 深度値を書き込む状態にしておく
		&depthClearValue, // Clear最適値
		IID_PPV_ARGS(&result)
	);
	assert(SUCCEEDED(hr));

	return result;
}

