#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <d3dcompiler.h>
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

/*
* @brief シェーダーコンパイラークラス
*/
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
		kLuminanceBasedOutline,
		kDepthBasedOutline,
		kSkinning,
		kCount

	};

	enum class ShaderType {
		kVS,
		kPS
	};

public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ShaderCompiler* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	//void DXCInitialize();

	//ID3DBlob* Get(FileName name, ShaderType shaderType);
	
	/// <summary>
	/// シェーダー取得
	/// </summary>
	/// <param name="shaderName"></param>
	/// <param name="shaderType"></param>
	/// <returns></returns>
	ID3DBlob* Get(std::string shaderName, ShaderType shaderType);

private:
	/// <summary>
	/// 全シェーダー生成
	/// </summary>
	void ShaderCompile();

	/// <summary>
	/// シェーダー読み込み
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="profile"></param>
	/// <returns></returns>
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

	std::unordered_map<std::string, ID3DBlob*> vsblob_;
	std::unordered_map<std::string, ID3DBlob*> psblob_;

};

