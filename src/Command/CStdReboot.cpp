#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "CStdReboot.h"

using namespace Qybercom::Protonix;

Command::CStdReboot::CStdReboot () {
	this->_init("std:reboot");
}

bool Command::CStdReboot::CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) {
	(void)device;
	(void)hardware;

	return this->_name == command;
}

bool Command::CStdReboot::CommandSerialize () {
	this->_buffer = this->_name;

	return true;
}