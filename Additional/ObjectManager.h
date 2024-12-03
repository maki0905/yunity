#pragma once
#include "BaseObjectManager.h"

#include "MoveFloor.h"
#include "TV.h"
#include "Coin.h"
#include "ForceField.h"
#include "Pillar.h"
#include "RotationCube.h"

class ObjectManager : public yunity::BaseObjectManager {
public:
	void Load(const std::string& fileName, yunity::Camera* camera, yunity::World* world) override;
};