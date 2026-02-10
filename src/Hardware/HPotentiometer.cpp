#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HPotentiometer.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

Hardware::HPotentiometer::HPotentiometer (unsigned short pin, int max, int min) {
	this->_value = -1;

	this->_config["pin"] = pin;
	this->_config["max"] = max;
	this->_config["min"] = min;
}

int Hardware::HPotentiometer::Value () {
	return this->_value;
}

bool Hardware::HPotentiometer::ValueMatch (int min, int max, bool minEQ, bool maxEQ) {
	return matchRange(this->_value, min, max, minEQ, maxEQ);
}

int Hardware::HPotentiometer::ValueMapped (int min, int max) {
	return map(this->_value, this->_config["min"], this->_config["max"], min, max);
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

	this->_value = this->_bridge->BridgeAnalogRead(this->_config["pin"]);

	this->_capability("value:int", String(this->_value));
}

void Hardware::HPotentiometer::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HPotentiometer::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}