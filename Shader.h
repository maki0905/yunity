#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <string>

class Shader
{
public:

	enum class ShaderName
	{
		BasicVS,
		BasicPS,

	};

public:
	static Shader* GetInstance();
	void DXCInitialize();

	IDxcBlob* Get(ShaderName name);
	

private:
	void ShaderCompile();
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile);

private:
	Shader() = default;
	Shader(const Shader&) = delete;
	const Shader& operator=(const Shader&) = delete;

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;



	Microsoft::WRL::ComPtr<IDxcBlob> basicVS_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcBlob> basicPS_ = nullptr;


};

