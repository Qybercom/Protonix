#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HGenericAnalog.h"

using namespace Qybercom::Protonix;

Hardware::HGenericAnalog::HGenericAnalog (unsigned short pin, bool init) {
	this->_pin = pin;
	this->_init = init;
	this->_value = -1;
}

unsigned short Hardware::HGenericAnalog::Pin () {
	return this->_pin;
}

bool Hardware::HGenericAnalog::Init () {
	return this->_init;
}

int Hardware::HGenericAnalog::Value () {
	return this->_value;
}

String Hardware::HGenericAnalog::HardwareSummary () {
	return "Generic analog";
}

void Hardware::HGenericAnalog::HardwareInitPre (Protonix* device) {
	(void)device;

	if (this->_init)
		pinMode(this->_pin, INPUT);

	this->_capability("value", "value:int", "Raw value");
}

void Hardware::HGenericAnalog::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	this->_value = analogRead(this->_pin);

	this->_capability("value:int", String(this->_value));
}

void Hardware::HGenericAnalog::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}