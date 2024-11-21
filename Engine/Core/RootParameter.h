#pragma once

#include <d3d12.h>
#include <wrl.h>
#include <memory>

class RootSignature;

namespace yunity {
	/*
	* @brief ルートパラメータクラス
	*/
	class RootParameter
	{
		friend class RootSignature;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		RootParameter();

		/// <summary>
		/// デストラクタ
		/// </summary>
		~RootParameter();

		/// <summary>
		/// クリア
		/// </summary>
		void Clear();

		/// <summary>
		/// Constants初期化
		/// </summary>
		/// <param name="shaderRegister"></param>
		/// <param name="numDwords"></param>
		/// <param name="shaderVisibility"></param>
		/// <param name="registerSpace"></param>
		void InitializeAsConstants(UINT shaderRegister, UINT numDwords, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);

		/// <summary>
		/// ConstantBuffer初期化
		/// </summary>
		/// <param name="shaderRegister"></param>
		/// <param name="shaderVisibility"></param>
		/// <param name="registerSpace"></param>
		void InitializeAsConstantBuffer(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);

		/// <summary>
		/// BufferSRV初期化
		/// </summary>
		/// <param name="shaderRegister"></param>
		/// <param name="shaderVisibility"></param>
		/// <param name="registerSpace"></param>
		void InitializeAsBufferSRV(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);

		/// <summary>
		/// BufferUAV初期化
		/// </summary>
		/// <param name="shaderRegister"></param>
		/// <param name="shaderVisibility"></param>
		/// <param name="registerSpace"></param>
		void InitializeAsBufferUAV(UINT shaderRegister, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rangeType"></param>
		/// <param name="shaderRegister"></param>
		/// <param name="numDescriptors"></param>
		/// <param name="shaderVisibility"></param>
		/// <param name="registerSpace"></param>
		void InitializeAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT shaderRegister, UINT numDescriptors, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL, UINT registerSpace = 0);

		/// <summary>
		/// DescriptorTable初期化
		/// </summary>
		/// <param name="rangeCount"></param>
		/// <param name="shaderVisibility"></param>
		void InitializeAsDescriptorTable(UINT rangeCount, D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL);

		/// <summary>
		/// TableRange設定
		/// </summary>
		/// <param name="rangeIndex"></param>
		/// <param name="rangeType"></param>
		/// <param name="shaderRegister"></param>
		/// <param name="numDescriptors"></param>
		/// <param name="registerSpace"></param>
		void SetTableRange(UINT rangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE rangeType, UINT shaderRegister, UINT numDescriptors, UINT registerSpace = 0);

		const D3D12_ROOT_PARAMETER& operator() (void) const { return rootParameter_; }


	private:
		D3D12_ROOT_PARAMETER rootParameter_;

	};


}
