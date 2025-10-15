#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HWeightHX711.h"

using namespace Qybercom::Protonix;

Hardware::HWeightHX711::HWeightHX711 (unsigned short pinDT, unsigned short pinSCK) {
	this->_pinDT = pinDT;
	this->_pinSCK = pinSCK;
	this->_value = -1;
}

Hardware::HWeightHX711* Hardware::HWeightHX711::Init (unsigned short pinDT, unsigned short pinSCK) {
	return new Hardware::HWeightHX711(pinDT, pinSCK);
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

long Hardware::HWeightHX711::Value () {
	return this->_value;
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
}

void Hardware::HWeightHX711::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	//this->_value = this->_bridge->BridgeAnalogRead(this->_pin);
	//while (digitalRead(pinDT) == HIGH); // ждем готовности
	if (this->_bridge->BridgeDigitalRead(this->_pinDT) != HIGH) return;

	unsigned long value = 0;
	unsigned short i = 0;

	while (i < 24) {
		this->_bridge->BridgeDigitalWrite(this->_pinSCK, HIGH);
		value = value << 1;
		this->_bridge->BridgeDigitalWrite(this->_pinSCK, LOW);

		if (this->_bridge->BridgeDigitalRead(this->_pinDT)) {
			value++;
		}

		i++;
	}

	// Послепроцессинг: 25-й такт определяет коэффициент усиления
	this->_bridge->BridgeDigitalWrite(this->_pinSCK, HIGH);
	this->_bridge->BridgeDigitalWrite(this->_pinSCK, LOW);

	// Преобразуем из 24-битного знакового в long
	if (value & 0x800000) {
		value |= 0xFF000000; // расширяем знак
	}

	this->_value = value;

	this->_capability("value:int", String(this->_value));
}

void Hardware::HWeightHX711::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HWeightHX711::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}