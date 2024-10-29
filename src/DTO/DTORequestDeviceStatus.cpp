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

DTO::DTORequestDeviceStatus::DTORequestDeviceStatus(ProtonixDeviceStatus* status, String registry) {
	this->Status(status);
    this->Registry(registry);
}

void DTO::DTORequestDeviceStatus::Status(ProtonixDeviceStatus* status) {
	this->_status = status;
}

ProtonixDeviceStatus* DTO::DTORequestDeviceStatus::Status() {
	return this->_status;
}

void DTO::DTORequestDeviceStatus::Registry(String registry) {
	this->_registry = registry;
}

String DTO::DTORequestDeviceStatus::Registry() {
	return this->_registry;
}

void DTO::DTORequestDeviceStatus::DTOPopulate(ProtonixDTO* dto) {
	(void)dto;
}

void DTO::DTORequestDeviceStatus::DTOSerialize(JsonDocument& dto) {
	dto["data"]["firmware"] = this->_status->Firmware();
	dto["data"]["registry"] = this->_registry;
	dto["data"]["state"] = this->_status->State();
	dto["data"]["enabled"] = this->_status->On();
	dto["data"]["summary"] = this->_status->Summary();

	ProtonixDeviceSensor** sensors = this->_status->Sensors();
	//JsonArray sensors_out = dto["data"].createNestedArray("sensors");
	JsonArray sensors_out = dto["data"]["sensors"].to<JsonArray>();

	unsigned int i = 0;
	unsigned int size = this->_status->SensorCount();

	while (i < size) {
		//JsonObject sensor = sensors_out.createNestedObject();
		JsonObject sensor = sensors_out.add<JsonObject>();

		sensor["id"] = sensors[i]->ID();
		sensor["value"] = sensors[i]->Value();
		sensor["active"] = sensors[i]->Active();
		sensor["failure"] = sensors[i]->Failure();
		sensor["state"] = sensors[i]->State();

		i++;
	}
}

DTO::DTORequestDeviceStatus* DTO::DTORequestDeviceStatus::Reset(ProtonixDeviceStatus* status) {
	this->Status(status);

	return this;
}