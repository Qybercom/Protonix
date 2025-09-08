#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HEncoder.h"

using namespace Qybercom::Protonix;

Hardware::HEncoder::HEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval) {
	this->_pinA = pinA;
	this->_pinB = pinB;
	this->_valA = false;
	this->_valB = false;
	this->_dir = 0;
	this->_changed = false;

	this->_debouncer.CheckInterval(checkInterval);

	/*this->_debounceValues[0] = 0;
	this->_debounceValues[1] = 0;
	this->_debounceValues[2] = 0;*/
}

unsigned short Hardware::HEncoder::PinA () {
	return this->_pinA;
}

unsigned short Hardware::HEncoder::PinB () {
	return this->_pinB;
}

bool Hardware::HEncoder::ValA () {
	return this->_valA;
}

bool Hardware::HEncoder::ValB () {
	return this->_valB;
}

short Hardware::HEncoder::Dir () {
	return this->_dir;
}

bool Hardware::HEncoder::Changed (bool allowZero) {
	bool out = false;

	if (this->_changed && (allowZero || this->_dir != 0)) {
		out = true;
		this->_changed = false;
	}

	return out;
}

Qybercom::Debouncer<short> &Hardware::HEncoder::Debouncer () {
	return this->_debouncer;
}

bool Hardware::HEncoder::HardwareSPI () {
	return false;
}

void Hardware::HEncoder::HardwareInitPre (ProtonixDevice* device) {
	pinMode(this->_pinA, INPUT_PULLUP);
	device->InterruptAttach(this->_pinA, CHANGE);

	pinMode(this->_pinB, INPUT_PULLUP);
	device->InterruptAttach(this->_pinB, CHANGE);
}

void Hardware::HEncoder::HardwareInitPost (ProtonixDevice* device) {
	(void) device;

}

void Hardware::HEncoder::HardwarePipe (ProtonixDevice* device, short core) {
	(void) device;


}

void Hardware::HEncoder::HardwarePipeInterrupt (ProtonixDevice* device) {
	bool valA = digitalRead(this->_pinA);
	bool valB = digitalRead(this->_pinB);
	short dir = 0;

	if (this->_valA == false && valA == true) {
		dir = valB == false ? -1 : 1;
	}

	/*if (dir == -1) this->_debounceValues[0]++;
	if (dir == 0) this->_debounceValues[1]++;
	if (dir == 1) this->_debounceValues[2]++;*/
	Serial.println("[hardware:encoder] use1");
	this->_debouncer.Use(dir);
	Serial.println("[hardware:encoder] use2");

	if (this->_debouncer.Pipe()) {
		/*dir = 0;
		if (this->_debounceValues[0] > this->_debounceValues[2]) dir = -1;
		if (this->_debounceValues[0] < this->_debounceValues[2]) dir = 1;*/
		Serial.println("[hardware:encoder] actual1");
		dir = this->_debouncer.Empty() ? 0 : this->_debouncer.Actual();
		Serial.println("[hardware:encoder] actual2");

		if (this->_dir != dir) {
			this->_dir = dir;
			this->_changed = true;
		}

		/*this->_debounceValues[0] = 0;
		this->_debounceValues[1] = 0;
		this->_debounceValues[2] = 0;*/
		Serial.println("[hardware:encoder] reset1");
		this->_debouncer.Reset();
		Serial.println("[hardware:encoder] reset2");
	}

	this->_valA = valA;
	this->_valB = valB;
}