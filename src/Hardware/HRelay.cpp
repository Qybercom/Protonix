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
	this->_capability("open:bool", "1");
}

void Hardware::HRelay::Close () {
	digitalWrite(this->_pin, this->_inverted ? HIGH : LOW);
	this->_capability("open:bool", "0");
}

String Hardware::HRelay::HardwareSummary () {
	return "Relay";
}

void Hardware::HRelay::HardwareInitPre (Protonix* device) {
	(void)device;

	pinMode(this->_pin, OUTPUT);

	this->Close();

	this->_capability("command", "open", "Activate relay");
	this->_capability("command", "close", "Deactivate relay");
	this->_capability("value", "open:bool", "State of the relay");
}

void Hardware::HRelay::HardwareOnCommand (Protonix* device, String command) {
	(void)device;

	if (command == "open") this->Open();
	if (command == "close") this->Close();
}