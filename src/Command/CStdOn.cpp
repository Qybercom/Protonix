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
	return this->_name == name;
}

bool Command::CStdOn::CommandSerialize() {
	this->_output = this->_name;

	return true;
}

/*
void Command::CStdOn::CommandRecognized(ProtonixDevice* device, ProtonixDevicePort* port) {
}

bool Command::CStdOn::CommandParse(ProtonixDevice* device, ProtonixDevicePort* port, char b) {
	return true;
}
*/

void Command::CStdOn::CommandReset() {
	this->_output = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CStdOn::CommandFromDTO(DTO::DTOEventCommand* dto) {

}
#endif