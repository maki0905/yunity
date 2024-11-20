#pragma once
#include "BaseObjectManager.h"

class ObjectManager : public BaseObjectManager {
public:
	void Load(const std::string& fileName, Camera* camera, World* world) override;
};