#pragma once
#include "BaseObjectManager.h"

#include "MoveFloor.h"
#include "TV.h"
#include "Coin.h"
#include "ForceField.h"
#include "RotationCube.h"
#include "SpringBoard.h"
#include "Signboard.h"
#include "SelectTV.h"
#include "CheckPoint.h"
#include "Player.h"

class ObjectManager : public yunity::BaseObjectManager {
public:
	void CreatePlayer(yunity::Camera* camera, yunity::World* world);
	void Load(const std::string& objectFileName, yunity::Camera* camera, yunity::World* world, const std::string& jointFileName = "") override;
};