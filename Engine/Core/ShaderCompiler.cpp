#include "ShaderCompiler.h"

#include <cassert>
#include "DirectXTex/DirectXTex.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dxcompiler.lib")


yunity::ShaderCompiler* yunity::ShaderCompiler::GetInstance()
{
	static ShaderCompiler instance;

	return &instance;
}

void yunity::ShaderCompiler::Initialize()
{
	HRESULT result = S_FALSE;
	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(result));
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(result));

	result = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(result));

	vsblob_.clear();
	psblob_.clear();

	ShaderCompile();
}

ID3DBlob* yunity::ShaderCompiler::Get(std::string shaderName, ShaderType shaderType)
{
	ID3DBlob* result = nullptr;
	switch (shaderType)
	{
	case ShaderCompiler::ShaderType::kVS:
		result = vsblob_[shaderName];
		break;
	case ShaderCompiler::ShaderType::kPS:
		result = psblob_[shaderName];
		break;
	default:
		break;
	}

	return result;
}

void yunity::ShaderCompiler::ShaderCompile()
{

	vsblob_["Object3d"] = CompileShader(L"Resources/Shaders/Object3d.VS.hlsl", L"vs_6_0");
	assert(vsblob_["Object3d"]);
	psblob_["Object3d"] = CompileShader(L"Resources/Shaders/Object3d.PS.hlsl", L"ps_6_0");
	assert(psblob_["Object3d"]);

	vsblob_["Sprite"] = CompileShader(L"Resources/Shaders/SpriteVS.hlsl", L"vs_6_0");
	assert(vsblob_["Sprite"]);
	psblob_["Sprite"] = CompileShader(L"Resources/Shaders/SpritePS.hlsl", L"ps_6_0");
	assert(psblob_["Sprite"]);

	vsblob_["Line"] = CompileShader(L"Resources/Shaders/Line.VS.hlsl", L"vs_6_0");
	assert(vsblob_["Line"]);
	psblob_["Line"] = CompileShader(L"Resources/Shaders/Line.PS.hlsl", L"ps_6_0");
	assert(psblob_["Line"]);

	vsblob_["Primitive"] = CompileShader(L"Resources/Shaders/Primitive.VS.hlsl", L"vs_6_0");
	assert(vsblob_["Primitive"]);
	psblob_["Primitive"] = CompileShader(L"Resources/Shaders/Primitive.PS.hlsl", L"ps_6_0");
	assert(psblob_["Primitive"]);

	vsblob_["Particle"] = CompileShader(L"Resources/Shaders/ParticleVS.hlsl", L"vs_6_0");
	assert(vsblob_["Particle"]);
	psblob_["Particle"] = CompileShader(L"Resources/Shaders/ParticlePS.hlsl", L"ps_6_0");
	assert(psblob_["Particle"]);

	vsblob_["Fullscreen"] = CompileShader(L"Resources/Shaders/Fullscreen.VS.hlsl", L"vs_6_0");
	assert(vsblob_["Fullscreen"]);

	psblob_["CopyImage"] = CompileShader(L"Resources/Shaders/CopyImage.PS.hlsl", L"ps_6_0");
	assert(psblob_["CopyImage"]);

	psblob_["Grayscale"] = CompileShader(L"Resources/Shaders/Grayscale.PS.hlsl", L"ps_6_0");
	assert(psblob_["Grayscale"]);

	psblob_["Vignette"] = CompileShader(L"Resources/Shaders/Vignette.PS.hlsl", L"ps_6_0");
	assert(psblob_["Vignette"]);

	psblob_["BoxFilter"] = CompileShader(L"Resources/Shaders/BoxFilter.PS.hlsl", L"ps_6_0");
	assert(psblob_["BoxFilter"]);

	psblob_["GaussianFilter"] = CompileShader(L"Resources/Shaders/GaussianFilter.PS.hlsl", L"ps_6_0");
	assert(psblob_["GaussianFilter"]);

	psblob_["LuminanceBasedOutline"] = CompileShader(L"Resources/Shaders/LuminanceBasedOutline.PS.hlsl", L"ps_6_0");
	assert(psblob_["LuminanceBasedOutline"]);

	psblob_["DepthBasedOutline"] = CompileShader(L"Resources/Shaders/DepthBasedOutline.PS.hlsl", L"ps_6_0");
	assert(psblob_["DepthBasedOutline"]);

	psblob_["RadialBlur"] = CompileShader(L"Resources/Shaders/RadialBlur.PS.hlsl", L"ps_6_0");
	assert(psblob_["RadialBlur"]);

	vsblob_["Skinning"] = CompileShader(L"Resources/Shaders/SkinningObject3d.VS.hlsl", L"vs_6_0");
	assert(vsblob_["Skinning"]);

	vsblob_["Skybox"] = CompileShader(L"Resources/Shaders/Skybox.VS.hlsl", L"vs_6_0");
	assert(vsblob_["Skybox"]);
	psblob_["Skybox"] = CompileShader(L"Resources/Shaders/Skybox.PS.hlsl", L"ps_6_0");
	assert(psblob_["Skybox"]);

	psblob_["HSVFilter"] = CompileShader(L"Resources/Shaders/HSVFilter.PS.hlsl", L"ps_6_0");
	assert(psblob_["HSVFilter"]);


}

ID3DBlob* yunity::ShaderCompiler::CompileShader(const std::wstring& filePath, const wchar_t* profile)
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

void yunity::ShaderCompiler::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}
