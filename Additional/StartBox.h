#pragma once
#pragma once
#include "BaseObject.h"
class StartBox : public BaseObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera) override;
private:
};

