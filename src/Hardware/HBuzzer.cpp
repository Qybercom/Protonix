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

void Hardware::HBuzzer::Play (short value, unsigned long duration) {
	this->_started = millis();
	this->_duration = duration;

	noTone(this->_pin);
	tone(this->_pin, value);
}

void Hardware::HBuzzer::Stop () {
	this->_started = 0;
	this->_duration = 0;

	noTone(this->_pin);
}

void Hardware::HBuzzer::HardwareInitPre (Protonix* device) {
	(void)device;

	pinMode(this->_pin, OUTPUT);

	this->Stop();
}

void Hardware::HBuzzer::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	unsigned long current = millis();
	unsigned long edge = this->_started + this->_duration;

	if (current >= edge)
		this->Stop();
}

void Hardware::HBuzzer::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}