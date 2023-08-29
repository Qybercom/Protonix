#include <Arduino.h>
#include <ArduinoJson.h>

#include "../ProtonixDTO.h"
#include "../ProtonixDeviceStatus.h"
#include "../ProtonixDeviceSensor.h"
#include "DTORequestDeviceStatus.h"

using namespace Qybercom::Protonix;



DTO::DTORequestDeviceStatus::DTORequestDeviceStatus() {
	this->Status(new ProtonixDeviceStatus());
}

DTO::DTORequestDeviceStatus::DTORequestDeviceStatus(ProtonixDeviceStatus* status) {
	this->Status(status);
}

void DTO::DTORequestDeviceStatus::Status(ProtonixDeviceStatus* status) {
	this->_status = status;
}

ProtonixDeviceStatus* DTO::DTORequestDeviceStatus::Status() {
	return this->_status;
}

void DTO::DTORequestDeviceStatus::DTOPopulate(ProtonixDTO* dto) {
}

void DTO::DTORequestDeviceStatus::DTOSerialize(JsonDocument& dto) {
	dto["data"]["summary"] = this->_status->Summary();

	ProtonixDeviceSensor** sensors = this->_status->Sensors();
	JsonArray sensors_out = dto["data"].createNestedArray("sensors");

	unsigned int i = 0;
	unsigned int size = this->_status->SensorCount();

	while (i < size) {
		JsonObject sensor = sensors_out.createNestedObject();

		sensor["id"] = sensors[i]->ID();
		sensor["value"] = sensors[i]->Value();
		sensor["active"] = sensors[i]->Active();
		sensor["failure"] = sensors[i]->Failure();

		i++;
	}
}

DTO::DTORequestDeviceStatus* DTO::DTORequestDeviceStatus::Reset(ProtonixDeviceStatus* status) {
	this->Status(status);

	return this;
}