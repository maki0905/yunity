#pragma once

#ifdef _DEBUG
#include <d3d12.h>
#include "imgui.h"
#include <wrl.h>
#endif // _DEBUG

class ImGuiManager
{
public:
	static ImGuiManager* GetInstance();

    /// <summary>
    /// 初期化
    /// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
private:
#ifdef _DEBUG
#endif // _DEBUG

private:
	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(const ImGuiManager&) = delete;
	const ImGuiManager& operator=(const ImGuiManager&) = delete;
};

