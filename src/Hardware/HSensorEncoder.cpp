#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HSensorEncoder.h"

using namespace Qybercom::Protonix;

Hardware::HSensorEncoder::HSensorEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval) {
	this->_pinA = pinA;
	this->_pinB = pinB;
	this->_valA = false;
	this->_valB = false;
	this->_dir = 0;
	this->_checkLast = 0;
	this->_checkInterval = checkInterval;

	/*this->_debounceValues[0] = 0;
	this->_debounceValues[1] = 0;
	this->_debounceValues[2] = 0;*/
}

unsigned short Hardware::HSensorEncoder::PinA () {
	return this->_pinA;
}

unsigned short Hardware::HSensorEncoder::PinB () {
	return this->_pinB;
}

bool Hardware::HSensorEncoder::ValA () {
	return this->_valA;
}

bool Hardware::HSensorEncoder::ValB () {
	return this->_valB;
}

short Hardware::HSensorEncoder::Dir () {
	return this->_dir;
}

bool Hardware::HSensorEncoder::Changed (bool allowZero) {
	bool out = this->_changed && (allowZero || this->_dir != 0);
	this->_changed = false;

	return out;
}

unsigned long Hardware::HSensorEncoder::CheckLast () {
	return this->_checkLast;
}

unsigned int Hardware::HSensorEncoder::CheckInterval () {
	return this->_checkInterval;
}

Qybercom::Debouncer<short> &Hardware::HSensorEncoder::Debouncer () {
	return this->_debouncer;
}

bool Hardware::HSensorEncoder::HardwareSPI () {
	return false;
}

void Hardware::HSensorEncoder::HardwareInitPre (ProtonixDevice* device) {
	pinMode(this->_pinA, INPUT_PULLUP);
	device->InterruptAttach(this->_pinA, CHANGE);

	pinMode(this->_pinB, INPUT_PULLUP);
	device->InterruptAttach(this->_pinB, CHANGE);
}

void Hardware::HSensorEncoder::HardwareInitPost (ProtonixDevice* device) {
	(void) device;

}

void Hardware::HSensorEncoder::HardwarePipe (ProtonixDevice* device, short core) {
	(void) device;


}

void Hardware::HSensorEncoder::HardwarePipeInterrupt (ProtonixDevice* device) {
	unsigned long now = micros();
	bool valA = digitalRead(this->_pinA);
	bool valB = digitalRead(this->_pinB);
	short dir = 0;

	if (this->_valA == false && valA == true) {
		dir = valB == false ? -1 : 1;
	}

	/*if (dir == -1) this->_debounceValues[0]++;
	if (dir == 0) this->_debounceValues[1]++;
	if (dir == 1) this->_debounceValues[2]++;*/
	Serial.println("[encoder] use1");
	this->_debouncer.Use(dir);
	Serial.println("[encoder] use2");

	if (now - this->_checkLast > this->_checkInterval) {
		/*dir = 0;
		if (this->_debounceValues[0] > this->_debounceValues[2]) dir = -1;
		if (this->_debounceValues[0] < this->_debounceValues[2]) dir = 1;*/
		Serial.println("[encoder] actual1");
		dir = this->_debouncer.Empty() ? 0 : this->_debouncer.Actual();
		Serial.println("[encoder] actual2");

		if (this->_dir != dir) {
			this->_dir = dir;
			this->_changed = true;
		}

		this->_checkLast = now;

		/*this->_debounceValues[0] = 0;
		this->_debounceValues[1] = 0;
		this->_debounceValues[2] = 0;*/
		Serial.println("[encoder] reset1");
		this->_debouncer.Reset();
		Serial.println("[encoder] reset2");
	}

	this->_valA = valA;
	this->_valB = valB;
}