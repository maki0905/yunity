#pragma once

#include <vector>
#include <string>
#include "Vector3.h"
#include "Collider.h"
#include "Tag.h"
#include "Joint.h"

namespace yunity {
	/*
	* @brief ジョイントデータクラス
	*/
	class JointData {
	public:
		struct BaseJointParameter {
			uint32_t id;
			JointType type = JointType::kDefalt;
		};
		struct SpringJointParmameter : public BaseJointParameter {
			bool springEnabled[3];
			float equilibriumPoint[3];
			float stiffness[3];
			float dampingCoefficient[3];
		};
		struct PulleyJointParmameter : public BaseJointParameter
		{
			Vector3 groundAnchorA;
			Vector3 anchorA;
			Vector3 groundAnchorB;
			Vector3 anchorB;
			float ratio;
		};

	public:
		std::vector<BaseJointParameter> joints;
		std::vector<SpringJointParmameter> springJoints;
		std::vector<PulleyJointParmameter> pulleyJoints;
	};
}
