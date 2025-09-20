#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HEncoder.h"

using namespace Qybercom::Protonix;

bool Hardware::HEncoder::_changedPipe () {
	bool out = false;

	if (this->_changed) {
		if (this->_allowZero || this->_dir != 0) out = true;

		this->_changed = false;
	}

	return out;
}

Hardware::HEncoder::HEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval) {
	this->_pinA = pinA;
	this->_pinB = pinB;
	this->_pinButton = 0;
	this->_valA = false;
	this->_valB = false;
	this->_dir = 0;
	this->_clicked = false;
	this->_changed = false;
	this->_changedButton = false;
	this->_withButton = false;
	this->_allowZero = false;

	this->_debouncer.CheckInterval(checkInterval);
}

Hardware::HEncoder* Hardware::HEncoder::WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton) {
	return Hardware::HEncoder::WithButton(pinA, pinB, pinButton, 0, 0);
}

Hardware::HEncoder* Hardware::HEncoder::WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval) {
	return Hardware::HEncoder::WithButton(pinA, pinB, pinButton, checkInterval, checkInterval);
}

Hardware::HEncoder* Hardware::HEncoder::WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval, unsigned int checkIntervalButton) {
	Hardware::HEncoder* out = new Hardware::HEncoder(pinA, pinB, checkInterval);

	out->_pinButton = pinButton;
	out->_withButton = true;
	out->_debouncerButton.CheckInterval(checkIntervalButton);

	return out;
}

unsigned short Hardware::HEncoder::PinA () {
	return this->_pinA;
}

unsigned short Hardware::HEncoder::PinB () {
	return this->_pinB;
}

unsigned short Hardware::HEncoder::PinButton () {
	return this->_pinButton;
}

bool Hardware::HEncoder::ValA () {
	return this->_valA;
}

bool Hardware::HEncoder::ValB () {
	return this->_valB;
}

short Hardware::HEncoder::Dir () {
	return this->_dir;
}

bool Hardware::HEncoder::Clicked () {
	return this->_clicked;
}

bool Hardware::HEncoder::Changed () {
	return this->_changedPipe();
}

bool Hardware::HEncoder::ChangedButton () {
	bool out = false;

	if (this->_changedButton) {
		out = true;
		this->_changedButton = false;
	}

	return out;
}

bool Hardware::HEncoder::WithButton () {
	return this->_withButton;
}

bool Hardware::HEncoder::AllowZero () {
	return this->_allowZero;
}

Hardware::HEncoder* Hardware::HEncoder::AllowZero (bool allow) {
	this->_allowZero = allow;

	return this;
}

Qybercom::Debouncer<short> &Hardware::HEncoder::Debouncer () {
	return this->_debouncer;
}

Qybercom::Debouncer<bool> &Hardware::HEncoder::DebouncerButton () {
	return this->_debouncerButton;
}

bool Hardware::HEncoder::HardwareSPI () {
	return false;
}

void Hardware::HEncoder::HardwareInitPre (Protonix* device) {
	pinMode(this->_pinA, INPUT_PULLUP);
	device->InterruptAttach(this->_pinA, CHANGE);

	pinMode(this->_pinB, INPUT_PULLUP);
	device->InterruptAttach(this->_pinB, CHANGE);

	if (this->_withButton) {
		pinMode(this->_pinButton, INPUT_PULLUP);
		//device->InterruptAttach(this->_pinButton, CHANGE);
	}
}

void Hardware::HEncoder::HardwareInitPost (Protonix* device) {
	(void) device;
}

void Hardware::HEncoder::HardwarePipe (Protonix* device, short core) {
	(void)core;

	if (this->_withButton) {
		bool clicked = digitalRead(this->_pinButton);

		this->_debouncerButton.Use(clicked);

		if (this->_debouncerButton.Pipe()) {
			clicked = this->_debouncerButton.Empty() ? false : this->_debouncerButton.Actual();

			if (this->_clicked != clicked) {
				this->_clicked = clicked;
				this->_changedButton = true;
			}

			this->_debouncerButton.Reset();
		}
	}

	if (this->_allowSignal && this->_changedPipe())
		device->Signal(this->_id, "dir")->ValueInt(this->_dir);
}

void Hardware::HEncoder::HardwarePipeInterrupt (Protonix* device) {
	(void)device;

	bool valA = digitalRead(this->_pinA);
	bool valB = digitalRead(this->_pinB);
	short dir = 0;

	if (this->_valA == false && valA == true) {
		dir = valB == false ? -1 : 1;
	}

	this->_debouncer.Use(dir);

	if (this->_debouncer.Pipe()) {
		dir = this->_debouncer.Empty() ? 0 : this->_debouncer.Actual();

		if (this->_dir != dir) {
			this->_dir = dir;
			this->_changed = true;
		}

		this->_debouncer.Reset();
	}

	this->_valA = valA;
	this->_valB = valB;
}

void Hardware::HEncoder::HardwareCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}