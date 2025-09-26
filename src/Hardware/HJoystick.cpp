#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "Common//KeyValuePair.hpp"

#include "HButton.h"

#include "HJoystick.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

Hardware::HJoystick::HJoystick (short pinX, short pinY, short pinButton, bool init) {
	this->_pinX = pinX;
	this->_pinY = pinY;
	this->_init = init;
	this->_valueX = -1;
	this->_valueY = -1;
	this->_maxX = QYBERCOM_PROTONIX_ANALOG_MAX;
	this->_minX = 0;
	this->_maxY = QYBERCOM_PROTONIX_ANALOG_MAX;
	this->_minY = 0;

	if (pinButton > -1)
		this->_button = new Hardware::HButton(pinButton);
}

Hardware::HJoystick* Hardware::HJoystick::Init (short pinX, short pinY, short pinButton, bool init) {
	return new Hardware::HJoystick(pinX, pinY, pinButton, init);
}

short Hardware::HJoystick::PinX () {
	return this->_pinX;
}

short Hardware::HJoystick::PinY () {
	return this->_pinY;
}

bool Hardware::HJoystick::Init () {
	return this->_init;
}

int Hardware::HJoystick::MaxX () {
	return this->_maxX;
}

Hardware::HJoystick* Hardware::HJoystick::MaxX (int max) {
	this->_maxX = max;

	return this;
}

int Hardware::HJoystick::MinX () {
	return this->_minX;
}

Hardware::HJoystick* Hardware::HJoystick::MinX (int min) {
	this->_minX = min;

	return this;
}

int Hardware::HJoystick::MaxY () {
	return this->_maxY;
}

Hardware::HJoystick* Hardware::HJoystick::MaxY (int max) {
	this->_maxY = max;

	return this;
}

int Hardware::HJoystick::MinY () {
	return this->_minY;
}

Hardware::HJoystick* Hardware::HJoystick::MinY (int min) {
	this->_minY = min;

	return this;
}

Hardware::HButton* Hardware::HJoystick::Button () {
	return this->_button;
}

int Hardware::HJoystick::ValueX () {
	return this->_valueX;
}

int Hardware::HJoystick::ValueY () {
	return this->_valueY;
}

int Hardware::HJoystick::PositionX (int min, int max) {
	return map(this->_valueX, this->_minX, this->_maxX, min, max);
}

int Hardware::HJoystick::PositionY (int min, int max) {
	return map(this->_valueY, this->_minY, this->_maxY, min, max);
}

void Hardware::HJoystick::HardwareInitPre (Protonix* device) {
	(void)device;

	if (this->_init) {
		if (this->_pinX > -1)
			pinMode(this->_pinX, INPUT);

		if (this->_pinY > -1)
			pinMode(this->_pinY, INPUT);
	}

	if (this->_button != nullptr) {
		this->_button->SignalChanged("buttonChanged");
		this->_button->SignalPressed("buttonPressed");
		this->_button->SignalReleased("buttonReleased");

		this->_button->HardwareID(this->_id);
		this->_button->HardwareAllowSignal(this->_allowSignal);
		this->_button->HardwareInitPre(device);
	}
}

void Hardware::HJoystick::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	if (this->_pinX > -1)
		this->_valueX = analogRead(this->_pinX);

	if (this->_pinY > -1)
		this->_valueY = analogRead(this->_pinY);

	if (this->_allowSignal) {
		String valX = String(this->_valueX);
		String valY = String(this->_valueY);

		device->Signal(this->_id, "value")
			->ValueKV(new KeyValuePair(valX, valY))
			->ValueString(valX + ":" + valY);

		String posX = String(this->PositionX());
		String posY = String(this->PositionY());

		device->Signal(this->_id, "position")
			->ValueKV(new KeyValuePair(posX, posY))
			->ValueString(posX + ":" + posY);
	}

	if (this->_button != nullptr)
		this->_button->HardwarePipe(device, core);
}

void Hardware::HJoystick::HardwarePipeInterrupt (Protonix* device) {
	if (this->_button != nullptr)
		this->_button->HardwarePipeInterrupt(device);
}

void Hardware::HJoystick::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;

	// TODO: implement sending commands
}