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
	dto["data"]["active"] = this->_device->Active();
	dto["data"]["state"] = this->_device->State();
	dto["data"]["summary"] = this->_device->Summary();

	// temporary...
	#if defined(ESP32) || defined(ESP8266)
	dto["data"]["uptime"] = this->_device->TimerUptime()->RunTime();
	dto["data"]["firmware"] = this->_device->Firmware();

	JsonObject memory = dto["data"]["memory"].to<JsonObject>();
	memory["ram_free"] = this->_device->Memory()->RAMFree();
	memory["ram_used"] = this->_device->Memory()->RAMUsed();
	memory["ram_total"] = this->_device->Memory()->RAMTotal();
	memory["ram_fragmented"] = this->_device->Memory()->RAMFragmented();
	memory["flash_free"] = this->_device->Memory()->FlashFree();
	memory["flash_used"] = this->_device->Memory()->FlashUsed();
	memory["flash_total"] = this->_device->Memory()->FlashTotal();

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
	#endif
}