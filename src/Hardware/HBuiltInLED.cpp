#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HBuiltInLED.h"

using namespace Qybercom::Protonix;

Hardware::HBuiltInLED::HBuiltInLED (unsigned short pin) {
	this->_pin = pin;
}

void Hardware::HBuiltInLED::Active (bool active) {
	digitalWrite(this->_pin, active
		? QYBERCOM_PROTONIX_LED_ON
		: QYBERCOM_PROTONIX_LED_OFF
	);
}

bool Hardware::HBuiltInLED::HardwareSPI () {
	return false;
}

void Hardware::HBuiltInLED::HardwareInitPre (Protonix* device) {
	(void)device;

	pinMode(this->_pin, OUTPUT);
}

void Hardware::HBuiltInLED::HardwareInitPost (Protonix* device) {
	(void)device;
}

void Hardware::HBuiltInLED::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;
}

void Hardware::HBuiltInLED::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}