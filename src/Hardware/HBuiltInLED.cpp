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

	this->_capability("open:bool", String(active ? "1" : "0"));
}

String Hardware::HBuiltInLED::HardwareSummary () {
	return "Built-in LED";
}

void Hardware::HBuiltInLED::HardwareInitPre (Protonix* device) {
	(void)device;

	pinMode(this->_pin, OUTPUT);

	this->Active(false);

	this->_capability("command", "open", "Activate LED");
	this->_capability("command", "close", "Deactivate LED");
	this->_capability("value", "open:bool", "State of the LED");
}

void Hardware::HBuiltInLED::HardwareOnCommand (Protonix* device, String command) {
	(void)device;

	if (command == "on")
		this->Active(true);

	if (command == "off")
		this->Active(false);
}