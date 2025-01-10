#pragma once

#include "Object3D.h"


/*
* @brief 看板クラス
*/
class Signboard : public yunity::Object3D {
	struct  Signboardconstant {
		const float limit = 10.0f;
		const float addition = 0.5f;
		const Vector3 pos = { 25.0f, -7.0f, 6.0f };
	};
public:
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
private:
	Signboardconstant signboardconstant_;
	float amplitude_ = signboardconstant_.addition;
	float wavelength_ = 0.0f;
};