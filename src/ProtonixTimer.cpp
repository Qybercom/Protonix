#include <Arduino.h>

#include "ProtonixTimer.h"

using namespace Qybercom::Protonix;



ProtonixTimer::ProtonixTimer (unsigned int interval, bool enabled) {
	this->_enabled = enabled;
	this->_previous = 0;
	this->_overflows = 0;

	this->Interval(interval);
	this->Unit(ProtonixTimer::ProtonixTimerUnit::MILLISECONDS);
}

ProtonixTimer::ProtonixTimer (unsigned int interval, ProtonixTimer::ProtonixTimerUnit unit, bool enabled) {
	this->_enabled = enabled;
	this->_previous = 0;
	this->_overflows = 0;

	this->Interval(interval);
	this->Unit(unit);
}

unsigned long ProtonixTimer::Previous () {
	return this->_previous;
}

unsigned long long ProtonixTimer::TotalMilliseconds () {
	return this->_totalMS;
}

unsigned long long ProtonixTimer::TotalSeconds () {
	return this->_totalS;
}

unsigned int ProtonixTimer::RunDays () {
	return this->_totalS / 86400;
}

unsigned int ProtonixTimer::RunHours () {
	return (this->_totalS % 86400) / 3600;
}

unsigned int ProtonixTimer::RunMinutes () {
	return (this->_totalS % 3600) / 60;
}

unsigned int ProtonixTimer::RunSeconds () {
	return this->_totalS % 60;
}

unsigned int ProtonixTimer::RunMilliseconds () {
	return this->_totalMS % 1000;
}

String ProtonixTimer::RunTime () {
	unsigned int h = this->RunHours();
	unsigned int m = this->RunMinutes();
	unsigned int s = this->RunSeconds();
	unsigned int ms = this->RunMilliseconds();

	return ""
		   + String(this->RunDays()) + String(":")
		   + String(h < 10 ? "0" : "") + String(h) + String(":")
		   + String(m < 10 ? "0" : "") + String(m) + String(":")
		   + String(s < 10 ? "0" : "") + String(s) + String(":")
		   + String(ms < 10 ? "00" : String(ms < 100 ? "0" : "")) + String(ms);
}

void ProtonixTimer::Interval (int interval) {
	this->_interval = interval;
}

unsigned int ProtonixTimer::Interval () {
	return this->_interval;
}

void ProtonixTimer::Unit (ProtonixTimer::ProtonixTimerUnit unit) {
	this->_unit = unit;
}

ProtonixTimer::ProtonixTimerUnit ProtonixTimer::Unit () {
	return this->_unit;
}

void ProtonixTimer::Enabled (bool enabled) {
	this->_enabled = enabled;
}

bool ProtonixTimer::Enabled () {
	return this->_enabled;
}

bool ProtonixTimer::Pipe () {
	if (!this->_enabled) return false;

	unsigned long current = 0;

	if (this->_unit == ProtonixTimer::ProtonixTimerUnit::MILLISECONDS)
		current = millis();

	if (this->_unit == ProtonixTimer::ProtonixTimerUnit::MICROSECONDS)
		current = micros();

	long diff = current - this->_previous;

	if (diff < 0)
		this->_overflows++;

	bool elapsed = diff < 0 || diff >= this->_interval;

	this->_totalMS = (unsigned long long)this->_overflows * TIMER_MAX + current;
	this->_totalS = this->_totalMS / 1000;

	if (elapsed)
		this->_previous = current;

	return elapsed;
}

void ProtonixTimer::Reset () {
	this->_previous = 0;
}