#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HTrigger.h"

using namespace Qybercom::Protonix;

Hardware::HTrigger::HTrigger (bool input, unsigned short pin, unsigned int checkInterval, unsigned short mode) {
	this->_pin = pin;
	this->_input = input;
	this->_inputMode = mode;
	this->_inputChanged = false;
	this->_inputValue = false;

	this->_debouncer.CheckInterval(checkInterval);
}

Hardware::HTrigger* Hardware::HTrigger::Input (unsigned short pin, unsigned short mode, unsigned int checkInterval) {
	return new Hardware::HTrigger(true, pin, checkInterval, mode);
}

Hardware::HTrigger* Hardware::HTrigger::Output (unsigned short pin) {
	return new Hardware::HTrigger(false, pin, 0, LOW);
}

unsigned short Hardware::HTrigger::Pin () {
	return this->_pin;
}

unsigned short Hardware::HTrigger::InputMode () {
	return this->_inputMode;
}

bool Hardware::HTrigger::InputChanged () {
	bool out = false;

	if (this->_inputChanged) {
		out = true;
		this->_inputChanged = false;
	}

	return out;
}

bool Hardware::HTrigger::InputValue () {
	return this->_inputValue;
}

bool Hardware::HTrigger::OutputValue (unsigned short value) {
	if (this->_input) return false;

	Serial.println("[hardware:trigger] OutputValue " + String(value));

	digitalWrite(this->_pin, value);

	return true;
}

bool Hardware::HTrigger::HardwareSPI () {
	return false;
}

void Hardware::HTrigger::HardwareInitPre (ProtonixDevice* device) {
	(void)device;

	if (this->_input) {
		digitalWrite(this->_pin, this->_inputMode);
		pinMode(this->_pin, INPUT_PULLUP);
	}
	else {
		pinMode(this->_pin, OUTPUT);
		digitalWrite(this->_pin, LOW);
	}
}

void Hardware::HTrigger::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HTrigger::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;
	if (!this->_input) return;

	unsigned short value = digitalRead(this->_pin);

	this->_debouncer.Use(value);

	if (this->_debouncer.Pipe()) {
		value = this->_debouncer.Empty() ? 0 : this->_debouncer.Actual();

		bool val = (value == HIGH) != (this->_inputMode == HIGH);

		if (this->_inputValue != val) {
			this->_inputValue = val;
			this->_inputChanged = true;
		}

		this->_debouncer.Reset();
	}
}