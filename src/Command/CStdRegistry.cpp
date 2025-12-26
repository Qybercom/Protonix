#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "CStdRegistry.h"

using namespace Qybercom::Protonix;

Command::CStdRegistry::CStdRegistry () {
	this->_init("std:registry");
}

Command::CStdRegistry* Command::CStdRegistry::Key (String key) {
	this->_key = key;

	return this;
}

String Command::CStdRegistry::Key () {
	return this->_key;
}

Command::CStdRegistry* Command::CStdRegistry::Value (String value) {
	this->_value = value;

	return this;
}

String Command::CStdRegistry::Value () {
	return this->_value;
}

bool Command::CStdRegistry::CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) {
	(void)device;
	(void)hardware;

	short trail = this->_recognize(command);
	if (trail == -1) return false;

	unsigned int i = trail;
	unsigned int l = command.length();

	bool setKey = false;

	String key = "";
	String value = "";

	while (i < l) {
		if (setKey) value += command[i];
		else {
			if (command[i] != ';') key += command[i];
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

bool Command::CStdRegistry::CommandSerialize () {
	this->_buffer = "";
	this->_buffer += this->_name;
	this->_buffer += ":";

	// TODO: serialize

	return true;
}