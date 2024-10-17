#pragma once

#include <array>
#include <d3d12.h>
#include <string>
#include <unordered_map>
#include <wrl.h>

#include "Core/DescriptorHeap.h"


/*
* @brief テクスチャマネージャ
*/
class TextureManager {
public:
	// デスクリプターの数
	static const size_t kNumDescriptors = 256;

	/// <summary>
	/// テクスチャ
	/// </summary>
	struct Texture {
		// テクスチャリソース
		Microsoft::WRL::ComPtr<ID3D12Resource> resource;
		// シェーダリソースビューのハンドル(CPU)
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダリソースビューのハンドル(GPU)
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		// 名前
		std::string name;
	};

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t Load(const std::string& fileName);

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// システム初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(std::string directoryPath = "Resources/");

	/// <summary>
	/// 全テクスチャリセット
	/// </summary>
	void ResetAll();

	/// <summary>
	/// リソース情報取得
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <returns>リソース情報</returns>
	const D3D12_RESOURCE_DESC GetResoureDesc(uint32_t textureHandle);

	/// <summary>
	/// デスクリプタテーブルをセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rootParamIndex">ルートパラメータ番号</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetGraphicsRootDescriptorTable(
		ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle);

private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	// デスクリプタサイズ
	UINT sDescriptorHandleIncrementSize_ = 0u;
	// ディレクトリパス
	std::string directoryPath_;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// 次に使うデスクリプタヒープの番号
	uint32_t indexNextDescriptorHeap_ = 0u;
	// テクスチャコンテナ
	std::array<Texture, kNumDescriptors> textures_;

	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> intermediateResources_;

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	uint32_t LoadInternal(const std::string& fileName);

	// SRV用ヒープ
	DescriptorHeap* srvHeap_;
};
