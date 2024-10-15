#pragma once

#include "Body.h"

class Joint {
public:
	virtual void Solve();
protected:
	Body* bodyA_;
	Body* bodyB_;

};