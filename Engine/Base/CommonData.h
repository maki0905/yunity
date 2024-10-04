#pragma once

#include <string>
#include "EventTrigger.h"

enum class  Scene
{
	kTitle,
	kSelect,
	kStage,

};

class CommonData {

public:
	static CommonData* GetInstance();

	void Initialize();

	std::string GetStageName();

	bool isGoal_;
	uint32_t stageNum_ = -1;

	Scene scene_;


private:
	CommonData() = default;
	~CommonData() = default;
	CommonData(const CommonData&) = default;
	const CommonData& operator=(const CommonData&) = delete;
};