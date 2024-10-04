#pragma once

#include <vector>
#include <string>
#include "Vector3.h"
#include "Collider.h"


class LevelData {
public:
	struct ObjectData {
		bool empth = false;
		Vector3 scaling;
		Vector3 rotation;
		Vector3 translation;
		std::string fileName;
		Collider::Shape shape;
		Vector3 size;
		Vector3 center;
		bool isTrigger;
		uint32_t attribute;
		float mass;
		float drag;
		float miu;
		uint32_t frictionCombine;
		float bounciness;
		uint32_t bounceCombine;
		int serialNumber = -1;
	};

public:
	std::vector<ObjectData> objects;


};