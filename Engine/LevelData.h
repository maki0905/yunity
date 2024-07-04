#pragma once

#include <vector>
#include <string>
#include "Vector3.h"
#include "Collider.h"


class LevelData {
public:
	struct ObjectData {
		Vector3 scaling;
		Vector3 rotation;
		Vector3 translation;
		std::string fileName;
		Collider::Shape shape;
		Vector3 size;
		Vector3 center;
		bool isTrigger;
	};

public:
	std::vector<ObjectData> objects;


};