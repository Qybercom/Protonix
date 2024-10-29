#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

#include "CStdFirmware.h"

using namespace Qybercom::Protonix;



Command::CStdFirmware::CStdFirmware() {
	this->_init("std:firmware");
}

Command::CStdFirmware* Command::CStdFirmware::Action(String action) {
	this->_action = action;

    return this;
}

String Command::CStdFirmware::Action() {
	return this->_action;
}

bool Command::CStdFirmware::ActionUpdate() {
	return this->_action == "update";
}

Command::CStdFirmware* Command::CStdFirmware::Version(String version) {
	this->_version = version;

    return this;
}

String Command::CStdFirmware::Version() {
	return this->_version;
}

bool Command::CStdFirmware::VersionLatest() {
	return this->_version == "";
}

bool Command::CStdFirmware::CommandRecognize(ProtonixDevice* device, ProtonixDevicePort* port, String name) {
	(void)device;
	(void)port;

	String n = name.substring(0, 12);
	if (n != this->_name) return false;

	unsigned int i = 13;
	unsigned int l = name.length();
	bool setAction = false;

	String action = "";
	String version = "";

	while (i < l) {
		if (setAction) version += name[i];
		else {
			if (name[i] != ';') action += name[i];
			else {
				setAction = true;

				this->Action(action);
			}
		}

		i++;
	}

    this->Action(action);
    this->Version(version);

    return true;
}

bool Command::CStdFirmware::CommandSerialize() {
	this->_output = "";
	this->_output += this->_name;
	this->_output += ":";

	return true;
}

void Command::CStdFirmware::CommandReset() {
	this->_output = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CStdFirmware::CommandFromDTO(DTO::DTOEventCommand* dto) {
	(void)dto;
}
#endif