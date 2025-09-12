#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

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

bool Hardware::HTouchButton::HardwareSPI () {
	return false;
}

void Hardware::HTouchButton::HardwareInitPre (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HTouchButton::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HTouchButton::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;

	int value = digitalRead(this->_pin);

	this->_active = value == HIGH;
}

void Hardware::HTouchButton::HardwareCommand (ProtonixDevice* device, String command) {
	(void)device;

	
}