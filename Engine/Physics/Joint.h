#pragma once

#include "Body.h"

/*
* @brief ジョイントクラス
*/
class Joint {
public:
	/// <summary>
	/// 解決
	/// </summary>
	virtual void Solve();
protected:
	Body* bodyA_;
	Body* bodyB_;

};

enum class JointType {
	kDefalt,
	kSpring,
	kPulley,
	kJointTypeCount
};