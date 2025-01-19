#pragma once

#include <string>
#include "EventTrigger.h"

enum class  Scene
{
	kTitle,
	kSelect,
	kStage,

};

enum class FlagState {
	kInStage,
	kPlay,
	kDeth,
	kReset,
	kResult,
	kGoal,
	kClear,
	kCount
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

	bool isGoal_;
	int stageNum_ = -1;
	Scene scene_;
	uint32_t hiScore_ = 0;
	yunity::WorldTransform start_;
	FlagState flagState_ = FlagState::kPlay;
private:
	CommonData() = default;
	~CommonData() = default;
	CommonData(const CommonData&) = default;
	const CommonData& operator=(const CommonData&) = delete;
};