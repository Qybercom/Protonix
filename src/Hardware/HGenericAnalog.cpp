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

bool Hardware::HGenericAnalog::Init () {
	return this->_init;
}

int Hardware::HGenericAnalog::Value () {
	return this->_value;
}

bool Hardware::HGenericAnalog::HardwareSPI () {
	return false;
}

void Hardware::HGenericAnalog::HardwareInitPre (Protonix* device) {
	(void)device;

	if (this->_init)
		pinMode(this->_pin, INPUT);
}

void Hardware::HGenericAnalog::HardwareInitPost (Protonix* device) {
	(void)device;
}

void Hardware::HGenericAnalog::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	this->_value = analogRead(this->_pin);
}

void Hardware::HGenericAnalog::HardwareCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}