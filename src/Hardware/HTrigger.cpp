#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
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
	if (!this->_init) return false;

	String mode = "";//this->_config.Get<String>("mode", "");
	if (mode != "input") return false;

	unsigned short pin = 0;//this->_config.Get<unsigned short>("pin", 0);
	bool value = this->_bridge->BridgeDigitalRead(pin);
	value = this->_debouncer->Value(value);

	if (this->_inputValue != value) {
		this->_inputValue = value;
		this->_inputChanged = true;
		this->_inputChangedSignal = true;
	}

	this->_capability("active:bool", String(value ? "1" : "0"));

	return true;
}

void Hardware::HTrigger::_signal (Protonix* device) {
	/*String mode = this->_config.Get<String>("mode", "");

	if (mode == "input") {
		String signal = this->_config.Get<String>("signal:InputChanged", "");

		device->Signal(this->_id, signal)->Value(this->_inputValue);
	}*/
}

Hardware::HTrigger::HTrigger (String mode, unsigned short pin) {
	this->_init = false;
	this->_inputChanged = false;
	this->_inputChangedSignal = false;
	this->_inputValue = false;

	this->_debouncer = new Qybercom::Debouncer<bool>(false);

	/*this->_config
		.Set("mode", mode)
		.Set("pin", pin)
		.Set("debounce", 0)
		.Set("initial", true)
		.Set("interrupt", false)
		.Set("signal:InputChanged", String("inputChanged"));*/

	//this->_interrupt = interrupt;
	//this->_pin = pin;
	//this->_input = input;
	//this->_inputInitial = inputInitial;
	//this->_filter.CheckInterval(checkInterval);

	//this->_signalInputChanged = "inputChanged";
}

Hardware::HTrigger* Hardware::HTrigger::Input (unsigned short pin) {
	return new Hardware::HTrigger("input", pin);
}

Hardware::HTrigger* Hardware::HTrigger::Output (unsigned short pin) {
	return new Hardware::HTrigger("output", pin);
}

/*unsigned short Hardware::HTrigger::Pin () {
	return this->_pin;
}

unsigned short Hardware::HTrigger::InputInitial () {
	return this->_inputInitial;
}

Hardware::HTrigger* Hardware::HTrigger::InputInitial (unsigned short value) {
	this->_inputInitial = value;

	return this;
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

Qybercom::Filter<unsigned short> &Hardware::HTrigger::Filter () {
	return this->_filter;
}*/

Qybercom::Debouncer<bool>* Hardware::HTrigger::Debouncer () {
	return this->_debouncer;
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
	Serial.println("[debug] trigger:OutputValue:1");
	String mode = "";//this->_config.Get<String>("mode", "");
	Serial.println("[debug] trigger:OutputValue:2");
	if (mode != "output") return false;

	this->_log("OutputValue " + String(value));

	unsigned short pin = 0;//this->_config.Get<unsigned short>("pin", 0);

	this->_bridge->BridgeDigitalWrite(pin, value);

	this->_capability("active:bool", String(value));

	return true;
}

/*void Hardware::HTrigger::HardwareConfigSet (String key, Any value) {
	if (key == "debounce")
		this->_debouncer->Threshold(value.As<unsigned int>(0));
}*/

String Hardware::HTrigger::HardwareSummary () {
	return "Trigger ";//this->_config.Get<String>("mode", "");
}

void Hardware::HTrigger::HardwareInitPre (Protonix* device) {
}

bool Hardware::HTrigger::HardwareI2C () {
	return this->_bridge->HardwareI2C();
}

bool Hardware::HTrigger::HardwareSPI () {
	return this->_bridge->HardwareSPI();
}

void Hardware::HTrigger::HardwareInitPost (Protonix* device) {
	unsigned short pin = 0;//this->_config.Get<unsigned short>("pin", 0);
	String mode = "";//this->_config.Get<String>("mode", "");

	if (mode == "input") {
		this->_bridge->BridgePinInitInputUp(pin, 0);//this->_config.Get<bool>("initial", true));

		/*if (this->_config.Get<bool>("interrupt", false))
			device->InterruptAttach(pin, CHANGE);*/

		this->_capability("value", "active:bool", "State of the trigger");
	}

	if (mode == "output") {
		this->_bridge->BridgePinMode(pin, OUTPUT);
		this->_bridge->BridgeDigitalWrite(pin, LOW);

		this->_capability("command", "output:<bool>", "Set trigger' output value");
		this->_capability("active:bool", "0");
	}

	this->_init = true;
}

void Hardware::HTrigger::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	/*if (!this->_config.Get<bool>("interrupt", false))
		this->_pipe();*/

	if (this->_inputChangedHandler())
		this->_signal(device);
}

void Hardware::HTrigger::HardwarePipeInterrupt (Protonix* device) {
	(void)device;

	//if (!this->_config.Get<bool>("interrupt", false)) return;
	return;

	this->_pipe();
}

void Hardware::HTrigger::HardwareOnReset (Protonix* device) {
	String mode = "";//this->_config.Get<String>("mode", "");

	if (mode == "input")
		this->_signal(device);

	if (mode == "output")
		this->OutputValue(false);
}

void Hardware::HTrigger::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}