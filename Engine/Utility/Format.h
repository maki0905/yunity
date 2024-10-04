#pragma once
#include <string>


enum Format {
	obj,
	gltf,
};

std::string AddToFormat(const std::string& filePath, Format format);