#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HSensorMicrophone.h"

using namespace Qybercom::Protonix;

void Hardware::HSensorMicrophone::_signal (Protonix* device) {
	if (this->_config["allowSignal"])
		device->Signal(this->_id, "active")->Data(this->_active);
}

Hardware::HSensorMicrophone::HSensorMicrophone (unsigned short pin) {
	this->_active = false;

	this->_config["pin"] = pin;
}

bool Hardware::HSensorMicrophone::Active () {
	return this->_active;
}

String Hardware::HSensorMicrophone::HardwareSummary () {
	return "Microphone sensor";
}

void Hardware::HSensorMicrophone::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_bridge->BridgePinMode(this->_config["pin"], INPUT);

	this->_capability("value", "active:bool", "State of the microphone");
}

void Hardware::HSensorMicrophone::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	// TODO: refactor for analogRead
	bool active = this->_bridge->BridgeDigitalRead(this->_config["pin"]);

	if (active != this->_active) {
		this->_active = active;

		this->_signal(device);
	}

	this->_capability("active:bool", String(active));
}

void Hardware::HSensorMicrophone::HardwareOnReset (Protonix* device) {
	this->_signal(device);
}

void Hardware::HSensorMicrophone::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	(void)command;
}