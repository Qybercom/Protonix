#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HGenericAnalog.h"

using namespace Qybercom::Protonix;

Hardware::HGenericAnalog::HGenericAnalog (unsigned short pin, bool init) {
	this->_pin = pin;
	this->_init = init;
	this->_value = -1;
}

Hardware::HGenericAnalog* Hardware::HGenericAnalog::Init (unsigned short pin, bool init) {
	return new Hardware::HGenericAnalog(pin, init);
}

bool Hardware::HGenericAnalog::Init () {
	return this->_init;
}

int Hardware::HGenericAnalog::Value () {
	return this->_value;
}

unsigned short Hardware::HGenericAnalog::Pin () {
	return this->_pin;
}

Hardware::HGenericAnalog* Hardware::HGenericAnalog::Pin (unsigned short pin) {
	this->_pin = pin;

	return this;
}

String Hardware::HGenericAnalog::HardwareSummary () {
	return "Generic analog";
}

void Hardware::HGenericAnalog::HardwareInitPre (Protonix* device) {
	(void)device;

	if (this->_init)
		this->_bridge->BridgePinInitInput(this->_pin);

	this->_capability("value", "value:int", "Raw value");
}

void Hardware::HGenericAnalog::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	this->_value = this->_bridge->BridgeAnalogRead(this->_pin);

	this->_capability("value:int", String(this->_value));
}

void Hardware::HGenericAnalog::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HGenericAnalog::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}