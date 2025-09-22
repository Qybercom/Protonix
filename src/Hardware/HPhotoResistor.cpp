#include <Arduino.h>

#include "Common/Utils.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HPhotoResistor.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

Hardware::HPhotoResistor::HPhotoResistor (unsigned short pin, int max, int min) {
	this->_value = -1;
	this->_pin = pin;
	this->_max = max;
	this->_min = min;
}

unsigned short Hardware::HPhotoResistor::Pin () {
	return this->_pin;
}

Hardware::HPhotoResistor* Hardware::HPhotoResistor::Pin (unsigned short pin) {
	this->_pin = pin;

	return this;
}

int Hardware::HPhotoResistor::Max () {
	return this->_max;
}

Hardware::HPhotoResistor* Hardware::HPhotoResistor::Max (int max) {
	this->_max = max;

	return this;
}

int Hardware::HPhotoResistor::Min () {
	return this->_min;
}

Hardware::HPhotoResistor* Hardware::HPhotoResistor::Min (int min) {
	this->_min = min;

	return this;
}

int Hardware::HPhotoResistor::Value () {
	return this->_value;
}

bool Hardware::HPhotoResistor::ValueMatch (int min, int max, bool minEQ, bool maxEQ) {
	return matchRange(this->_value, min, max, minEQ, maxEQ);
}

int Hardware::HPhotoResistor::ValueMapped (int min, int max) {
	return map(this->_value, this->_min, this->_max, min, max);
}

bool Hardware::HPhotoResistor::HardwareSPI () {
	return false;
}

void Hardware::HPhotoResistor::HardwareInitPre (Protonix* device) {
	(void)device;

	pinMode(this->_pin, INPUT);
}

void Hardware::HPhotoResistor::HardwareInitPost (Protonix* device) {
	(void)device;
}

void Hardware::HPhotoResistor::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	this->_value = analogRead(this->_pin);
}

void Hardware::HPhotoResistor::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}