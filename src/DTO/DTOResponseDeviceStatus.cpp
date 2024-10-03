#include <Arduino.h>
#include <ArduinoJson.h>

#include "../ProtonixDTO.h"
#include "DTOResponseDeviceStatus.h"

using namespace Qybercom::Protonix;



void DTO::DTOResponseDeviceStatus::Status(unsigned short status) {
	this->_status = status;
}

unsigned short DTO::DTOResponseDeviceStatus::Status() {
	return this->_status;
}

void DTO::DTOResponseDeviceStatus::DTOPopulate(ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	//if (data.containsKey("status"))
	if (data["status"].is<unsigned short>())
		this->Status(data["status"]);
}

void DTO::DTOResponseDeviceStatus::DTOSerialize(JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}

unsigned short DTO::DTOResponseDeviceStatus::DTOResponseStatus() {
	return this->_status;
}