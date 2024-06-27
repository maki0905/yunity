#pragma once

#include <map>
#include <vector>
#include <string>

#include "Object3D.h"
#include "Camera.h"
#include "World.h"

class ObjectManager {
public:
	static ObjectManager* GetInstance();
	void Initialize();
	void Load(const std::string& fileName, Camera* camera, World* world);
	std::vector<Object3D*> GetObjects(const std::string& fileName);
	std::vector<Object3D*> GetObj(const std::string& fileName, const std::string& modelName);

private:
	ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager&) = delete;
	const ObjectManager& operator=(const ObjectManager&) = delete;
private:
	//std::vector<Object3D*> objectVector_;
	std::map<std::string, std::vector<Object3D*>>objects_;
	//std::map<std::string, Object3D*> objects_;
};