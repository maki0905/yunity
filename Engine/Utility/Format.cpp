#include "Format.h"

std::string AddToFormat(const std::string& filePath, Format format)
{
	std::string result;
	switch (format)
	{
	case obj:
		result = filePath + ".obj";
		break;
	case gltf:
		result = filePath + ".gltf";
		break;
	default:
		break;
	}

    return result;
}
