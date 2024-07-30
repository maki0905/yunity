#include "CommonData.h"

#include "EventTrigger.h"

CommonData* CommonData::GetInstance()
{
	static CommonData instance;
	return &instance;
}

void CommonData::Initialize()
{
	isGoal_ = false;
}

std::string CommonData::GetStageName()
{
	std::string result;

	switch (stageNum_)
	{
	case EventTrigger::SerialNumber::kStage1:
		result = "stage1";
		break;
	case EventTrigger::SerialNumber::kStage2:
		result = "stage2";
		break;
	case EventTrigger::SerialNumber::kStage3:
		result = "stage3";
		break;
	}

	return result;
}
