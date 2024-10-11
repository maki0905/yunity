#pragma once

#include "Object3D.h"

class EventTrigger : public Object3D{
public:
	enum class SerialNumber {
		kStage1,
		kStage2, 
		kStage3,
	};
public:

	virtual void OnTriggerEvent();

	void SetSerialNumber(uint32_t serialNumber) { serialNumber_ = static_cast<SerialNumber>(serialNumber); }
	void SetSerialNumber(SerialNumber serialNumber) { serialNumber_ = serialNumber; }
private:
	SerialNumber serialNumber_;

};