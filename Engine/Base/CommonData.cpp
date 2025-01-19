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
	start_.Initialize();
}
