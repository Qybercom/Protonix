#include <Arduino.h>

#include "PCF8574.h"

#include "HBridgePCF8574.h"

using namespace Qybercom::Protonix;

Hardware::HBridgePCF8574::HBridgePCF8574 (int address) {
	this->_address = address;
	this->_driver = nullptr;
}

Hardware::HBridgePCF8574* Hardware::HBridgePCF8574::Init (int address) {
	return new Hardware::HBridgePCF8574(address);
}

int Hardware::HBridgePCF8574::Address () {
	return this->_address;
}

Hardware::HBridgePCF8574* Hardware::HBridgePCF8574::Address (int address) {
	this->_address = address;

	return this;
}

PCF8574* Hardware::HBridgePCF8574::Driver () {
	return this->_driver;
}

bool Hardware::HBridgePCF8574::HardwareI2C () {
	return true;
}

void Hardware::HBridgePCF8574::HardwareI2CPre (Protonix* device) {
	(void)device;

	this->_driver = new PCF8574(this->_address);
}

void Hardware::HBridgePCF8574::HardwareI2CPost (Protonix* device) {
	(void)device;

	this->_driver->begin();
}

bool Hardware::HBridgePCF8574::BridgePinMode (unsigned int pin, int mode) {
	this->_driver->pinMode(pin, mode);

	return true;
}

bool Hardware::HBridgePCF8574::BridgeDigitalRead (unsigned int pin) {
	return (bool)this->_driver->digitalRead(pin);
}

bool Hardware::HBridgePCF8574::BridgeDigitalWrite (unsigned int pin, bool value) {
	this->_driver->digitalWrite(pin, value);

	return true;
}

int Hardware::HBridgePCF8574::BridgeAnalogRead (unsigned int pin) {
	return -1;
}

bool Hardware::HBridgePCF8574::BridgeAnalogWrite (unsigned int pin, int value) {
	return false;
}