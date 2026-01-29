#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HTrigger.h"

#include "HButton.h"

using namespace Qybercom::Protonix;

void Hardware::HButton::_signal (Protonix* device) {
	String signalChanged = "";//this->_config.Get<String>("signal:Changed", "");
	if (!this->_allowSignal || !device->SignalSpawned(this->_id, signalChanged)) return;

	bool active = this->_trigger->InputValue();
	String signalPressed = "";//this->_config.Get<String>("signal:Pressed", "");
	String signalReleased = "";//this->_config.Get<String>("signal:Released", "");

	device->Signal(this->_id, String(active ? signalPressed : signalReleased));

	this->_capability("active:bool", String(active ? "1" : "0"));
}

Hardware::HButton::HButton (unsigned short pin) {
	this->_trigger = Hardware::HTrigger::Input(pin);

	this->_config.Listener(this);

	/*this->_config
		.Set("signal:Changed", String("changed"))
		.Set("signal:Pressed", String("pressed"))
		.Set("signal:Released", String("released"));*/

	//this->_signalChanged = "changed";
	//this->_signalPressed = "pressed";
	//this->_signalReleased = "released";
}

Hardware::HButton* Hardware::HButton::Init (unsigned short pin) {
	return new Hardware::HButton(pin);
}

Hardware::HTrigger* Hardware::HButton::Trigger () {
	return this->_trigger;
}

/*String Hardware::HButton::SignalChanged () {
	return this->_signalChanged;
}

Hardware::HButton* Hardware::HButton::SignalChanged (String signal) {
	this->_signalChanged = signal;

	return this;
}

String Hardware::HButton::SignalPressed () {
	return this->_signalPressed;
}

Hardware::HButton* Hardware::HButton::SignalPressed (String signal) {
	this->_signalPressed = signal;

	return this;
}

String Hardware::HButton::SignalReleased () {
	return this->_signalReleased;
}

Hardware::HButton* Hardware::HButton::SignalReleased (String signal) {
	this->_signalReleased = signal;

	return this;
}*/

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

/*void Hardware::HButton::HardwareConfigSet (String key, Any value) {
	//this->_trigger->HardwareConfig()->Set(key, value);
}*/

String Hardware::HButton::HardwareSummary () {
	return "Button";
}

void Hardware::HButton::HardwareInitPre (Protonix* device) {
	//this->_trigger->HardwareConfig()->Set("signal:InputChanged", this->_signalChanged);

	this->_trigger->HardwareID(this->_id);
	this->_trigger->HardwareAllowSignal(this->_allowSignal);
	this->_trigger->HardwareBridge(this->_bridge);
	this->_trigger->HardwareInitPre(device);

	this->_capability("value", "active:bool", "State of the button");
	this->_capability("active:bool", "0");
}

bool Hardware::HButton::HardwareI2C () {
	return this->_trigger->HardwareI2C();
}

bool Hardware::HButton::HardwareSPI () {
	return this->_trigger->HardwareSPI();
}

void Hardware::HButton::HardwareInitPost (Protonix* device) {
	this->_trigger->HardwareInitPost(device);
}

void Hardware::HButton::HardwarePipe (Protonix* device, short core) {
	this->_trigger->HardwarePipe(device, core);

	this->_signal(device);
}

void Hardware::HButton::HardwarePipeInterrupt (Protonix* device) {
	this->_trigger->HardwarePipeInterrupt(device);
}

void Hardware::HButton::HardwareOnReset (Protonix* device) {
	this->_signal(device);
}

void Hardware::HButton::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;

	// TODO: send command to trigger
}

void Hardware::HButton::ValueListenerGet (Value &value) {
	Serial.println("[debug:hardware:button] Config get '" + value.Key() + "'");
}

void Hardware::HButton::ValueListenerSet (Value &value) {
	Serial.print("[debug:hardware:button] Config set '" + value.Key() + "': ");
	value.Dump();
}