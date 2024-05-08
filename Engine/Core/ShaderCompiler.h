#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <d3dcompiler.h>
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <memory>

class ShaderCompiler
{
public:

	enum class FileName
	{
		kBasic,
		kSprite,
		kLine,
		kParticle,
		kFullscreen,
		kGrayscale,
		kVignette,
		kBoxFilter,
		kGaussianFilter,
		kCount

	};

	enum class ShaderType {
		kVS,
		kPS
	};

public:
	static ShaderCompiler* GetInstance();

	void Initialize();
	//void DXCInitialize();

	ID3DBlob* Get(FileName name, ShaderType shaderType);
	

private:
	void ShaderCompile();
	ID3DBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile);

	/// <summary>
	/// ログ
	/// </summary>
	/// <param name="message"></param>
	static void Log(const std::string& message);

private:
	ShaderCompiler() = default;
	~ShaderCompiler() = default;
	ShaderCompiler(const ShaderCompiler&) = delete;
	const ShaderCompiler& operator=(const ShaderCompiler&) = delete;

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;

	ID3DBlob* vsblob_[static_cast<size_t>(FileName::kCount)];
	ID3DBlob* psblob_[static_cast<size_t>(FileName::kCount)];

	/*ID3DBlob* basicVS_ = nullptr;
	ID3DBlob* basicPS_ = nullptr;*/


};

