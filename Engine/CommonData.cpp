#include "CommonData.h"

CommonData* CommonData::GetInstance()
{
	static CommonData instance;
	return &instance;
}

void CommonData::Initialize()
{
	isGoal_ = false;
	stageNum_ = 0;
}
