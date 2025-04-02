#include "Signboard.h"

#include "GlobalVariables.h"

void Signboard::Initialize()
{
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "Signboard";
	signboardconstant_.limit = globalVariables->GetFloatValue(groupName, "Limit");
	signboardconstant_.addition = globalVariables->GetFloatValue(groupName, "Addition");
}

void Signboard::Update()
{
	wavelength_ += amplitude_;
	if (wavelength_ > signboardconstant_.limit || wavelength_ < -signboardconstant_.limit) {
		amplitude_ *= -1.0f;
	}
	worldTransform_.rotation_.z = wavelength_ * DegToRad();
	worldTransform_.UpdateMatrix();

#ifdef _DEBUG
	yunity::GlobalVariables* globalVariables = yunity::GlobalVariables::GetInstance();
	const char* groupName = "Signboard";
	signboardconstant_.limit = globalVariables->GetFloatValue(groupName, "Limit");
	signboardconstant_.addition = globalVariables->GetFloatValue(groupName, "Addition");
#endif // _DEBUG

}
