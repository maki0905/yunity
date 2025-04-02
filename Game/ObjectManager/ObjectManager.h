#pragma once
#include "BaseObjectManager.h"

#include "MoveFloor/MoveFloor.h"
#include "TV/TV.h"
#include "Coin/Coin.h"
#include "ForceField/ForceField.h"
#include "RotationCube/RotationCube.h"
#include "SpringBoard/SpringBoard.h"
#include "Signboard/Signboard.h"
#include "SelectTV/SelectTV.h"
#include "CheckPoint/CheckPoint.h"
#include "Player.h"

class ObjectManager : public yunity::BaseObjectManager {
public:
	void CreatePlayer(yunity::Camera* camera, yunity::World* world);
	void Load(const std::string& objectFileName, yunity::Camera* camera, yunity::World* world, const std::string& jointFileName = "") override;
};

