#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "CStdAction.h"

using namespace Qybercom::Protonix;

Command::CStdAction::CStdAction () {
	this->_init("std:action");
}

Command::CStdAction* Command::CStdAction::CMD (String cmd) {
	this->_cmd = cmd;

	return this;
}

String Command::CStdAction::CMD () {
	return this->_cmd;
}

Command::CStdAction* Command::CStdAction::Name (String name) {
	this->_name = name;

	return this;
}

String Command::CStdAction::Name () {
	return this->_name;
}

bool Command::CStdAction::CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) {
	(void)device;
	(void)hardware;

	short trail = this->_recognize(command);
	if (trail == -1) return false;

	unsigned int i = trail;
	unsigned int l = command.length();

	bool setCMD = false;

	String cmd = "";
	String name = "";

	while (i < l) {
		if (setCMD) name += command[i];
		else {
			if (command[i] != ';') cmd += command[i];
			else {
				setCMD = true;

				this->CMD(cmd);
			}
		}

		i++;
	}

	this->CMD(cmd);
	this->Name(name);

	return true;
}

bool Command::CStdAction::CommandSerialize () {
	this->_buffer = "";
	this->_buffer += this->_name;
	this->_buffer += ":";

	// TODO: serialize

	return true;
}