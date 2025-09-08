#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

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

bool Hardware::HButton::HardwareSPI () {
	return this->_trigger->HardwareSPI();
}

void Hardware::HButton::HardwareInitPre (ProtonixDevice* device) {
	this->_trigger->HardwareInitPre(device);
}

void Hardware::HButton::HardwareInitPost (ProtonixDevice* device) {
	this->_trigger->HardwareInitPost(device);
}

void Hardware::HButton::HardwarePipe (ProtonixDevice* device, short core) {
	this->_trigger->HardwarePipe(device, core);
}