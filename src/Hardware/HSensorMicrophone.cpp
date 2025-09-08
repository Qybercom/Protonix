#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HSensorMicrophone.h"

using namespace Qybercom::Protonix;

Hardware::HSensorMicrophone::HSensorMicrophone (unsigned short pin) {
	this->_active = false;
	this->_pin = pin;
}

unsigned short Hardware::HSensorMicrophone::Pin () {
	return this->_pin;
}

Hardware::HSensorMicrophone* Hardware::HSensorMicrophone::Pin (unsigned short pin) {
	pinMode(this->_pin, INPUT);
	this->_pin = pin;

	return this;
}

bool Hardware::HSensorMicrophone::Active () {
	return this->_active;
}

bool Hardware::HSensorMicrophone::HardwareSPI () {
	return false;
}

void Hardware::HSensorMicrophone::HardwareInitPre (ProtonixDevice* device) {
	(void)device;

	this->Pin(this->_pin);
}

void Hardware::HSensorMicrophone::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorMicrophone::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;

	// TODO: refactor for analogRead
	this->_active = digitalRead(this->_pin);
}