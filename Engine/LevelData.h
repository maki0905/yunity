#pragma once

#include <vector>
#include <string>
#include "Vector3.h"


class LevelData {
public:
	struct ObjectData {
		Vector3 scaling;
		Vector3 rotation;
		Vector3 translation;
		std::string fileName;
	};

public:
	std::vector<ObjectData> objects;


};