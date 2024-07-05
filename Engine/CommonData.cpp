#include "CommonData.h"

CommonData* CommonData::GetInstance()
{
	static CommonData instance;
	return &instance;
}
