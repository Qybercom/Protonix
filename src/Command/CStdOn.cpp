#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"
#include "../ProtonixDevicePort.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

#include "CStdOn.h"

using namespace Qybercom::Protonix;



Command::CStdOn::CStdOn() {
	this->_init("std:on");
}

bool Command::CStdOn::CommandRecognize(ProtonixDevice* device, ProtonixDevicePort* port, String name) {
	(void)device;
	(void)port;

	return this->_name == name;
}

bool Command::CStdOn::CommandSerialize() {
	this->_output = this->_name;

	return true;
}

void Command::CStdOn::CommandReset() {
	this->_output = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CStdOn::CommandFromDTO(DTO::DTOEventCommand* dto) {
	(void)dto;
}
#endif