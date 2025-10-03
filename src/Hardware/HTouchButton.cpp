#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HTouchButton.h"

using namespace Qybercom::Protonix;

Hardware::HTouchButton::HTouchButton (unsigned short pin) {
	this->_active = false;
	this->_pin = pin;
}

unsigned short Hardware::HTouchButton::Pin () {
	return this->_pin;
}

Hardware::HTouchButton* Hardware::HTouchButton::Pin (unsigned short pin) {
	// TODO: handle reading through PCF8574

	pinMode(this->_pin, INPUT_PULLUP);
	this->_pin = pin;

	return this;
}

bool Hardware::HTouchButton::Active () {
	return this->_active;
}

String Hardware::HTouchButton::HardwareSummary () {
	return "Touch button";
}

void Hardware::HTouchButton::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_capability("value", "active:bool", "State of the button");
}

void Hardware::HTouchButton::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	int value = digitalRead(this->_pin);
	bool active = value == HIGH;
	bool changed = this->_active != active;

	this->_active = active;

	if (this->_allowSignal && changed) {
		device->Signal(this->_id, "changed")->Value(active);
		device->Signal(this->_id, String(active ? "touch" : "release"));
	}

	this->_capability("active:bool", String(this->_active ? "1" : "0"));
}

void Hardware::HTouchButton::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}