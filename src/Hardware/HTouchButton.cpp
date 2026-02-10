#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HTrigger.h"

#include "HTouchButton.h"

using namespace Qybercom::Protonix;

void Hardware::HTouchButton::_signal (Protonix* device) {
	String signalChanged = this->_config["signal:Changed"];
	if (!this->_config["allowSignal"] || !device->SignalSpawned(this->_id, signalChanged)) return;

	bool active = this->_trigger->InputValue();
	String signalPressed = this->_config["signal:Pressed"];
	String signalReleased = this->_config["signal:Released"];

	device->Signal(this->_id, String(active ? signalPressed : signalReleased));

	this->_capability("active:bool", String(active ? "1" : "0"));
}

Hardware::HTouchButton::HTouchButton (unsigned short pin) {
	this->_trigger = Hardware::HTrigger::Input(pin);

	this->_config.Listener(this);
	this->_config["pin"] = pin;
	this->_config["allowSignal"] = true;
	this->_config["signal:Changed"] = "changed";
	this->_config["signal:Pressed"] = "pressed";
	this->_config["signal:Released"] = "released";
}

Hardware::HTrigger* Hardware::HTouchButton::Trigger () {
	return this->_trigger;
}

bool Hardware::HTouchButton::Changed () {
	return this->_trigger->InputChanged();
}

bool Hardware::HTouchButton::Active () {
	return this->_trigger->InputValue();
}

bool Hardware::HTouchButton::Pressed (bool changed) {
	return (changed ? this->Changed() : true) && this->_trigger->InputValue();
}

bool Hardware::HTouchButton::Released (bool changed) {
	return (changed ? this->Changed() : true) && !this->_trigger->InputValue();
}

String Hardware::HTouchButton::HardwareSummary () {
	return "Touch button";
}

void Hardware::HTouchButton::HardwareInitPre (Protonix* device) {
	this->_trigger->HardwareConfig().Set("signal:InputChanged", this->_config["signal:Changed"]);

	this->_trigger->HardwareID(this->_id);
	this->_trigger->HardwareBridge(this->_bridge);
	this->_trigger->HardwareInitPre(device);

	this->_capability("value", "active:bool", "State of the button");
	this->_capability("active:bool", "0");
}

bool Hardware::HTouchButton::HardwareI2C () {
	return this->_trigger->HardwareI2C();
}

bool Hardware::HTouchButton::HardwareSPI () {
	return this->_trigger->HardwareSPI();
}

void Hardware::HTouchButton::HardwarePipe (Protonix* device, short core) {
	this->_trigger->HardwarePipe(device, core);

	this->_signal(device);
}

void Hardware::HTouchButton::HardwarePipeInterrupt (Protonix* device) {
	this->_trigger->HardwarePipeInterrupt(device);
}

void Hardware::HTouchButton::HardwareOnReset (Protonix* device) {
	this->_signal(device);
}

void Hardware::HTouchButton::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;

	// TODO: send command to trigger
}

void Hardware::HTouchButton::ValueListenerSet (Value &value) {
	this->_trigger->HardwareConfig().Replace(value);
}