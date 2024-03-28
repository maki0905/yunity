#pragma once

#include "Block.h"
#include "CSV.h"
#include "World.h"

class BlockManager {
public:
	void Initialize(Camera* camera, World* world);

	void Update(World* world);

	void Draw();

private:
	void Add(World* world);
	void Save();
	void Delete();
private:
	Camera* camera_;
	World* world_;
	std::vector<Block*> blocks_;
	std::stringstream* file_;

};