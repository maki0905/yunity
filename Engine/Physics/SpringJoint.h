#pragma once

#include "Body.h"
#include "Joint.h"

namespace yunity {
	/*
	* @brief スプリングジョイントクラス
	*/
	class SpringJoint : public Joint {
	public:
		/// <summary>
		/// スプリングジョイント生成
		/// </summary>
		/// <param name="bodyA"></param>
		/// <param name="bodyB"></param>
		void CreateSpringJoint(Body* bodyA, Body* bodyB);

		/// <summary>
		/// 有効軸設定
		/// </summary>
		/// <param name="index"></param>
		/// <param name="onOff"></param>
		void EnableSpring(int index, bool onOff);

		/// <summary>
		/// バネ係数設定
		/// </summary>
		/// <param name="index"></param>
		/// <param name="stiffness"></param>
		void SetStiffness(int index, float stiffness);

		/// <summary>
		/// 減衰設定
		/// </summary>
		/// <param name="index"></param>
		/// <param name="damping"></param>
		void SetDamping(int index, float damping);

		/// <summary>
		/// 自然長設定
		/// </summary>
		void SetEquilibriumPoint();

		/// <summary>
		/// 自然長設定
		/// </summary>
		/// <param name="index"></param>
		void SetEquilibriumPoint(int index);

		/// <summary>
		/// 自然長設定
		/// </summary>
		/// <param name="index"></param>
		/// <param name="value"></param>
		void SetEquilibriumPoint(int index, float value);

		/// <summary>
		/// 解決
		/// </summary>
		void Solve() override;

		/// <summary>
		/// バネの運動
		/// </summary>
		/// <param name="bodyA"></param>
		/// <param name="bodyB"></param>
		/// <param name="naturalLength"></param>
		/// <param name="stiffness"></param>
		/// <param name="dampingCoefficient"></param>
		/// <returns></returns>
		Vector3 Spring(Body* bodyA, Body* bodyB, float naturalLength, float stiffness, float dampingCoefficient);
	private:
		bool springEnabled_[3];
		float equilibriumPoint_[3];
		float stiffness_[3];
		float dampingCoefficient_[3];

	};
}