#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HWeightHX711.h"

using namespace Qybercom::Protonix;

Hardware::HWeightHX711::HWeightHX711 (unsigned short pinDT, unsigned short pinSCK, float scale, unsigned short readings) {
	this->_pinDT = pinDT;
	this->_pinSCK = pinSCK;
	this->_value = -1;
	this->_weight = 0;
	this->_scale = scale;
	this->_readings = readings;
	this->_readingsCounter = 0;
	this->_readingsValue = 0;
	this->_zero = 0;
	this->_init = false;
}

Hardware::HWeightHX711* Hardware::HWeightHX711::Init (unsigned short pinDT, unsigned short pinSCK, float scale, unsigned short readings) {
	return new Hardware::HWeightHX711(pinDT, pinSCK, scale, readings);
}

unsigned short Hardware::HWeightHX711::PinDT () {
	return this->_pinDT;
}

Hardware::HWeightHX711* Hardware::HWeightHX711::PinDT (unsigned short pin) {
	this->_pinDT = pin;

	return this;
}

unsigned short Hardware::HWeightHX711::PinSCK () {
	return this->_pinSCK;
}

Hardware::HWeightHX711* Hardware::HWeightHX711::PinSCK (unsigned short pin) {
	this->_pinSCK = pin;

	return this;
}

float Hardware::HWeightHX711::Scale () {
	return this->_scale;
}

Hardware::HWeightHX711* Hardware::HWeightHX711::Scale (float scale) {
	this->_scale = scale;

	return this;
}

long Hardware::HWeightHX711::Value () {
	return this->_value;
}

long Hardware::HWeightHX711::Weight () {
	return this->_weight;
}

long Hardware::HWeightHX711::Zero () {
	return this->_zero;
}

String Hardware::HWeightHX711::HardwareSummary () {
	return "Weight sensor through HX711 DAC";
}

void Hardware::HWeightHX711::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_bridge->BridgePinInitOutput(this->_pinSCK);
	this->_bridge->BridgeDigitalWrite(this->_pinSCK, LOW);

	this->_bridge->BridgePinInitInput(this->_pinDT);

	this->_capability("value", "value:int", "Raw value");
	this->_capability("value", "weight:int", "Interpolated weight");
}

void Hardware::HWeightHX711::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	if (this->_bridge->BridgeDigitalRead(this->_pinDT) != HIGH) return;

	unsigned long value = 0;
	unsigned short i = 0;

	while (i < 24) {
		this->_bridge->BridgeDigitalWrite(this->_pinSCK, HIGH);
		delayMicroseconds(1);

		value = value << 1;

		this->_bridge->BridgeDigitalWrite(this->_pinSCK, LOW);
		delayMicroseconds(1);

		if (this->_bridge->BridgeDigitalRead(this->_pinDT)) {
			value++;
		}

		i++;
	}

	this->_bridge->BridgeDigitalWrite(this->_pinSCK, HIGH);
	delayMicroseconds(1);

	this->_bridge->BridgeDigitalWrite(this->_pinSCK, LOW);
	delayMicroseconds(1);

	if (value & 0x800000)
		value |= 0xFF000000;

	this->_value = value;
	this->_capability("value:int", String(this->_value));

	if (this->_readingsCounter < this->_readings) {
		this->_readingsCounter = this->_readingsCounter + 1;
		this->_readingsValue = this->_readingsValue + value;
	}
	else {
		this->_readingsCounter = 0;

		long average = this->_readingsValue / this->_readings;

		if (this->_init) {
			this->_weight = (average - this->_zero) / (this->_scale == 0.0 ? 1.0 : this->_scale);
			this->_weight *= 0.035274;

			device->Signal(this->_id, "weight")->Value(this->_weight);
		}
		else {
			//this->_zero = average;
			this->_init = true;

			device->Signal(this->_id, "calibrated")->Value(this->_zero);
		}
	}

	this->_capability("weight:int", String(this->_weight));
}

void Hardware::HWeightHX711::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HWeightHX711::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}