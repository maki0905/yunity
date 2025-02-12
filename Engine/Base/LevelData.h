#pragma once

#include <vector>
#include <string>
#include "Vector3.h"
#include "Collider.h"
#include "Tag.h"
#include "Joint.h"

namespace yunity {
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
			std::vector<uint32_t> jointIDs;
			Tag tag_ = Tag::kDefalt;
		};

		struct PlayerSpawnData {
			Vector3 translation;
			Vector3 rotation;
		};

		struct EnemySpawnData {
			std::string fileName;
			Vector3 translation;
			Vector3 rotation;
		};


	public:
		std::vector<ObjectData> objects;
		std::vector<PlayerSpawnData> players;
		std::vector<EnemySpawnData> enemies;
	};
}
