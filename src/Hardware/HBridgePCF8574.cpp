#include <Arduino.h>
#include <Wire.h>

#include "../Protonix.h"

#include "HBridgePCF8574.h"

using namespace Qybercom::Protonix;

bool Hardware::HBridgePCF8574::_probe () {
	Wire.beginTransmission(this->_address);

	return Wire.endTransmission() == 0;
}

byte Hardware::HBridgePCF8574::_read () {
	if (!this->_init) return 0x00;

	Wire.requestFrom(this->_address, (byte)1);

	return Wire.available() ? Wire.read() : 0xFF;
}

bool Hardware::HBridgePCF8574::_write (byte value) {
	if (!this->_init) return false;

	Wire.beginTransmission(this->_address);
	Wire.write(value);
	Wire.endTransmission();

	return true;
}

Hardware::HBridgePCF8574::HBridgePCF8574 (byte address) {
	this->_address = address;
	this->_init = false;
	this->_dataOut = 0xFF;   // all HIGH (input)
	this->_modeMask = 0x00;  // all INPUT
}

Hardware::HBridgePCF8574* Hardware::HBridgePCF8574::Init (byte address) {
	return new Hardware::HBridgePCF8574(address);
}

byte Hardware::HBridgePCF8574::Address () {
	return this->_address;
}

Hardware::HBridgePCF8574* Hardware::HBridgePCF8574::Address (byte address) {
	this->_address = address;

	return this;
}

bool Hardware::HBridgePCF8574::HardwareI2C () {
	return true;
}

void Hardware::HBridgePCF8574::HardwareI2CPre (Protonix* device) {
	(void)device;
}

void Hardware::HBridgePCF8574::HardwareI2CPost (Protonix* device) {
	(void)device;
	if (!this->_probe()) return;

	this->_write(0xFF);

	this->_init = true;
}

bool Hardware::HBridgePCF8574::BridgePinInitInput (unsigned int pin, int initial) {
	return this->BridgePinMode(pin, INPUT);
}

bool Hardware::HBridgePCF8574::BridgePinInitInputUp (unsigned int pin, int initial) {
	return this->BridgePinMode(pin, INPUT_PULLUP);
}

bool Hardware::HBridgePCF8574::BridgePinInitInputDown (unsigned int pin, int initial) {
	#if defined(ESP8266)
	return false;
	#else
	return this->BridgePinMode(pin, INPUT_PULLDOWN);
	#endif
}

bool Hardware::HBridgePCF8574::BridgePinInitOutput (unsigned int pin) {
	return this->BridgePinMode(pin, OUTPUT);
}

bool Hardware::HBridgePCF8574::BridgePinMode (unsigned int pin, int mode) {
	if (!this->_init) return false;
	if (pin > 7) return false;

	if (mode == OUTPUT) this->_modeMask |= (1 << pin);
	else this->_modeMask &= ~(1 << pin);

	if (mode == INPUT || mode == INPUT_PULLUP)
		this->_dataOut |= (1 << pin);

	#if !defined(ESP8266)
	if (mode == INPUT_PULLDOWN)
		this->_dataOut &= ~(1 << pin);
	#endif

	return this->_write(this->_dataOut);
}

bool Hardware::HBridgePCF8574::BridgePinAvailable (unsigned int pin) {
	return this->_init && pin < 8;
}

bool Hardware::HBridgePCF8574::BridgeDigitalRead (unsigned int pin) {
	if (pin > 7) return false;

	byte val = this->_read();

	return !((bool)((val >> pin) & 0x01));
}

bool Hardware::HBridgePCF8574::BridgeDigitalWrite (unsigned int pin, bool value) {
	if (!this->_init) return false;

	if (pin > 7) return false;
	if (!(this->_modeMask & (1 << pin))) return false;

	if (value) this->_dataOut |= (1 << pin);
	else this->_dataOut &= ~(1 << pin);

	return this->_write(this->_dataOut);
}

int Hardware::HBridgePCF8574::BridgeAnalogRead (unsigned int pin) {
	return -1;
}

bool Hardware::HBridgePCF8574::BridgeAnalogWrite (unsigned int pin, int value) {
	return false;
}

void Hardware::HBridgePCF8574::HardwarePipe (Protonix* device, short core) {
	// TODO: add interrupt
}