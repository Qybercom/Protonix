#include <Arduino.h>
#include <Wire.h>

#include "../IProtonixHardware.hpp"

#include "HContextCA954.h"

using namespace Qybercom::Protonix;

Hardware::HContextCA954::HContextCA954 (char address) {
	this->_config["address"] = address;
}

String Hardware::HContextCA954::HardwareSummary () {
	return "I2C context adapter based on (T/P)CA954XX";
}

bool Hardware::HContextCA954::HardwareI2C () {
	return true;
}

void Hardware::HContextCA954::HardwareI2CPre (Protonix* device) {
	(void)device;
	if (!this->HardwareI2C()) return;
}

void Hardware::HContextCA954::HardwareI2CPost (Protonix* device) {
	(void)device;
	if (!this->HardwareI2C()) return;
}

void Hardware::HContextCA954::HardwareContextApply (IProtonixHardware* hardware) {
	String hardwareID = hardware->HardwareID();
	int channel = this->HardwareContextData(hardwareID);
	int address = this->_config["address"];

	Wire.beginTransmission(address);
	Wire.write(1 << channel);
	Wire.endTransmission();
}