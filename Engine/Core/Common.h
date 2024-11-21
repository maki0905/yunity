#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <wrl.h>
#include <cassert>

namespace yunity {
	/// <summary>
	///  定数バッファ生成
	/// </summary>
	/// <param name="sizeInBytes"></param>
	/// <returns>サイズ</returns>
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
}