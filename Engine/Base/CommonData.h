#pragma once

#include <string>
#include "EventTrigger.h"

enum class  Scene
{
	kTitle,
	kSelect,
	kStage,

};

/*
* @brief 共通データクラス
*/
class CommonData {

public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static CommonData* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ステージ名取得
	/// </summary>
	/// <returns></returns>
	std::string GetStageName();

	bool isGoal_;
	int stageNum_ = -1;
	Scene scene_;
	uint32_t hiScore_ = 0;
private:
	CommonData() = default;
	~CommonData() = default;
	CommonData(const CommonData&) = default;
	const CommonData& operator=(const CommonData&) = delete;
};