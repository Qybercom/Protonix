#include "Arduino.h"

#include "IProtonixHardware.h"
#include "Protonix.h"

using namespace Qybercom::Protonix;

bool IProtonixHardware::_log (String message, bool ret) {
	Serial.println("[hardware:" + this->_id + "] " + message);

	return ret;
}

String IProtonixHardware::HardwareID () {
	return this->_id;
}

IProtonixHardware* IProtonixHardware::HardwareID (String id) {
	this->_id = id;

	return this;
}

bool IProtonixHardware::HardwareAllowSignal () {
	return this->_allowSignal;
}

IProtonixHardware* IProtonixHardware::HardwareAllowSignal (bool allow) {
	this->_allowSignal = allow;

	return this;
}

short IProtonixHardware::HardwareDedicatedCore () {
	return this->_dedicatedCore;
}