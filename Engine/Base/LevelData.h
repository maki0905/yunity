#pragma once

#include <vector>
#include <string>
#include "Vector3.h"
#include "Collider.h"
#include "Tag.h"
#include "Joint.h"

/*
* @brief レベルエディターデータクラス
*/
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
		int jointPair_ = -1;
		JointType jointType_ = JointType::kDefalt;
		bool springEnabled_[3];
		float equilibriumPoint_[3];
		float stiffness_[3];
		float dampingCoefficient_[3];
		Vector3 groundAnchor_;
		Vector3 anchor_;
		float ratio_;
		Tag tag_ = Tag::kDefalt;
	};


public:
	std::vector<ObjectData> objects;


};