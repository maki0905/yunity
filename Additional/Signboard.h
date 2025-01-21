#pragma once

#include "Object3D.h"


/*
* @brief 看板クラス
*/
class Signboard : public yunity::Object3D {
	struct  Signboardconstant {
		float limit = 10.0f;
		float addition = 0.5f;
	};
public:
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
private:
	Signboardconstant signboardconstant_;
	float amplitude_ = signboardconstant_.addition;
	float wavelength_ = 0.0f;
};