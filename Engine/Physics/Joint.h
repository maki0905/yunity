#pragma once

#include "Body.h"

namespace yunity {
	class Body;
}

namespace yunity{
	/*
	* @brief ジョイントクラス
	*/
	class Joint {
	public:
		virtual ~Joint() {};

		/// <summary>
		/// 解決
		/// </summary>
		virtual void Solve() = 0;

        Body* GetBody(int index) {
            if (index == 0) {
                return bodyA_;
            }
            else {
                return bodyB_;
            }
        };

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
}
