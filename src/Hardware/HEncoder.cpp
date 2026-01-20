#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HButton.h"

#include "HEncoder.h"

using namespace Qybercom::Protonix;

void Hardware::HEncoder::_signal (Protonix* device) {
	/*if (this->_allowSignal)
		device->Signal(this->_id, "dir")->Value(
			Hardware::HEncoderState(
				this->_dir,
				this->_button != nullptr && this->_button->Active()
			)
		);*/
}

Hardware::HEncoder::HEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval) {
	this->_pinA = pinA;
	this->_pinB = pinB;
	this->_valA = false;
	this->_valB = false;
	this->_dir = 0;
	this->_changed = false;
	this->_allowZero = false;

	this->_filter.CheckInterval(checkInterval);

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

	out->_button = new Hardware::HButton(pinButton);//, checkIntervalButton);

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

Qybercom::Filter<short> &Hardware::HEncoder::Filter () {
	return this->_filter;
}

String Hardware::HEncoder::HardwareSummary () {
	return "Encoder";
}

void Hardware::HEncoder::HardwareInitPre (Protonix* device) {
	pinMode(this->_pinA, INPUT_PULLUP);
	device->InterruptAttach(this->_pinA, CHANGE);

	pinMode(this->_pinB, INPUT_PULLUP);
	device->InterruptAttach(this->_pinB, CHANGE);

	if (this->_button != nullptr) {
		/*this->_button->HardwareConfig("signal:Changed", String("buttonChanged"));
		this->_button->HardwareConfig("signal:Pressed", String("buttonPressed"));
		this->_button->HardwareConfig("signal:Released", String("buttonReleased"));*/

		this->_button->HardwareID(this->_id);
		this->_button->HardwareAllowSignal(this->_allowSignal);
		this->_button->HardwareInitPre(device);
	}

	this->_capability("value", "dir:int", "The rotation direction");
}

void Hardware::HEncoder::HardwarePipe (Protonix* device, short core) {
	(void)core;

	if (this->_button != nullptr)
		this->_button->HardwarePipe(device, core);

	if (this->Changed())
		this->_signal(device);

	this->_capability("dir:int", String(this->_dir));
}

void Hardware::HEncoder::HardwarePipeInterrupt (Protonix* device) {
	(void)device;

	bool valA = digitalRead(this->_pinA);
	bool valB = digitalRead(this->_pinB);
	short dir = 0;

	if (this->_valA == false && valA == true) {
		dir = valB == false ? -1 : 1;
	}

	this->_filter.Use(dir);

	if (this->_filter.Pipe()) {
		dir = this->_filter.Empty() ? 0 : this->_filter.Actual();

		if (this->_dir != dir) {
			this->_dir = dir;
			this->_changed = true;
		}

		this->_filter.Reset();
	}

	this->_valA = valA;
	this->_valB = valB;

	if (this->_button != nullptr)
		this->_button->HardwarePipeInterrupt(device);
}

void Hardware::HEncoder::HardwareOnReset (Protonix* device) {
	this->_signal(device);
}

void Hardware::HEncoder::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}