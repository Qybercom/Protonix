#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HTrigger.h"

#include "HButton.h"

using namespace Qybercom::Protonix;

Hardware::HButton::HButton (unsigned short pin, unsigned int checkInterval) {
	this->_trigger = Hardware::HTrigger::Input(pin, HIGH, checkInterval);

	this->_signalChanged = "changed";
	this->_signalPressed = "pressed";
	this->_signalReleased = "released";
}

Hardware::HButton* Hardware::HButton::Init (unsigned short pin, unsigned int checkInterval) {
	return new Hardware::HButton(pin, checkInterval);
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

void Hardware::HButton::HardwareInitPre (Protonix* device) {
	this->_trigger->SignalInputChanged(this->_signalChanged);

	this->_trigger->HardwareID(this->_id);
	this->_trigger->HardwareAllowSignal(this->_allowSignal);
	this->_trigger->HardwareInitPre(device);
}

void Hardware::HButton::HardwarePipe (Protonix* device, short core) {
	this->_trigger->HardwarePipe(device, core);

	if (this->_allowSignal && device->SignalSpawned(this->_id, this->_signalChanged))
		device->Signal(this->_id, String(this->_trigger->InputValue()
			? this->_signalPressed
			: this->_signalReleased
		));
}

void Hardware::HButton::HardwarePipeInterrupt (Protonix* device) {
	this->_trigger->HardwarePipeInterrupt(device);
}

void Hardware::HButton::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;

	// TODO: send command to trigger
}