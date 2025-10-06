#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HTrigger.h"

#include "HSelector3P.h"

using namespace Qybercom::Protonix;

bool Hardware::HSelector3P::_changedPipe () {
	bool out = false;

	if (this->_changed) {
		if (this->_allowZero || this->_value != 0) out = true;

		this->_changed = false;
	}

	return out;
}

void Hardware::HSelector3P::_signal (Protonix* device, short value) {
	if (this->_allowSignal)
		device->Signal(this->_id, "changed")->Value(value);
}

Hardware::HSelector3P::HSelector3P (unsigned short pin1, unsigned short pin2, unsigned int checkInterval) {
	this->_trigger1 = Hardware::HTrigger::Input(pin1, HIGH, checkInterval);
	this->_trigger2 = Hardware::HTrigger::Input(pin2, HIGH, checkInterval);
	this->_value = 0;
	this->_changed = false;
	this->_allowZero = true;
}

Hardware::HTrigger* Hardware::HSelector3P::Trigger1 () {
	return this->_trigger1;
}

Hardware::HTrigger* Hardware::HSelector3P::Trigger2 () {
	return this->_trigger2;
}

bool Hardware::HSelector3P::Changed () {
	return this->_changedPipe();
}

short Hardware::HSelector3P::Value () {
	return this->_value;
}

bool Hardware::HSelector3P::AllowZero () {
	return this->_allowZero;
}

Hardware::HSelector3P* Hardware::HSelector3P::AllowZero (bool allow) {
	this->_allowZero = allow;

	return this;
}

String Hardware::HSelector3P::HardwareSummary () {
	return "Selector with 3 positions";
}

void Hardware::HSelector3P::HardwareInitPre (Protonix* device) {
	this->_trigger1->HardwareBridge(this->_bridge);
	this->_trigger2->HardwareBridge(this->_bridge);

	this->_trigger1->HardwareInitPre(device);
	this->_trigger2->HardwareInitPre(device);

	this->_capability("value", "value:int", "Current position");
}

void Hardware::HSelector3P::HardwarePipe (Protonix* device, short core) {
	this->_trigger1->HardwarePipe(device, core);
	this->_trigger2->HardwarePipe(device, core);

	short value1 = this->_trigger1->InputValue();
	short value2 = this->_trigger2->InputValue();

	if (value1 && value2) return; // abnormal

	short value = 0;
	if (value1) value = 1;
	if (value2) value = -1;

	if (this->_value != value) {
		this->_value = value;
		this->_changed = true;
	}

	if (this->_changedPipe())
		this->_signal(device, value);

	this->_capability("value:int", String(this->_value));
}

void Hardware::HSelector3P::HardwareOnReset (Protonix* device) {
	this->_signal(device, this->_value);
}

void Hardware::HSelector3P::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;

	// TODO: maybe transmit commands to triggers
}