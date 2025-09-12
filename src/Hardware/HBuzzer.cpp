#include <Arduino.h>

#include "../IProtonixHardware.h"

#include "HBuzzer.h"

using namespace Qybercom::Protonix;

Hardware::HBuzzer::HBuzzer (unsigned short pin) {
	this->_pin = pin;
	this->_started = 0;
	this->_duration = 0;

	pinMode(this->_pin, OUTPUT);
}

unsigned short Hardware::HBuzzer::Pin () {
	return this->_pin;
}

Hardware::HBuzzer* Hardware::HBuzzer::Pin (unsigned short pin) {
	this->_pin = pin;

	return this;
}

void Hardware::HBuzzer::On (short value) {
	tone(this->_pin, value);
}

void Hardware::HBuzzer::Off () {
	noTone(this->_pin);
}

void Hardware::HBuzzer::Play (short value, unsigned long duration) {
	this->_started = millis();
	this->_duration = duration;

	this->Off();
	this->On(value);
}

void Hardware::HBuzzer::Stop () {
	this->_started = 0;
	this->_duration = 0;

	this->Off();
}

bool Hardware::HBuzzer::HardwareSPI () {
	return false;
}

void Hardware::HBuzzer::HardwareInitPre (ProtonixDevice* device) {
	(void)device;

	pinMode(this->_pin, OUTPUT);

	this->Off();
}

void Hardware::HBuzzer::HardwareInitPost (ProtonixDevice* device) {
	(void)device;
}

void Hardware::HBuzzer::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;

	unsigned long current = millis();
	unsigned long edge = this->_started + this->_duration;

	if (current >= edge) {
		this->_started = 0;
		this->_duration = 0;

		this->Off();
	}
}

void Hardware::HBuzzer::HardwareCommand (ProtonixDevice* device, String command) {
	(void)device;

	
}