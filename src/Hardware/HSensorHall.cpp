#include <Arduino.h>

#include "Common/Utils.hpp"

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HSensorHall.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

Hardware::HSensorHall::HSensorHall (unsigned short pin, int max, int min) {
	this->_value = -1;
	this->_pin = pin;
	this->_max = max;
	this->_min = min;
}

unsigned short Hardware::HSensorHall::Pin () {
	return this->_pin;
}

Hardware::HSensorHall* Hardware::HSensorHall::Pin (unsigned short pin) {
	this->_pin = pin;

	return this;
}

int Hardware::HSensorHall::Max () {
	return this->_max;
}

Hardware::HSensorHall* Hardware::HSensorHall::Max (int max) {
	this->_max = max;

	return this;
}

int Hardware::HSensorHall::Min () {
	return this->_min;
}

Hardware::HSensorHall* Hardware::HSensorHall::Min (int min) {
	this->_min = min;

	return this;
}

int Hardware::HSensorHall::Value () {
	return this->_value;
}

bool Hardware::HSensorHall::ValueMatch (int min, int max, bool minEQ, bool maxEQ) {
	return matchRange(this->_value, min, max, minEQ, maxEQ);
}

int Hardware::HSensorHall::ValueMapped (int min, int max) {
	return map(this->_value, this->_min, this->_max, min, max);
}

bool Hardware::HSensorHall::HardwareSPI () {
	return false;
}

void Hardware::HSensorHall::HardwareInitPre (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorHall::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorHall::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;

	this->_value = analogRead(this->_pin);
}

void Hardware::HSensorHall::HardwareCommand (ProtonixDevice* device, String command) {
	(void)device;

	
}