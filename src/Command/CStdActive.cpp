#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "CStdActive.h"

using namespace Qybercom::Protonix;

Command::CStdActive::CStdActive () {
	this->_init("std:active");
}

Command::CStdActive::CStdActive (bool active) {
	this->_init("std:active");
	this->Active(active);
}

void Command::CStdActive::Active (bool active) {
	this->_active = active;
}

bool Command::CStdActive::Active () {
	return this->_active;
}

bool Command::CStdActive::CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) {
	(void)device;
	(void)hardware;

	short trail = this->_recognize(command);
	if (trail == -1) return false;

	String active = command.substring(trail, command.length());
	if (active != "1" && active != "0") return false;

	this->_active = active == "1";

	return true;
}

bool Command::CStdActive::CommandSerialize () {
	this->_buffer = this->_name + ":" + String(this->_active);

	return true;
}