#include <Arduino.h>

#include "../IProtonixHardware.hpp"

#include "HBuzzer.h"

using namespace Qybercom::Protonix;

Hardware::HBuzzer::HBuzzer (unsigned short pin) {
	this->_started = 0;
	this->_duration = 0;

	this->_config["pin"] = pin;
}

void Hardware::HBuzzer::Play (short value, unsigned long duration) {
	this->_started = millis();
	this->_duration = duration;

	noTone((int)this->_config["pin"]);
	tone((int)this->_config["pin"], value);

	this->_capability("active:bool", "1");
	this->_capability("tone:int", String(value));
}

void Hardware::HBuzzer::Stop () {
	this->_started = 0;
	this->_duration = 0;

	noTone((int)this->_config["pin"]);

	this->_capability("active:bool", "0");
	this->_capability("tone:int", "0");
}

String Hardware::HBuzzer::HardwareSummary () {
	return "Buzzer";
}

void Hardware::HBuzzer::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_bridge->BridgePinMode(this->_config["pin"], OUTPUT);

	this->Stop();

	this->_capability("value", "active:bool", "State of the buzzer");
	this->_capability("value", "tone:int", "Tone value");
	this->_capability("command", "stop", "Stop");
}

void Hardware::HBuzzer::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	unsigned long current = millis();
	unsigned long edge = this->_started + this->_duration;

	if (current >= edge)
		this->Stop();
}

void Hardware::HBuzzer::HardwareOnReset (Protonix* device) {
	(void)device;

	this->Stop();
}

void Hardware::HBuzzer::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "stop")
		this->Stop();
}