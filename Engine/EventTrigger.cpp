#include "EventTrigger.h"

#include "CommonData.h"

void EventTrigger::OnTriggerEvent(Body* body)
{
	switch (serialNumber_)
	{
	case EventTrigger::SerialNumber::kStage1:
		CommonData::GetInstance()->stageNum_ = SerialNumber::kStage1;
		break;
	case EventTrigger::SerialNumber::kStage2:
		CommonData::GetInstance()->stageNum_ = SerialNumber::kStage2;
		break;
	case EventTrigger::SerialNumber::kStage3:
		CommonData::GetInstance()->stageNum_ = SerialNumber::kStage3;
		break;
	}

}
