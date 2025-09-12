#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HTrigger.h"

#include "HSelector3P.h"

using namespace Qybercom::Protonix;

Hardware::HSelector3P::HSelector3P (unsigned short pin1, unsigned short pin2, unsigned int checkInterval) {
	this->_trigger1 = Hardware::HTrigger::Input(pin1, HIGH, checkInterval);
	this->_trigger2 = Hardware::HTrigger::Input(pin2, HIGH, checkInterval);
	this->_value = 0;
	this->_changed = false;
}

Hardware::HTrigger* Hardware::HSelector3P::Trigger1 () {
	return this->_trigger1;
}

Hardware::HTrigger* Hardware::HSelector3P::Trigger2 () {
	return this->_trigger2;
}

bool Hardware::HSelector3P::Changed (bool allowZero) {
	bool out = false;

	if (this->_changed && (allowZero || this->_value != 0)) {
		out = true;
		this->_changed = false;
	}

	return out;
}

short Hardware::HSelector3P::Value () {
	return this->_value;
}

bool Hardware::HSelector3P::HardwareSPI () {
	return true
		&& this->_trigger1->HardwareSPI()
		&& this->_trigger2->HardwareSPI();
}

void Hardware::HSelector3P::HardwareInitPre (ProtonixDevice* device) {
	this->_trigger1->HardwareInitPre(device);
	this->_trigger2->HardwareInitPre(device);
}

void Hardware::HSelector3P::HardwareInitPost (ProtonixDevice* device) {
	this->_trigger1->HardwareInitPost(device);
	this->_trigger2->HardwareInitPost(device);
}

void Hardware::HSelector3P::HardwarePipe (ProtonixDevice* device, short core) {
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
}

void Hardware::HSelector3P::HardwareCommand (ProtonixDevice* device, String command) {
	(void)device;

	
}