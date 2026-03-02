#include <Arduino.h>
#include <Wire.h>

#include "../Protonix.h"

#include "HBridgePCF8574.h"

using namespace Qybercom::Protonix;

bool Hardware::HBridgePCF8574::_probe () {
	Wire.beginTransmission(this->_address);

	return Wire.endTransmission() == 0;
}

char Hardware::HBridgePCF8574::_read () {
	if (!this->_init) return 0x00;

	Wire.requestFrom(this->_address, (char)1);

	return Wire.available() ? Wire.read() : 0xFF;
}

bool Hardware::HBridgePCF8574::_write (char value) {
	if (!this->_init) return false;

	Wire.beginTransmission(this->_address);
	Wire.write(value);
	Wire.endTransmission();

	return true;
}

Hardware::HBridgePCF8574::HBridgePCF8574 (char address) {
	this->_address = address;
	this->_init = false;
	this->_dataOut = 0xFF; // all HIGH (input)
	this->_modeMask = 0x00; // all INPUT
}

char Hardware::HBridgePCF8574::Address () {
	return this->_address;
}

Hardware::HBridgePCF8574* Hardware::HBridgePCF8574::Address (char address) {
	this->_address = address;

	return this;
}

String Hardware::HBridgePCF8574::HardwareSummary () {
	return "PCF8574 expander";
}

void Hardware::HBridgePCF8574::HardwareInitPre (Protonix* device) {
	this->_capability("value", "pin0:bool", "Pin 0 value");
	this->_capability("value", "pin1:bool", "Pin 1 value");
	this->_capability("value", "pin2:bool", "Pin 2 value");
	this->_capability("value", "pin3:bool", "Pin 3 value");
	this->_capability("value", "pin4:bool", "Pin 4 value");
	this->_capability("value", "pin5:bool", "Pin 5 value");
	this->_capability("value", "pin6:bool", "Pin 6 value");
	this->_capability("value", "pin7:bool", "Pin 7 value");

	this->_capability("command", "pin", "Pin # value");
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

void Hardware::HBridgePCF8574::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "pin") {
		unsigned short pin = command.Argument(2);
		bool value = command.Argument(3);

		this->BridgeDigitalWrite(pin, value);
	}
}

bool Hardware::HBridgePCF8574::BridgePinInitInput (unsigned int pin, int initial) {
	return this->BridgePinMode(pin, INPUT);
}

bool Hardware::HBridgePCF8574::BridgePinInitInputUp (unsigned int pin, int initial) {
	return this->BridgePinMode(pin, INPUT_PULLUP);
}

bool Hardware::HBridgePCF8574::BridgePinInitInputDown (unsigned int pin, int initial) {
	#if defined(ESP8266) || !defined(INPUT_PULLDOWN)
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

	#if !defined(ESP8266) && defined(INPUT_PULLDOWN)
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

	char val = this->_read();

	bool out = !((bool)((val >> pin) & 0x01));
	this->_capability("pin" + String(pin) + ":bool", String(out));

	return out;
}

bool Hardware::HBridgePCF8574::BridgeDigitalWrite (unsigned int pin, bool value) {
	if (!this->_init) return false;

	if (pin > 7) return false;
	if (!(this->_modeMask & (1 << pin))) return false;

	if (value) this->_dataOut |= (1 << pin);
	else this->_dataOut &= ~(1 << pin);

	bool out = this->_write(this->_dataOut);

	this->_capability("pin" + String(pin) + ":bool", String(value));

	return out;
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