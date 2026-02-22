#include <Arduino.h>

#include "../IProtonixHardware.hpp"

#include "HRelay.h"

using namespace Qybercom::Protonix;

Hardware::HRelay::HRelay (unsigned short pin, bool inverted) {
	this->_config["pin"] = pin;
	this->_config["inverted"] = inverted;
}

void Hardware::HRelay::Open () {
	this->_bridge->BridgeDigitalWrite(
		this->_config["pin"],
		this->_config["inverted"] ? false : true
	);

	this->_capability("open:bool", "1");
}

void Hardware::HRelay::Close () {
	this->_bridge->BridgeDigitalWrite(
		this->_config["pin"],
		this->_config["inverted"] ? true : false
	);

	this->_capability("open:bool", "0");
}

String Hardware::HRelay::HardwareSummary () {
	return "Relay";
}

void Hardware::HRelay::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_bridge->BridgePinMode(this->_config["pin"], OUTPUT);

	this->_capability("command", "open", "Activate relay");
	this->_capability("command", "close", "Deactivate relay");
	this->_capability("value", "open:bool", "State of the relay");

	this->Close();
}

void Hardware::HRelay::HardwareOnReset (Protonix* device) {
	(void)device;

	this->Close();
}

void Hardware::HRelay::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "open") this->Open();
	if (cmd == "close") this->Close();
}