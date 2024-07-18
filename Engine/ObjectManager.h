#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "Object3D.h"
#include "Camera.h"
#include "World.h"
#include "MoveFloor.h"

class ObjectManager {
public:
	static ObjectManager* GetInstance();
	void Initialize();
	void Update(const std::string& fileName);
	void Draw(const std::string& fileName);
	void Load(const std::string& fileName, Camera* camera, World* world);
	std::vector<std::unique_ptr<Object3D>> GetObjects(const std::string& fileName);
	//std::vector<Object3D*> GetObj(const std::string& fileName, const std::string& modelName);
	Vector3 GetPos(const std::string& fileName, const std::string& modelName);

private:
	ObjectManager() = default;
	~ObjectManager() = default;
	ObjectManager(const ObjectManager&) = delete;
	const ObjectManager& operator=(const ObjectManager&) = delete;
private:
	//std::vector<Object3D*> objectVector_;
	std::unordered_map<std::string, std::vector<std::unique_ptr<Object3D>>>objects_;
	//std::map<std::string, Object3D*> objects_;
};