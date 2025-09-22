#include <Arduino.h>

#include "../IProtonixHardware.h"

#include "HRelay.h"

using namespace Qybercom::Protonix;

Hardware::HRelay::HRelay (unsigned short pin, bool inverted) {
	this->_pin = pin;
	this->_inverted = inverted;
}

unsigned short Hardware::HRelay::Pin () {
	return this->_pin;
}

Hardware::HRelay* Hardware::HRelay::Pin (unsigned short pin) {
	this->_pin = pin;

	return this;
}

void Hardware::HRelay::Open () {
	digitalWrite(this->_pin, this->_inverted ? LOW : HIGH);
}

void Hardware::HRelay::Close () {
	digitalWrite(this->_pin, this->_inverted ? HIGH : LOW);
}

bool Hardware::HRelay::HardwareSPI () {
	return false;
}

void Hardware::HRelay::HardwareInitPre (Protonix* device) {
	(void)device;

	pinMode(this->_pin, OUTPUT);

	this->Close();
}

void Hardware::HRelay::HardwareInitPost (Protonix* device) {
	(void)device;
}

void Hardware::HRelay::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;
}

void Hardware::HRelay::HardwareOnCommand (Protonix* device, String command) {
	(void)device;

	if (command == "open") this->Open();
	if (command == "close") this->Close();
}