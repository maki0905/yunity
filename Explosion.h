#pragma once

#include <memory>

#include "Model.h"
#include "WorldTransform.h"


class Explosion
{
public:
	void Initialize(Model* model, const Vector3& translation, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);
	bool GetIsDead() { return isDead_; }
private:
	WorldTransform worldTransform_;
	Vector3 velocity_;
	Model* model_;
	bool isDead_;
};

