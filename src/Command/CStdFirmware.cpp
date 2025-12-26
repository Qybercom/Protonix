#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "CStdFirmware.h"

using namespace Qybercom::Protonix;

Command::CStdFirmware::CStdFirmware () {
	this->_init("std:firmware");
}

Command::CStdFirmware* Command::CStdFirmware::Action (String action) {
	this->_action = action;

	return this;
}

String Command::CStdFirmware::Action () {
	return this->_action;
}

bool Command::CStdFirmware::ActionUpdate () {
	return this->_action == "update";
}

Command::CStdFirmware* Command::CStdFirmware::Version (String version) {
	this->_version = version;

	return this;
}

String Command::CStdFirmware::Version () {
	return this->_version;
}

bool Command::CStdFirmware::VersionLatest () {
	return this->_version == "";
}

bool Command::CStdFirmware::CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) {
	(void)device;
	(void)hardware;

	short trail = this->_recognize(command);
	if (trail == -1) return false;

	unsigned int i = trail;
	unsigned int l = command.length();

	bool setAction = false;

	String action = "";
	String version = "";

	while (i < l) {
		if (setAction) version += command[i];
		else {
			if (command[i] != ';') action += command[i];
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

bool Command::CStdFirmware::CommandSerialize () {
	this->_buffer = "";
	this->_buffer += this->_name;
	this->_buffer += ":";

	// TODO: serialize

	return true;
}