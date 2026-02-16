#include <Arduino.h>
#include <Wire.h>

#include "../IProtonixHardware.hpp"

#include "HContextCA958.h"

using namespace Qybercom::Protonix;

Hardware::HContextCA958::HContextCA958 (char address) {
	this->_config["address"] = address;
}

String Hardware::HContextCA958::HardwareSummary () {
	return "Context adapter based on CA95X";
}

bool Hardware::HContextCA958::HardwareI2C () {
	return true;
}

void Hardware::HContextCA958::HardwareI2CPre (Protonix* device) {
	(void)device;
	if (!this->HardwareI2C()) return;
}

void Hardware::HContextCA958::HardwareI2CPost (Protonix* device) {
	(void)device;
	if (!this->HardwareI2C()) return;
}

void Hardware::HContextCA958::HardwareContextApply (IProtonixHardware* hardware) {
	String hardwareID = hardware->HardwareID();
	int channel = this->HardwareContextData(hardwareID);
	int address = this->_config["address"];

	Wire.beginTransmission(address);
	Wire.write(1 << channel);
	Wire.endTransmission();
}