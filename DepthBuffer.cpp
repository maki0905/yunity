#include "DepthBuffer.h"

#include <cassert>

#include "WindowsAPI.h"

#pragma comment(lib, "d3d12.lib")

DepthBuffer::DepthBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	device_ = device;
	commandList_ = commandList;
}

void DepthBuffer::Create()
{
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

void DepthBuffer::ClearDepthView()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle =
		D3D12_CPU_DESCRIPTOR_HANDLE(dsvHeap_->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	commandList_->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}


Microsoft::WRL::ComPtr<ID3D12Resource> DepthBuffer::CreateDepthStencilTextureResource()
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

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DepthBuffer::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	HRESULT hr = S_FALSE;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> result = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC dsvDescriptorHeap{};
	dsvDescriptorHeap.NumDescriptors = numDescriptors; // 深度ビューは1つ
	dsvDescriptorHeap.Type = heapType; // デプスステンシルビュー
	dsvDescriptorHeap.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = device_->CreateDescriptorHeap(&dsvDescriptorHeap, IID_PPV_ARGS(&result));
	assert(SUCCEEDED(hr));
	return result;
}
