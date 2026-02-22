#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HGenericAnalog.h"

using namespace Qybercom::Protonix;

Hardware::HGenericAnalog::HGenericAnalog (unsigned short pin, bool init) {
	this->_value = -1;

	this->_config["pin"] = pin;
	this->_config["init"] = init;
}

int Hardware::HGenericAnalog::Value () {
	return this->_value;
}

String Hardware::HGenericAnalog::HardwareSummary () {
	return "Generic analog";
}

void Hardware::HGenericAnalog::HardwareInitPre (Protonix* device) {
	(void)device;

	if (this->_config["init"])
		this->_bridge->BridgePinInitInput(this->_config["pin"]);

	this->_capability("value", "value:int", "Raw value");
}

void Hardware::HGenericAnalog::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	this->_value = this->_bridge->BridgeAnalogRead(this->_config["pin"]);

	this->_capability("value:int", String(this->_value));
}

void Hardware::HGenericAnalog::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HGenericAnalog::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	(void)command;
}