#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"
#include "../ProtonixDevicePort.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

#include "CCustom.h"

using namespace Qybercom::Protonix;



Command::CCustom::CCustom() {
	this->_init("custom");
}

Command::CCustom::CCustom(String cmd) {
	this->_init("custom");
	this->CMD(cmd);
}

void Command::CCustom::CMD(String cmd) {
	this->_cmd = cmd;
}

String Command::CCustom::CMD() {
	return this->_cmd;
}

bool Command::CCustom::CommandRecognize(ProtonixDevice* device, ProtonixDevicePort* port, String name) {
	(void)device;
	(void)port;

	String n = name.substring(0, 7);
	if (n != "custom:") return false;

	this->_cmd = name.substring(7, name.length());

	return true;
}

bool Command::CCustom::CommandSerialize() {
	this->_output = this->_name + ":" + this->_cmd;

	return true;
}

void Command::CCustom::CommandReset() {
	this->_output = "";
	this->_cmd = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CCustom::CommandFromDTO(DTO::DTOEventCommand* dto) {
	String cmd = dto->Name();
	this->_cmd = cmd.substring(7, cmd.length());
}
#endif