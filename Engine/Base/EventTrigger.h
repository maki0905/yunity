#pragma once

#include "Object3D.h"

/*
* @brief イベントトリガークラス
*/
class EventTrigger : public yunity::Object3D{
public:
	enum class SerialNumber {
		kStage1,
		kStage2, 
		kStage3,
	};
public:
	/// <summary>
	/// 衝突時のイベント
	/// </summary>
	void OnTriggerEvent() override;

	/// <summary>
	/// シリアルナンバーの設定
	/// </summary>
	/// <param name="serialNumber"></param>
	void SetSerialNumber(uint32_t serialNumber) { serialNumber_ = static_cast<SerialNumber>(serialNumber); }
	
	/// <summary>
	/// シリアルナンバーの設定
	/// </summary>
	/// <param name="serialNumber"></param>
	void SetSerialNumber(SerialNumber serialNumber) { serialNumber_ = serialNumber; }
private:
	SerialNumber serialNumber_;

};