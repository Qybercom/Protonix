#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HWeightHX711.h"

using namespace Qybercom::Protonix;

Hardware::HWeightHX711::HWeightHX711 (unsigned short pinDT, unsigned short pinSCK) {
	this->_value = 0;
	this->_weight = 0;
	this->_readingsCounter = 0;
	this->_readingsValue = 0;

	this->_config["pinDT"] = pinDT;
	this->_config["pinSCK"] = pinSCK;
	this->_config["scale"] = 1.0;
	this->_config["reading"] = 1;
	this->_config["offset"] = 0;
}

long Hardware::HWeightHX711::Value () {
	return this->_value;
}

long Hardware::HWeightHX711::Weight () {
	return this->_weight;
}

String Hardware::HWeightHX711::HardwareSummary () {
	return "Weight sensor through HX711 DAC";
}

void Hardware::HWeightHX711::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_bridge->BridgePinInitOutput(this->_config["pinSCK"]);
	this->_bridge->BridgeDigitalWrite(this->_config["pinSCK"], false);

	this->_bridge->BridgePinInitInput(this->_config["pinDT"]);

	this->_capability("value", "value:int", "Raw value");
	this->_capability("value", "weight:int", "Interpolated weight");
}

void Hardware::HWeightHX711::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	if (this->_bridge->BridgeDigitalRead(this->_config["pinDT"])) return;

	unsigned long value = 0;

	noInterrupts();
	for (uint8_t i = 0; i < 24; i++) {
		this->_bridge->BridgeDigitalWrite(this->_config["pinSCK"], true);
		delayMicroseconds(1);

		value = (value << 1) | this->_bridge->BridgeDigitalRead(this->_config["pinDT"]);

		this->_bridge->BridgeDigitalWrite(this->_config["pinSCK"], false);
		delayMicroseconds(1);
	}

	// select channel/gain - 1 iteration for gain = 128
	this->_bridge->BridgeDigitalWrite(this->_config["pinSCK"], true);
	delayMicroseconds(1);

	this->_bridge->BridgeDigitalWrite(this->_config["pinSCK"], false);
	delayMicroseconds(1);
	interrupts();

	// extending to 32-bit
	if (value & 0x800000UL) value |= 0xFF000000UL;

	this->_value = value;
	this->_capability("value:int", String(this->_value));

	int readings = this->_config["readings"];
	int offset = this->_config["offset"];
	int scale = this->_config["scale"];

	if (this->_readingsCounter < readings) {
		this->_readingsCounter = this->_readingsCounter + 1;
		this->_readingsValue = this->_readingsValue + value;
	}
	else {
		this->_readingsCounter = 0;

		long average = this->_readingsValue / readings;
		this->_readingsValue = 0;

		this->_weight = (average - offset) / (scale == 0.0 ? 1.0 : scale) * 0.035274;

		device->Signal(this->_id, "weight")->Data(this->_weight);
	}

	this->_capability("weight:int", String(this->_weight));
}

void Hardware::HWeightHX711::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HWeightHX711::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	(void)command;
}