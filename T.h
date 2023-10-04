#pragma once

#include "Triangle.h"
#include "WorldTransform.h"
class T
{
public:
	// ‰Šú‰»
	void Initialize();

	// XV
	void Update();

	// •`‰æ
	void Draw();

private:
	WorldTransform worldTransform_;
	Triangle* triangle_;

};

