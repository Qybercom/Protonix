#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HTrigger.h"

using namespace Qybercom::Protonix;

bool Hardware::HTrigger::_inputChangedHandler () {
	if (!this->_allowSignal) return false;

	bool out = false;

	if (this->_inputChangedSignal) {
		out = true;
		this->_inputChangedSignal = false;
	}

	return out;
}

bool Hardware::HTrigger::_pipe () {
	if (!this->_input) return false;

	unsigned short value = digitalRead(this->_pin);

	this->_debouncer.Use(value);

	if (this->_debouncer.Pipe()) {
		value = this->_debouncer.Empty() ? 0 : this->_debouncer.Actual();

		bool val = (value == HIGH) != (this->_inputMode == HIGH);

		if (this->_inputValue != val) {
			this->_inputValue = val;
			this->_inputChanged = true;
			this->_inputChangedSignal = true;

			this->_capability("active:bool", String(val ? "1" : "0"));
		}

		this->_debouncer.Reset();
	}

	return true;
}

Hardware::HTrigger::HTrigger (bool input, unsigned short pin, unsigned int checkInterval, unsigned short mode, bool interrupt) {
	this->_pin = pin;
	this->_interrupt = interrupt;
	this->_input = input;
	this->_inputMode = mode;
	this->_inputChanged = false;
	this->_inputChangedSignal = false;
	this->_inputValue = false;

	this->_debouncer.CheckInterval(checkInterval);

	this->_signalInputChanged = "inputChanged";
}

Hardware::HTrigger* Hardware::HTrigger::Input (unsigned short pin, unsigned short mode, unsigned int checkInterval, bool interrupt) {
	return new Hardware::HTrigger(true, pin, checkInterval, mode, interrupt);
}

Hardware::HTrigger* Hardware::HTrigger::Output (unsigned short pin) {
	return new Hardware::HTrigger(false, pin, 0, LOW, false);
}

unsigned short Hardware::HTrigger::Pin () {
	return this->_pin;
}

unsigned short Hardware::HTrigger::InputMode () {
	return this->_inputMode;
}

Qybercom::Debouncer<unsigned short> &Hardware::HTrigger::Debouncer () {
	return this->_debouncer;
}

bool Hardware::HTrigger::Interrupt () {
	return this->_interrupt;
}

Hardware::HTrigger* Hardware::HTrigger::Interrupt (bool interrupt) {
	this->_interrupt = interrupt;

	return this;
}

String Hardware::HTrigger::SignalInputChanged () {
	return this->_signalInputChanged;
}

Hardware::HTrigger* Hardware::HTrigger::SignalInputChanged (String signal) {
	this->_signalInputChanged = signal;

	return this;
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

	this->_log("OutputValue " + String(value));

	digitalWrite(this->_pin, value);

	this->_capability("active:bool", String(value));

	return true;
}

String Hardware::HTrigger::HardwareSummary () {
	return "Trigger " + String(this->_input ? "input" : "output");
}

void Hardware::HTrigger::HardwareInitPre (Protonix* device) {
	(void)device;

	if (this->_input) {
		digitalWrite(this->_pin, this->_inputMode);
		pinMode(this->_pin, INPUT_PULLUP);

		if (this->_interrupt)
			device->InterruptAttach(this->_pin, CHANGE);

		this->_capability("value", "active:bool", "State of the trigger");
	}
	else {
		pinMode(this->_pin, OUTPUT);
		digitalWrite(this->_pin, LOW);

		this->_capability("command", "output:<bool>", "Set trigger' output value");
	}
}

void Hardware::HTrigger::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	if (!this->_interrupt)
		this->_pipe();

	if (this->_inputChangedHandler())
		device->Signal(this->_id, this->_signalInputChanged)->Value(this->_inputValue);
}

void Hardware::HTrigger::HardwarePipeInterrupt (Protonix* device) {
	(void)device;

	if (!this->_interrupt) return;

	this->_pipe();
}

void Hardware::HTrigger::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}