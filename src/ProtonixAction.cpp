#include <Arduino.h>

#include "ProtonixAction.h"
#include "ProtonixTimer.h"

using namespace Qybercom::Protonix;



void ProtonixAction::_init(String name, unsigned int interval, int stepBegin, int stepEnd, int step) {
	this->_completed = false;
	this->_timer = new ProtonixTimer(interval);

	this->Name(name);

	this->Step(step);
	this->StepBegin(stepBegin);
	this->StepEnd(stepEnd);

	this->_stepDirection = this->_stepBegin <= this->_stepEnd ? 1 : -1;
	this->_cursor = this->_stepBegin;
}

ProtonixAction::ProtonixAction(String name) {
	this->_init(name, 0, 0, 0, 1);
}

ProtonixAction::ProtonixAction(String name, unsigned int interval) {
	this->_init(name, interval, 0, 0, 1);
}

ProtonixAction::ProtonixAction(String name, unsigned int interval, int stepEnd) {
	this->_init(name, interval, 0, stepEnd, 1);
}

ProtonixAction::ProtonixAction(String name, unsigned int interval, int stepBegin, int stepEnd) {
	this->_init(name, interval, stepBegin, stepEnd, 1);
}

ProtonixAction::ProtonixAction(String name, unsigned int interval, int stepBegin, int stepEnd, int step) {
	this->_init(name, interval, stepBegin, stepEnd, step);
}

ProtonixTimer* ProtonixAction::Timer() {
	return this->_timer;
}

void ProtonixAction::Name(String name) {
	this->_name = name;
}

String ProtonixAction::Name() {
	return this->_name;
}

void ProtonixAction::Step(int step) {
	this->_step = step;
}

int ProtonixAction::Step() {
	return this->_step;
}

void ProtonixAction::StepBegin(int step) {
	this->_stepBegin = step;
}

int ProtonixAction::StepBegin() {
	return this->_stepBegin;
}

void ProtonixAction::StepEnd(int step) {
	this->_stepEnd = step;
}

int ProtonixAction::StepEnd() {
	return this->_stepEnd;
}

int ProtonixAction::StepDirection() {
	return this->_stepDirection;
}

int ProtonixAction::Cursor() {
	return this->_cursor;
}

bool ProtonixAction::Completed() {
	return this->_completed;
}

bool ProtonixAction::PipeStart() {
	return this->_timer->Pipe();
}

bool ProtonixAction::Pipe () {
	bool completed = false;
	if (this->_stepDirection ==  1) completed = (this->_cursor > this->_stepEnd);
	if (this->_stepDirection == -1) completed = (this->_cursor < this->_stepEnd);
	
	if (completed)
		this->_completed = true;
	
	return !this->_completed;
}

void ProtonixAction::PipeEnd() {
	this->_cursor += (this->_stepDirection * this->_step);
}

void ProtonixAction::Reset() {
	this->_cursor = this->_stepBegin;
	this->_completed = false;

	this->_timer->Reset();
}