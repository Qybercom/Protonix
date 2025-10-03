#include <Arduino.h>
#include <ArduinoJson.h>

#include "../../../Protonix.h"
#include "../../../ProtonixSensor.h"

#include "../AxionDTO.h"

#include "DTORequestDeviceData.h"

using namespace Qybercom::Protonix;

Profile::Axion::DTO::DTORequestDeviceData::DTORequestDeviceData (Protonix* device, bool includeMemory, bool includeHardware) {
	this->_device = device;
	this->_includeMemory = includeMemory;
	this->_includeHardware = includeHardware;
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
	dto["data"]["cpu"] = this->_device->CPUFrequency();
	dto["data"]["uptime"] = this->_device->TimerUptime()->RunTime();
	dto["data"]["firmware"] = this->_device->Firmware();

	if (this->_includeMemory) {
		JsonObject memory = dto["data"]["memory"].to<JsonObject>();
		memory["ram_free"] = this->_device->Memory()->RAMFree();
		memory["ram_used"] = this->_device->Memory()->RAMUsed();
		memory["ram_total"] = this->_device->Memory()->RAMTotal();
		memory["ram_fragmented"] = this->_device->Memory()->RAMFragmented();
		memory["flash_free"] = this->_device->Memory()->FlashFree();
		memory["flash_used"] = this->_device->Memory()->FlashUsed();
		memory["flash_total"] = this->_device->Memory()->FlashTotal();
	}

	JsonArray sensors_out = dto["data"]["sensors"].to<JsonArray>();
	List<ProtonixSensor*> &sensors = this->_device->Sensors();
	for (ProtonixSensor* sensor : sensors) {
		JsonObject sItem = sensors_out.add<JsonObject>();

		sItem["id"] = sensor->ID();
		sItem["value"] = sensor->Value();
		sItem["active"] = sensor->Active();
		sItem["failure"] = sensor->Failure();
		sItem["state"] = sensor->State();
	}

	if (this->_includeHardware) {
		JsonArray hardware_out = dto["data"]["hardware"].to<JsonArray>();
		List<IProtonixHardware*> &hardware = this->_device->Hardware();
		for (IProtonixHardware* hw : hardware) {
			JsonObject hwItem = hardware_out.add<JsonObject>();

			hwItem["id"] = hw->HardwareID();
			hwItem["summary"] = hw->HardwareSummary();

			JsonArray capabilities_out = hwItem["capabilities"].to<JsonArray>();
			List<ProtonixHardwareCapability*> &capabilities = hw->HardwareCapabilities();
			for (ProtonixHardwareCapability* capability : capabilities) {
				JsonObject capabilityItem = capabilities_out.add<JsonObject>();

				capabilityItem["kind"] = capability->Kind();
				capabilityItem["id"] = capability->ID();
				capabilityItem["value"] = capability->Value();
				capabilityItem["comment"] = capability->Comment();
			}
		}
	}

	dto["data"]["registry"] = String(this->_device->Registry()->Raw());
	#endif
}