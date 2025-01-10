#include "Signboard.h"

void Signboard::Update()
{
	wavelength_ += amplitude_;
	if (wavelength_ > signboardconstant_.limit || wavelength_ < -signboardconstant_.limit) {
		amplitude_ *= -1.0f;
	}
	worldTransform_.rotation_.z = wavelength_ * DegToRad();
	worldTransform_.UpdateMatrix();
}
