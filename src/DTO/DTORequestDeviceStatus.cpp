#include <Arduino.h>
#include <ArduinoJson.h>

#include "../ProtonixDTO.h"
#include "../ProtonixDeviceStatus.h"
#include "../ProtonixDeviceSensor.h"
#include "DTORequestDeviceStatus.h"

using namespace Qybercom::Protonix;



DTO::DTORequestDeviceStatus::DTORequestDeviceStatus () {
	this->Status(new ProtonixDeviceStatus());
	this->Registry("");

	this->_trailing = false;
}

DTO::DTORequestDeviceStatus::DTORequestDeviceStatus (ProtonixDeviceStatus* status, String registry) {
	this->Status(status);
	this->Registry(registry);

	this->_trailing = false;
}

void DTO::DTORequestDeviceStatus::Status (ProtonixDeviceStatus* status) {
	this->_status = status;
}

ProtonixDeviceStatus* DTO::DTORequestDeviceStatus::Status () {
	return this->_status;
}

void DTO::DTORequestDeviceStatus::Registry (String registry) {
	this->_registry = registry;
}

String DTO::DTORequestDeviceStatus::Registry () {
	return this->_registry;
}

void DTO::DTORequestDeviceStatus::DTOPopulate (ProtonixDTO* dto) {
	(void)dto;
}

void DTO::DTORequestDeviceStatus::DTOSerialize (JsonDocument& dto) {
	dto["data"]["firmware"] = this->_status->Firmware();
	dto["data"]["enabled"] = this->_status->On();
	dto["data"]["state"] = this->_status->State();
	dto["data"]["uptime"] = this->_status->Uptime();
	dto["data"]["summary"] = this->_status->Summary();

	JsonArray sensors_out = dto["data"]["sensors"].to<JsonArray>();

	List<ProtonixDeviceSensor*> &sensors = this->_status->Sensors();

	for (ProtonixDeviceSensor* sensor : sensors) {
		JsonObject item = sensors_out.add<JsonObject>();

		item["id"] = sensor->ID();
		item["value"] = sensor->Value();
		item["active"] = sensor->Active();
		item["failure"] = sensor->Failure();
		item["state"] = sensor->State();
	}

	dto["data"]["registry"] = String(this->_registry + "");
}

String DTO::DTORequestDeviceStatus::DTOSerializeFilter (String raw) {
	return raw;
}

DTO::DTORequestDeviceStatus* DTO::DTORequestDeviceStatus::Reset (ProtonixDeviceStatus* status) {
	this->Status(status);

	return this;
}