#include <Arduino.h>
#include <Wire.h>

#include "../Protonix.h"
#include "../ProtonixTimer.h"

#include "HBridgeADS1115.h"

using namespace Qybercom::Protonix;

unsigned int Hardware::HBridgeADS1115::CMDReadPin (unsigned short pin) {
	return (0x4000 + (pin << 12))
		| Hardware::HBridgeADS1115::FLAGS::OS_SINGLE
		| Hardware::HBridgeADS1115::FLAGS::PGA_4_096V
		| Hardware::HBridgeADS1115::FLAGS::MODE_SINGLE
		| Hardware::HBridgeADS1115::FLAGS::DR_128SPS
		| Hardware::HBridgeADS1115::FLAGS::COMP_OFF;
}

unsigned int Hardware::HBridgeADS1115::_read (byte reg) {
	if (!this->_init) return 0;

	Wire.beginTransmission(this->_address);
	Wire.write(reg);
	Wire.endTransmission();

	Wire.requestFrom(this->_address, (byte)2);

	return ((unsigned int)Wire.read() << 8) | Wire.read();
}

bool Hardware::HBridgeADS1115::_write (byte reg, unsigned int value) {
	if (!this->_init) return false;

	Wire.beginTransmission(this->_address);
	Wire.write(reg);
	Wire.write((byte)(value >> 8));
	Wire.write((byte)(value & 0xFF));
	Wire.endTransmission();

	return true;
}

Hardware::HBridgeADS1115::HBridgeADS1115 (byte address) {
	this->_address = address;
	this->_init = false;
	this->_readTimer = new ProtonixTimer(10, false);
	this->_readPin = 0;

	this->_values[0] = -1;
	this->_values[1] = -1;
	this->_values[2] = -1;
	this->_values[3] = -1;
}

Hardware::HBridgeADS1115* Hardware::HBridgeADS1115::Init (byte address) {
	return new Hardware::HBridgeADS1115(address);
}

ProtonixTimer* Hardware::HBridgeADS1115::ReadTimer () {
	return this->_readTimer;
}

byte Hardware::HBridgeADS1115::Address () {
	return this->_address;
}

Hardware::HBridgeADS1115* Hardware::HBridgeADS1115::Address (byte address) {
	this->_address = address;

	return this;
}

bool Hardware::HBridgeADS1115::HardwareI2C () {
	return true;
}

void Hardware::HBridgeADS1115::HardwareI2CPre (Protonix* device) {
	(void)device;
}

void Hardware::HBridgeADS1115::HardwareI2CPost (Protonix* device) {
	(void)device;

	this->_readTimer->Enabled(true);

	this->_init = true;
}

bool Hardware::HBridgeADS1115::BridgePinInitInput (unsigned int pin, int initial) {
	return this->BridgePinMode(pin, INPUT);
}

bool Hardware::HBridgeADS1115::BridgePinInitInputUp (unsigned int pin, int initial) {
	return false;
}

bool Hardware::HBridgeADS1115::BridgePinInitInputDown (unsigned int pin, int initial) {
	return false;
}

bool Hardware::HBridgeADS1115::BridgePinInitOutput (unsigned int pin) {
	return false;
}

bool Hardware::HBridgeADS1115::BridgePinMode (unsigned int pin, int mode) {
	if (!this->_init) return false;
	if (pin > 3) return false;

	if (mode != INPUT) return false;

	return true;
}

bool Hardware::HBridgeADS1115::BridgePinAvailable (unsigned int pin) {
	return this->_init && pin < 4;
}

bool Hardware::HBridgeADS1115::BridgeDigitalRead (unsigned int pin) {
	return false;
}

bool Hardware::HBridgeADS1115::BridgeDigitalWrite (unsigned int pin, bool value) {
	return false;
}

int Hardware::HBridgeADS1115::BridgeAnalogRead (unsigned int pin) {
	if (!this->_init) return 0;
	if (pin > 3) return 0;

	return this->_values[pin];
}

bool Hardware::HBridgeADS1115::BridgeAnalogWrite (unsigned int pin, int value) {
	return false;
}

void Hardware::HBridgeADS1115::HardwarePipe (Protonix* device, short core) {
	if (this->_readTimer->Pipe()) {
		this->_values[this->_readPin] = this->_read(0x00);

		this->_readPin = this->_readPin + 1;
		if (this->_readPin > 3) this->_readPin = 0;

		this->_write(0x01, Hardware::HBridgeADS1115::CMDReadPin(this->_readPin));
	}
}