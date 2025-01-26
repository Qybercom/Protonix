#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"
#include "../ProtonixDevicePort.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

#include "CStdReboot.h"

using namespace Qybercom::Protonix;



Command::CStdReboot::CStdReboot() {
	this->_init("std:reboot");
}

bool Command::CStdReboot::CommandRecognize(ProtonixDevice* device, ProtonixDevicePort* port, String name) {
	(void)device;
	(void)port;

	return this->_name == name;
}

bool Command::CStdReboot::CommandSerialize() {
	this->_output = this->_name;

	return true;
}

void Command::CStdReboot::CommandReset() {
	this->_output = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CStdReboot::CommandFromDTO(DTO::DTOEventCommand* dto) {
	(void)dto;
}
#endif