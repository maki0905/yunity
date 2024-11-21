#pragma once
#include "BaseObjectManager.h"

class ObjectManager : public yunity::BaseObjectManager {
public:
	void Load(const std::string& fileName, yunity::Camera* camera, yunity::World* world) override;
};