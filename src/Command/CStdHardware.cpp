#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

#include "CStdHardware.h"

using namespace Qybercom::Protonix;



Command::CStdHardware::CStdHardware () {
	this->_init("std:hardware");
}

Command::CStdHardware* Command::CStdHardware::ID (String id) {
	this->_id = id;

	return this;
}

String Command::CStdHardware::ID () {
	return this->_id;
}

Command::CStdHardware* Command::CStdHardware::CMD (String cmd) {
	this->_cmd = cmd;

	return this;
}

String Command::CStdHardware::CMD () {
	return this->_cmd;
}

bool Command::CStdHardware::CommandRecognize (ProtonixDevice* device, ProtonixDevicePort* port, String name) {
	(void)device;
	(void)port;

	String n = name.substring(0, 12);
	if (n != this->_name) return false;

	unsigned int i = 13;
	unsigned int l = name.length();
	bool setID = false;

	String id = "";
	String cmd = "";

	while (i < l) {
		if (setID) cmd += name[i];
		else {
			if (name[i] != ';') id += name[i];
			else {
				setID = true;

				this->ID(id);
			}
		}

		i++;
	}

	this->ID(id);
	this->CMD(cmd);

	return true;
}

bool Command::CStdHardware::CommandSerialize () {
	this->_output = "";
	this->_output += this->_name;
	this->_output += ":";
	this->_output += this->_id;
	this->_output += ":";
	this->_output += this->_cmd;

	return true;
}

void Command::CStdHardware::CommandReset () {
	this->_output = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CStdHardware::CommandFromDTO (DTO::DTOEventCommand* dto) {
	(void)dto;
}
#endif