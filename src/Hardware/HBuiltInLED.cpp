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

void Hardware::HBuiltInLED::HardwareInitPre (Protonix* device) {
	(void)device;

	pinMode(this->_pin, OUTPUT);
}

void Hardware::HBuiltInLED::HardwareOnCommand (Protonix* device, String command) {
	(void)device;

	if (command == "on")
		this->Active(true);

	if (command == "off")
		this->Active(false);
}