#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"
#include "../ProtonixDevicePort.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

#include "CStdOff.h"

using namespace Qybercom::Protonix;



Command::CStdOff::CStdOff () {
	this->_init("std:off");
}

bool Command::CStdOff::CommandRecognize (ProtonixDevice* device, ProtonixDevicePort* port, String name) {
	(void)device;
	(void)port;

	return this->_name == name;
}

bool Command::CStdOff::CommandSerialize () {
	this->_output = this->_name;

	return true;
}

void Command::CStdOff::CommandReset () {
	this->_output = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CStdOff::CommandFromDTO (DTO::DTOEventCommand* dto) {
	(void)dto;
}
#endif