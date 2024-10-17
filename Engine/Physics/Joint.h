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