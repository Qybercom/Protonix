#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HSensorHall.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

Hardware::HSensorHall::HSensorHall (unsigned short pin, int max, int min) {
	this->_value = -1;
	this->_config["pin"] = pin;
	this->_config["max"] = max;
	this->_config["min"] = min;
}

int Hardware::HSensorHall::Value () {
	return this->_value;
}

bool Hardware::HSensorHall::ValueMatch (int min, int max, bool minEQ, bool maxEQ) {
	return matchRange(this->_value, min, max, minEQ, maxEQ);
}

int Hardware::HSensorHall::ValueMapped (int min, int max) {
	return map(this->_value, this->_config["min"], this->_config["max"], min, max);
}

String Hardware::HSensorHall::HardwareSummary () {
	return "Hall-effect sensor";
}

void Hardware::HSensorHall::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_capability("value", "value:int", "Raw value");
}

void Hardware::HSensorHall::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	this->_value = this->_bridge->BridgeAnalogRead(this->_config["pin"]);

	this->_capability("value:int", String(this->_value));
}

void Hardware::HSensorHall::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HSensorHall::HardwareOnCommand (Protonix* device, const ProtonixCommand &command) {
	(void)device;
	(void)command;
}