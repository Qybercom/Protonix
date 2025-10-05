#include <Arduino.h>

#include "Common/Utils.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HPotentiometer.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

Hardware::HPotentiometer::HPotentiometer (unsigned short pin, int max, int min) {
	this->_value = -1;
	this->_pin = pin;
	this->_max = max;
	this->_min = min;
}

unsigned short Hardware::HPotentiometer::Pin () {
	return this->_pin;
}

Hardware::HPotentiometer* Hardware::HPotentiometer::Pin (unsigned short pin) {
	this->_pin = pin;

	return this;
}

int Hardware::HPotentiometer::Max () {
	return this->_max;
}

Hardware::HPotentiometer* Hardware::HPotentiometer::Max (int max) {
	this->_max = max;

	return this;
}

int Hardware::HPotentiometer::Min () {
	return this->_min;
}

Hardware::HPotentiometer* Hardware::HPotentiometer::Min (int min) {
	this->_min = min;

	return this;
}

int Hardware::HPotentiometer::Value () {
	return this->_value;
}

bool Hardware::HPotentiometer::ValueMatch (int min, int max, bool minEQ, bool maxEQ) {
	return matchRange(this->_value, min, max, minEQ, maxEQ);
}

int Hardware::HPotentiometer::ValueMapped (int min, int max) {
	return map(this->_value, this->_min, this->_max, min, max);
}

String Hardware::HPotentiometer::HardwareSummary () {
	return "Potentiometer";
}

void Hardware::HPotentiometer::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_capability("value", "value:int", "Raw value");
}

void Hardware::HPotentiometer::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	this->_value = analogRead(this->_pin);

	this->_capability("value:int", String(this->_value));
}

void Hardware::HPotentiometer::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HPotentiometer::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}