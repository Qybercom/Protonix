#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

#include "CStdRegistry.h"

using namespace Qybercom::Protonix;



Command::CStdRegistry::CStdRegistry() {
	this->_init("std:registry");
}

Command::CStdRegistry* Command::CStdRegistry::Key(String key) {
	this->_key = key;

	return this;
}

String Command::CStdRegistry::Key() {
	return this->_key;
}

Command::CStdRegistry* Command::CStdRegistry::Value(String value) {
	this->_value = value;

	return this;
}

String Command::CStdRegistry::Value() {
	return this->_value;
}

bool Command::CStdRegistry::CommandRecognize(ProtonixDevice* device, ProtonixDevicePort* port, String name) {
	(void)device;
	(void)port;

	String n = name.substring(0, 12);
	if (n != this->_name) return false;

	unsigned int i = 13;
	unsigned int l = name.length();
	bool setKey = false;

	String key = "";
	String value = "";

	while (i < l) {
		if (setKey) value += name[i];
		else {
			if (name[i] != ';') key += name[i];
			else {
				setKey = true;

				this->Key(key);
			}
		}

		i++;
	}

	this->Value(value);

	return true;
}

bool Command::CStdRegistry::CommandSerialize() {
	this->_output = "";
	this->_output += this->_name;
	this->_output += ":";

	return true;
}

void Command::CStdRegistry::CommandReset() {
	this->_output = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CStdRegistry::CommandFromDTO(DTO::DTOEventCommand* dto) {
	(void)dto;
}
#endif