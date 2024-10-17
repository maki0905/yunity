#pragma once
#include <string>


enum Format {
	obj,
	gltf,
};

/// <summary>
/// フォーマット追加
/// </summary>
/// <param name="filePath"></param>
/// <param name="format"></param>
/// <returns></returns>
std::string AddToFormat(const std::string& filePath, Format format);