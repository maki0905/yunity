#include "EventTrigger.h"

#include "CommonData.h"

void EventTrigger::OnTriggerEvent()
{
	switch (serialNumber_)
	{
	case EventTrigger::SerialNumber::kStage1:
		CommonData::GetInstance()->stageNum_ = 0;
		break;
	case EventTrigger::SerialNumber::kStage2:
		CommonData::GetInstance()->stageNum_ = 1;
		break;
	case EventTrigger::SerialNumber::kStage3:
		CommonData::GetInstance()->stageNum_ = 2;
		break;
	}

}
