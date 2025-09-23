#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

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

bool Command::CStdHardware::CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) {
	(void)device;
	(void)hardware;

	short trail = this->_recognize(command);
	if (trail == -1) return false;

	unsigned int i = trail;
	unsigned int l = command.length();

	bool setID = false;

	String id = "";
	String cmd = "";

	while (i < l) {
		if (setID) cmd += command[i];
		else {
			if (command[i] != ':') id += command[i];
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
	this->_buffer = "";
	this->_buffer += this->_name;
	this->_buffer += ":";
	this->_buffer += this->_id;
	this->_buffer += ":";
	this->_buffer += this->_cmd;

	return true;
}