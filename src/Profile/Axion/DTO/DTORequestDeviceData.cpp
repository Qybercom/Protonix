#include <Arduino.h>
#include <ArduinoJson.h>

#include "../../../Protonix.h"
#include "../../../ProtonixSensor.h"

#include "../AxionDTO.h"

#include "DTORequestDeviceData.h"

using namespace Qybercom::Protonix;

Profile::Axion::DTO::DTORequestDeviceData::DTORequestDeviceData (Protonix* device) {
	this->_device = device;
}

void Profile::Axion::DTO::DTORequestDeviceData::AxionDTOPopulate (Profile::Axion::AxionDTO* dto) {
	(void)dto;
}

void Profile::Axion::DTO::DTORequestDeviceData::AxionDTOSerialize (JsonDocument& dto) {
	dto["data"]["uptime"] = this->_device->TimerUptime()->RunTime();
	dto["data"]["memory"] = this->_device->Memory()->RAMFree();
	dto["data"]["active"] = this->_device->Active();
	dto["data"]["state"] = this->_device->State();
	dto["data"]["firmware"] = this->_device->Firmware();
	dto["data"]["summary"] = this->_device->Summary();

	JsonArray sensors_out = dto["data"]["sensors"].to<JsonArray>();

	List<ProtonixSensor*> &sensors = this->_device->Sensors();

	for (ProtonixSensor* sensor : sensors) {
		JsonObject item = sensors_out.add<JsonObject>();

		item["id"] = sensor->ID();
		item["value"] = sensor->Value();
		item["active"] = sensor->Active();
		item["failure"] = sensor->Failure();
		item["state"] = sensor->State();
	}

	dto["data"]["registry"] = String(this->_device->Registry()->Raw());
}