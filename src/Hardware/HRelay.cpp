#include <Arduino.h>

#include "../IProtonixHardware.h"

#include "HRelay.h"

using namespace Qybercom::Protonix;

Hardware::HRelay::HRelay (unsigned short pin, bool inverted) {
	this->_pin = pin;
	this->_inverted = inverted;
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

void Hardware::HRelay::HardwareInitPre (ProtonixDevice* device) {
	(void)device;
}

void Hardware::HRelay::HardwareInitPost (ProtonixDevice* device) {
	(void)device;

	pinMode(this->_pin, OUTPUT);

	this->Close();
}

void Hardware::HRelay::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;
}