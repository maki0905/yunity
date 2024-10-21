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
	case 0:
		result = "stage0";
		break;
	case 1:
		result = "stage2";
		break;
	case 2:
		result = "stage4";
		break;
	}

	return result;
}
