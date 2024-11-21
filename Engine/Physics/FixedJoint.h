#pragma once

#include "Body.h"
#include "Joint.h"

namespace yunity {
	/*
	* @brief 固定ジョイントクラス
	*/
	class FixedJoint : public Joint {
	public:
		/// <summary>
		/// 固定ジョイント生成
		/// </summary>
		/// <param name="bodyA"></param>
		/// <param name="bodyB"></param>
		void CreateFixedJoint(Body* bodyA, Body* bodyB);

		/// <summary>
		/// ジョイント解消
		/// </summary>
		void Clear();

		/// <summary>
		/// 解決
		/// </summary>
		void Solve() override;

	private:
		Vector3 initialPositionOffset;
		Vector3 initialRotationOffset;

	};
}
