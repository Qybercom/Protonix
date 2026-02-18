#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HBuiltInLED.h"

using namespace Qybercom::Protonix;

Hardware::HBuiltInLED::HBuiltInLED (unsigned short pin) {
	this->_config["pin"] = pin;
}

void Hardware::HBuiltInLED::Active (bool active) {
	this->_bridge->BridgeDigitalWrite(this->_config["pin"], active
		? QYBERCOM_PROTONIX_LED_ON
		: QYBERCOM_PROTONIX_LED_OFF
	);

	this->_capability("active:bool", String(active ? "1" : "0"));
}

String Hardware::HBuiltInLED::HardwareSummary () {
	return "Built-in LED";
}

void Hardware::HBuiltInLED::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_bridge->BridgePinMode(this->_config["pin"], OUTPUT);

	this->_capability("command", "on", "Activate LED");
	this->_capability("command", "off", "Deactivate LED");
	this->_capability("value", "active:bool", "State of the LED");

	this->Active(false);
}

void Hardware::HBuiltInLED::HardwareOnReset (Protonix* device) {
	this->Active(false);
}

void Hardware::HBuiltInLED::HardwareOnCommand (Protonix* device, const ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "on")
		this->Active(true);

	if (cmd == "off")
		this->Active(false);
}