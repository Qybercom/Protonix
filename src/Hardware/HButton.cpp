#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HTrigger.h"

#include "HButton.h"

using namespace Qybercom::Protonix;

Hardware::HButton::HButton (unsigned short pin, unsigned int checkInterval) {
	this->_trigger = Hardware::HTrigger::Input(pin, HIGH, checkInterval);
}

Hardware::HTrigger* Hardware::HButton::Trigger () {
	return this->_trigger;
}

bool Hardware::HButton::Changed () {
	return this->_trigger->InputChanged();
}

bool Hardware::HButton::Active () {
	return this->_trigger->InputValue();
}

bool Hardware::HButton::Pressed (bool changed) {
	return (changed ? this->Changed() : true) && this->_trigger->InputValue();
}

bool Hardware::HButton::Released (bool changed) {
	return (changed ? this->Changed() : true) && !this->_trigger->InputValue();
}

void Hardware::HButton::HardwareInitPre (Protonix* device) {
	this->_trigger->HardwareInitPre(device);
}

void Hardware::HButton::HardwarePipe (Protonix* device, short core) {
	this->_trigger->HardwarePipe(device, core);

	if (this->_allowSignal && this->Changed()) {
		bool value = this->_trigger->InputValue();

		device->Signal(this->_id, "changed")->ValueBool(value);
		device->Signal(this->_id, String(value ? "pressed" : "released"));
	}
}

void Hardware::HButton::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;

	// TODO: send command to trigger
}