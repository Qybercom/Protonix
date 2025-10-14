#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HTrigger.h"

#include "HButton.h"

using namespace Qybercom::Protonix;

void Hardware::HButton::_signal (Protonix* device) {
	if (!this->_allowSignal || !device->SignalSpawned(this->_id, this->_signalChanged)) return;

	bool active = this->_trigger->InputValue();

	device->Signal(this->_id, String(active ? this->_signalPressed : this->_signalReleased));

	this->_capability("active:bool", String(active ? "1" : "0"));
}

Hardware::HButton::HButton (unsigned short pin, unsigned short mode, unsigned int checkInterval) {
	this->_trigger = Hardware::HTrigger::Input(pin, mode, checkInterval);

	this->_signalChanged = "changed";
	this->_signalPressed = "pressed";
	this->_signalReleased = "released";
}

Hardware::HButton* Hardware::HButton::Init (unsigned short pin, unsigned short mode, unsigned int checkInterval) {
	return new Hardware::HButton(pin, mode, checkInterval);
}

Hardware::HTrigger* Hardware::HButton::Trigger () {
	return this->_trigger;
}

String Hardware::HButton::SignalChanged () {
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

String Hardware::HButton::HardwareSummary () {
	return "Button";
}

void Hardware::HButton::HardwareInitPre (Protonix* device) {
	this->_trigger->SignalInputChanged(this->_signalChanged);

	this->_trigger->HardwareID(this->_id);
	this->_trigger->HardwareAllowSignal(this->_allowSignal);
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