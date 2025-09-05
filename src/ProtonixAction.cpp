#include <Arduino.h>

#include "ProtonixAction.h"
#include "ProtonixTimer.h"

using namespace Qybercom::Protonix;



void ProtonixAction::_init (String name, unsigned int interval, int stepBegin, int stepEnd, int step) {
	this->_active = false;
	this->_completed = false;
	this->_delayConsumed = false;
	this->_timer = new ProtonixTimer(interval);

	this->Name(name);

	this->Step(step);
	this->StepBegin(stepBegin);
	this->StepEnd(stepEnd);

	this->_stepDirection = this->_stepBegin <= this->_stepEnd ? 1 : -1;
	this->_cursor = this->_stepBegin;

	this->Infinite(false);
	this->Delayed(false);
	this->Queued(false);
}

ProtonixAction::ProtonixAction (String name) {
	this->_init(name, 0, 0, 0, 1);
}

ProtonixAction::ProtonixAction (String name, unsigned int interval) {
	this->_init(name, interval, 0, 0, 1);
}

ProtonixAction::ProtonixAction (String name, unsigned int interval, int stepEnd) {
	this->_init(name, interval, 0, stepEnd, 1);
}

ProtonixAction::ProtonixAction (String name, unsigned int interval, int stepBegin, int stepEnd) {
	this->_init(name, interval, stepBegin, stepEnd, 1);
}

ProtonixAction::ProtonixAction (String name, unsigned int interval, int stepBegin, int stepEnd, int step) {
	this->_init(name, interval, stepBegin, stepEnd, step);
}

ProtonixTimer* ProtonixAction::Timer () {
	return this->_timer;
}

void ProtonixAction::Name (String name) {
	this->_name = name;
}

String ProtonixAction::Name () {
	return this->_name;
}

void ProtonixAction::Step (int step) {
	this->_step = step;
}

int ProtonixAction::Step () {
	return this->_step;
}

void ProtonixAction::StepBegin (int step) {
	this->_stepBegin = step;
}

int ProtonixAction::StepBegin () {
	return this->_stepBegin;
}

void ProtonixAction::StepEnd (int step) {
	this->_stepEnd = step;
}

int ProtonixAction::StepEnd () {
	return this->_stepEnd;
}

int ProtonixAction::StepDirection () {
	return this->_stepDirection;
}

int ProtonixAction::Cursor () {
	return this->_cursor;
}

bool ProtonixAction::Active () {
	return this->_active;
}

bool ProtonixAction::Completed () {
	return this->_completed;
}

bool ProtonixAction::OneShot () {
	return this->_stepBegin == 0 && this->_stepEnd == 0;
}

bool ProtonixAction::DelayedVirtual () {
	return this->OneShot() && this->_timer->Interval() != 0;
}

String ProtonixAction::Summary () {
	return String(this->_stepBegin) + " -> " + String(this->_cursor) + "/" + String(this->_step) + ":" + String(this->_timer->Interval()) + " -> " + String(this->_stepEnd) + String(this->_infinite ? " *" : "");
}

bool ProtonixAction::Infinite () {
	return this->_infinite;
}

ProtonixAction* ProtonixAction::Infinite (bool infinite) {
	this->_infinite = infinite;

	return this;
}

bool ProtonixAction::Delayed () {
	return this->_delayed;
}

ProtonixAction* ProtonixAction::Delayed (bool delayed) {
	this->_delayed = delayed;

	return this;
}

bool ProtonixAction::Queued () {
	return this->_queued;
}

ProtonixAction* ProtonixAction::Queued (bool queued) {
	this->_queued = queued;

	return this;
}

ProtonixAction* ProtonixAction::Start () {
	this->_cursor = this->_stepBegin;
	this->_active = true;
	this->_completed = false;

	this->_timer->Enabled(true);

	return this;
}

ProtonixAction* ProtonixAction::Play () {
	this->_active = true;

	this->_timer->Enabled(true);

	return this;
}

ProtonixAction* ProtonixAction::Pause () {
	this->_active = false;

	this->_timer->Enabled(false);

	return this;
}

ProtonixAction* ProtonixAction::Stop () {
	this->_cursor = this->_stepEnd;
	this->_active = false;
	this->_completed = true;
	this->_delayConsumed = false;

	this->_timer->Enabled(false);

	return this;
}

bool ProtonixAction::PipePre () {
	if (!this->_active) return false;
	if (!this->_timer->Pipe()) return false;

	bool delayed = this->_delayed || this->DelayedVirtual();

	if (delayed && !this->_delayConsumed) {
		this->_delayConsumed = true;

		return false;
	}

	return true;
}

void ProtonixAction::PipePost () {
	this->_cursor += (this->_stepDirection * this->_step);

	this->_completed = false;
	if (this->_stepDirection == 1) this->_completed = this->_cursor >= this->_stepEnd;
	if (this->_stepDirection == -1) this->_completed = this->_cursor <= this->_stepEnd;

	if (this->_completed) {
		if (this->_infinite) this->Start();
		else this->Stop();
	}
}

void ProtonixAction::Reset () {
	this->_active = false;
	this->_completed = false;
	this->_cursor = this->_stepBegin;
	this->_delayConsumed = false;

	this->Queued(false);

	this->_timer->Reset();
}