#include "Arduino.h"

#include "Common/List.hpp"

#include "IProtonixHardware.h"
#include "Protonix.h"
#include "ProtonixHardwareCapability.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

ProtonixHardwareCapability* IProtonixHardware::_capability (String kind, String id, String comment) {
	for (ProtonixHardwareCapability* capability : this->_capabilities)
		if (capability->ID() == id) return capability;

	ProtonixHardwareCapability* capability = new ProtonixHardwareCapability(kind, id, comment);

	this->_capabilities.Add(capability);

	return capability;
}

bool IProtonixHardware::_capability (String id, String value) {
	for (ProtonixHardwareCapability* capability : this->_capabilities) {
		if (capability->ID() != id) continue;

		capability->Value(value);

		return true;
	}

	return false;
}

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