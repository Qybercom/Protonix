#include <Arduino.h>

#include "ProtonixTimer.h"

using namespace Qybercom::Protonix;



ProtonixTimer::ProtonixTimer() {
	this->_enabled = true;
	this->_previous = 0;

	this->Interval(0);
	this->Unit(ProtonixTimer::ProtonixTimerUnit::MILLISECONDS);
}

ProtonixTimer::ProtonixTimer(unsigned int interval) {
	this->_enabled = true;
	this->_previous = 0;

	this->Interval(interval);
	this->Unit(ProtonixTimer::ProtonixTimerUnit::MILLISECONDS);
}

ProtonixTimer::ProtonixTimer(unsigned int interval, ProtonixTimer::ProtonixTimerUnit unit) {
	this->_enabled = true;
	this->_previous = 0;

	this->Interval(interval);
	this->Unit(unit);
}

unsigned long ProtonixTimer::Previous() {
	return this->_previous;
}

void ProtonixTimer::Interval(int interval) {
	this->_interval = interval;
}

unsigned int ProtonixTimer::Interval() {
	return this->_interval;
}

void ProtonixTimer::Unit(ProtonixTimer::ProtonixTimerUnit unit) {
	this->_unit = unit;
}

ProtonixTimer::ProtonixTimerUnit ProtonixTimer::Unit() {
	return this->_unit;
}

void ProtonixTimer::Enabled(bool enabled) {
	this->_enabled = enabled;
}

bool ProtonixTimer::Enabled() {
	return this->_enabled;
}

bool ProtonixTimer::Pipe() {
	unsigned long current = 0;

	if (this->_unit == ProtonixTimer::ProtonixTimerUnit::MILLISECONDS)
		current = millis();

	if (this->_unit == ProtonixTimer::ProtonixTimerUnit::MICROSECONDS)
		current = micros();

	long diff = current - this->_previous;
	bool elapsed = diff < 0 || diff >= this->_interval;
	//bool elapsed = diff >= this->_interval;

	if (elapsed)
		this->_previous = current;

	return this->_enabled ? elapsed : false;
}

void ProtonixTimer::Reset() {
	this->_previous = 0;
}