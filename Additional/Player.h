#pragma once

#include <memory>

#include "Collider.h"
#include "WorldTransform.h"
#include "Camera.h"
#include "PrimitiveDrawer.h"

class Player : public Collider
{
public:
	void Initialize();

	void Update();

	void Draw(const Camera& camera);




private:
	WorldTransform worldTransfrom_;
	std::unique_ptr<PrimitiveDrawer> HitBox_;

};

