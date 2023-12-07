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

	enum class Name
	{
		BasicVS,
		BasicPS,

	};

public:
	static Shader* GetInstance();
	void DXCInitialize();

	ID3DBlob* Get(Name name);
	

private:
	void ShaderCompile();
	ID3DBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile);

	/// <summary>
	/// ログ
	/// </summary>
	/// <param name="message"></param>
	static void Log(const std::string& message);

private:
	Shader() = default;
	Shader(const Shader&) = delete;
	const Shader& operator=(const Shader&) = delete;

	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;



	ID3DBlob* basicVS_ = nullptr;
	ID3DBlob* basicPS_ = nullptr;


};

