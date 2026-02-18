#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HTrigger.h"

using namespace Qybercom::Protonix;

bool Hardware::HTrigger::_inputChangedHandler () {
	if (!this->_config["allowSignal"]) return false;

	bool out = false;

	if (this->_inputChangedSignal) {
		out = true;

		this->_inputChangedSignal = false;
	}

	return out;
}

bool Hardware::HTrigger::_pipe () {
	if (!this->_init) return false;

	String mode = this->_config["mode"];
	if (mode != "input") return false;

	unsigned short pin = this->_config["pin"];
	bool value = this->_bridge->BridgeDigitalRead(pin);
	bool dual = this->_bridge->BridgePinDualPolarity(pin);

	unsigned int debounce = this->_config["debounce"];
	this->_debouncer->Threshold(debounce);

	value = dual ? (this->_config["initial"] ? value : !value) : value;
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
	String mode = this->_config["mode"];

	if (mode == "input") {
		String signal = this->_config["signal:InputChanged"];

		device->Signal(this->_id, signal)->Data(this->_inputValue);
	}
}

Hardware::HTrigger::HTrigger (String mode, unsigned short pin) {
	this->_init = false;
	this->_inputChanged = false;
	this->_inputChangedSignal = false;
	this->_inputValue = false;

	this->_debouncer = new Qybercom::Pipes::Debouncer<bool>(false);

	this->_config["mode"] = mode;
	this->_config["pin"] = pin;
	this->_config["debounce"] = 0;
	this->_config["initial"] = false;
	this->_config["interrupt"] = false;
	this->_config["allowSignal"] = true;
	this->_config["signal:InputChanged"] = "inputChanged";
}

Hardware::HTrigger* Hardware::HTrigger::Input (unsigned short pin) {
	return new Hardware::HTrigger("input", pin);
}

Hardware::HTrigger* Hardware::HTrigger::Output (unsigned short pin) {
	return new Hardware::HTrigger("output", pin);
}

Qybercom::Pipes::Debouncer<bool>* Hardware::HTrigger::Debouncer () {
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
	String mode = this->_config["mode"];
	if (mode != "output") return false;

	this->_log("OutputValue " + String(value));

	unsigned short pin = this->_config["pin"];

	this->_bridge->BridgeDigitalWrite(pin, value);

	this->_capability("active:bool", String(value));

	return true;
}

String Hardware::HTrigger::HardwareSummary () {
	return "Trigger " + this->_config["mode"];
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
	unsigned short pin = this->_config["pin"];
	String mode = this->_config["mode"];

	if (mode == "input") {
		this->_bridge->BridgePinInitInputUp(pin, this->_config["initial"]);

		if (this->_config["interrupt"])
			device->InterruptAttach(pin, CHANGE);

		this->_capability("value", "active:bool", "State of the trigger");
	}

	if (mode == "output") {
		this->_bridge->BridgePinMode(pin, OUTPUT);
		this->_bridge->BridgeDigitalWrite(pin, false);

		this->_capability("command", "output:<bool>", "Set trigger' output value");
		this->_capability("active:bool", "0");
	}

	this->_init = true;
}

void Hardware::HTrigger::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	if (!this->_config["interrupt"])
		this->_pipe();

	if (this->_inputChangedHandler())
		this->_signal(device);
}

void Hardware::HTrigger::HardwarePipeInterrupt (Protonix* device) {
	(void)device;

	if (!this->_config["interrupt"]) return;

	this->_pipe();
}

void Hardware::HTrigger::HardwareOnReset (Protonix* device) {
	String mode = this->_config["mode"];

	if (mode == "input")
		this->_signal(device);

	if (mode == "output")
		this->OutputValue(false);
}

void Hardware::HTrigger::HardwareOnCommand (Protonix* device, const ProtonixCommand &command) {
	(void)device;
	String mode = this->_config["mode"];

	if (mode == "output") {
		bool value = command.Argument(0);

		this->OutputValue(value);
	}
}