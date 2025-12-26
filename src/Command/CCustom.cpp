#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "CCustom.h"

using namespace Qybercom::Protonix;

Command::CCustom::CCustom () {
	this->_init("custom");
}

Command::CCustom::CCustom (String cmd) {
	this->_init("custom");
	this->CMD(cmd);
}

void Command::CCustom::CMD (String cmd) {
	this->_cmd = cmd;
}

String Command::CCustom::CMD () {
	return this->_cmd;
}

bool Command::CCustom::CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) {
	(void)device;
	(void)hardware;

	short trail = this->_recognize(command);
	if (trail == -1) return false;

	this->_cmd = command.substring(trail, command.length());

	return true;
}

bool Command::CCustom::CommandSerialize () {
	this->_buffer = this->_name + ":" + this->_cmd;

	return true;
}