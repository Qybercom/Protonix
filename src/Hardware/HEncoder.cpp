#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HButton.h"

#include "HEncoder.h"

using namespace Qybercom::Protonix;

Hardware::HEncoder::HEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval) {
	this->_pinA = pinA;
	this->_pinB = pinB;
	this->_valA = false;
	this->_valB = false;
	this->_dir = 0;
	this->_changed = false;
	this->_allowZero = false;

	this->_debouncer.CheckInterval(checkInterval);

	this->_button = nullptr;
}

Hardware::HEncoder* Hardware::HEncoder::Init (unsigned short pinA, unsigned short pinB, unsigned int checkInterval) {
	return new Hardware::HEncoder(pinA, pinB, checkInterval);
}

Hardware::HEncoder* Hardware::HEncoder::WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton) {
	return Hardware::HEncoder::WithButton(pinA, pinB, pinButton, 0, 0);
}

Hardware::HEncoder* Hardware::HEncoder::WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval) {
	return Hardware::HEncoder::WithButton(pinA, pinB, pinButton, checkInterval, checkInterval);
}

Hardware::HEncoder* Hardware::HEncoder::WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval, unsigned int checkIntervalButton) {
	Hardware::HEncoder* out = new Hardware::HEncoder(pinA, pinB, checkInterval);

	out->_button = new Hardware::HButton(pinButton, checkIntervalButton);

	return out;
}

unsigned short Hardware::HEncoder::PinA () {
	return this->_pinA;
}

unsigned short Hardware::HEncoder::PinB () {
	return this->_pinB;
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

bool Hardware::HEncoder::Changed () {
	bool out = false;

	if (this->_changed) {
		if (this->_allowZero || this->_dir != 0) out = true;

		this->_changed = false;
	}

	return out;
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

void Hardware::HEncoder::HardwareInitPre (Protonix* device) {
	pinMode(this->_pinA, INPUT_PULLUP);
	device->InterruptAttach(this->_pinA, CHANGE);

	pinMode(this->_pinB, INPUT_PULLUP);
	device->InterruptAttach(this->_pinB, CHANGE);

	if (this->_button != nullptr) {
		this->_button->SignalChanged("buttonChanged");
		this->_button->SignalPressed("buttonPressed");
		this->_button->SignalReleased("buttonReleased");

		this->_button->HardwareID(this->_id);
		this->_button->HardwareAllowSignal(this->_allowSignal);
		this->_button->HardwareInitPre(device);
	}
}

void Hardware::HEncoder::HardwarePipe (Protonix* device, short core) {
	(void)core;

	if (this->_button != nullptr)
		this->_button->HardwarePipe(device, core);

	if (this->_allowSignal && this->Changed())
		device->Signal(this->_id, "dir")->Value(Hardware::HEncoderState(this->_dir, this->_button != nullptr && this->_button->Active()));
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

	if (this->_button != nullptr)
		this->_button->HardwarePipeInterrupt(device);
}

void Hardware::HEncoder::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}