#include "ShaderCompiler.h"

#include <cassert>
//#include "externals/DirectXTex/DirectXTex.h"
#include "DirectXTex/DirectXTex.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dxcompiler.lib")


ShaderCompiler* ShaderCompiler::GetInstance()
{
	static ShaderCompiler instance;

	return &instance;
}

void ShaderCompiler::Initialize()
{
	HRESULT result = S_FALSE;
	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(result));

	result = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(result));

	ShaderCompile();
}

//void Shader::DXCInitialize()
//{
//	HRESULT result = S_FALSE;
//	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
//	assert(SUCCEEDED(result));
//	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
//	assert(SUCCEEDED(result));
//
//	result = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
//	assert(SUCCEEDED(result));
//
//	ShaderCompile();
//
//}

ID3DBlob* ShaderCompiler::Get(FileName name, ShaderType shaderType)
{
	ID3DBlob* result = nullptr;
	switch (shaderType)
	{
	case ShaderCompiler::ShaderType::kVS:
		result = vsblob_[static_cast<size_t>(name)];
		break;
	case ShaderCompiler::ShaderType::kPS:
		result = psblob_[static_cast<size_t>(name)];
		break;
	default:
		break;
	}

	return result;

}

void ShaderCompiler::ShaderCompile()
{
	/*basicVS_ = CompileShader(L"Resources/Shaders/Object3d.VS.hlsl", L"vs_6_0");
	assert(basicVS_ != nullptr);
	basicPS_ = CompileShader(L"Resources/Shaders/Object3d.PS.hlsl", L"ps_6_0");
	assert(basicPS_ != nullptr);*/

	vsblob_[static_cast<size_t>(FileName::kBasic)] = CompileShader(L"Resources/Shaders/Object3d.VS.hlsl", L"vs_6_0");
	assert(vsblob_[static_cast<size_t>(FileName::kBasic)]);
	psblob_[static_cast<size_t>(FileName::kBasic)] = CompileShader(L"Resources/Shaders/Object3d.PS.hlsl", L"ps_6_0");
	assert(psblob_[static_cast<size_t>(FileName::kBasic)]);

	vsblob_[static_cast<size_t>(FileName::kSprite)] = CompileShader(L"Resources/Shaders/SpriteVS.hlsl", L"vs_6_0");
	assert(vsblob_[static_cast<size_t>(FileName::kSprite)]);
	psblob_[static_cast<size_t>(FileName::kSprite)] = CompileShader(L"Resources/Shaders/SpritePS.hlsl", L"ps_6_0");
	assert(psblob_[static_cast<size_t>(FileName::kSprite)]);

	vsblob_[static_cast<size_t>(FileName::kLine)] = CompileShader(L"Resources/Shaders/LineVS.hlsl", L"vs_6_0");
	assert(vsblob_[static_cast<size_t>(FileName::kLine)]);
	psblob_[static_cast<size_t>(FileName::kLine)] = CompileShader(L"Resources/Shaders/LinePS.hlsl", L"ps_6_0");
	assert(psblob_[static_cast<size_t>(FileName::kLine)]);

	vsblob_[static_cast<size_t>(FileName::kParticle)] = CompileShader(L"Resources/Shaders/ParticleVS.hlsl", L"vs_6_0");
	assert(vsblob_[static_cast<size_t>(FileName::kParticle)]);
	psblob_[static_cast<size_t>(FileName::kParticle)] = CompileShader(L"Resources/Shaders/ParticlePS.hlsl", L"ps_6_0");
	assert(psblob_[static_cast<size_t>(FileName::kParticle)]);
}

ID3DBlob* ShaderCompiler::CompileShader(const std::wstring& filePath, const wchar_t* profile)
{
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; //UTF8の文字コードであることを通知

	LPCWSTR arguments[] = {
		filePath.c_str(), //コンパイル対象のhlslファイル名
		L"-E", L"main", //エントリーポイントの指定。基本的にmain以外にはしない
		L"-T", profile, //ShaderProfileの設定
		L"-Zi", L"-Qembed_debug", //デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr", //メモリレイアウトは行優先
	};
	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer, //読み込んだファイル
		arguments, //コンパイルオプション
		_countof(arguments), //コンパイルオプションの数
		includeHandler_.Get(), //includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult) //コンパイル結果
	);
	//コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		//警告・エラーダメゼッタイ
		assert(false);
	}

	//コンパイル結果から実行用のバイナリ部分を取得
	ID3DBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	//Log(ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;
}

void ShaderCompiler::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}