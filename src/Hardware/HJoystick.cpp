#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"
#include "../ProtonixTimer.h"

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

	this->_positionChanged = false;
	this->_positionX = 0;
	this->_positionY = 0;
	this->_calibrateAuto = true;
	this->_calibrateTimeout = ProtonixTimer::Timeout(5000);
	this->_calibrated = false;
	this->_gapMaxX = this->_minX;
	this->_gapMinX = this->_maxX;
	this->_gapMaxY = this->_minY;
	this->_gapMinY = this->_maxY;
	this->_allowSignalValue = false;
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

bool Hardware::HJoystick::CalibrateAuto () {
	return this->_calibrateAuto;
}

Hardware::HJoystick* Hardware::HJoystick::CalibrateAuto (bool calibrate) {
	this->_calibrateAuto = calibrate;

	return this;
}

Hardware::HJoystick* Hardware::HJoystick::Calibrate () {
	this->_calibrated = false;
	this->_calibrateTimeout->Enabled(true);

	return this;
}

ProtonixTimer* Hardware::HJoystick::CalibrateTimeout () {
	return this->_calibrateTimeout;
}

Hardware::HJoystick* Hardware::HJoystick::Calibrate (int gapMinX, int gapMaxX, int gapMinY, int gapMaxY) {
	this->_gapMaxX = gapMaxX;
	this->_gapMinX = gapMinX;
	this->_gapMaxY = gapMaxY;
	this->_gapMinY = gapMinY;

	return this;
}

bool Hardware::HJoystick::AllowSignalValue () {
	return this->_allowSignalValue;
}

Hardware::HJoystick* Hardware::HJoystick::AllowSignalValue (bool allow) {
	this->_allowSignalValue = allow;

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

int Hardware::HJoystick::PositionX () {
	return this->_positionX;
}

int Hardware::HJoystick::PositionY () {
	return this->_positionY;
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

	int rawX = 0;
	int rawY = 0;

	if (this->_pinX > -1)
		rawX = analogRead(this->_pinX);

	if (this->_pinY > -1)
		rawY = analogRead(this->_pinY);

	if (this->_calibrateAuto && !this->_calibrated) {
		this->_calibrateTimeout->Enabled(true);

		if (this->_calibrateTimeout->Pipe()) {
			this->_calibrated = true;

			Serial.println(String("calibrate:")
				+ String(" maxX:") + String(this->_gapMaxX)
				+ String(" minX:") + String(this->_gapMinX)
				+ String(" maxY:") + String(this->_gapMaxY)
				+ String(" minY:") + String(this->_gapMinY)
			);
		}
		else {
			if (rawX > this->_gapMaxX) this->_gapMaxX = rawX;
			if (rawX < this->_gapMinX) this->_gapMinX = rawX;
			if (rawY > this->_gapMaxY) this->_gapMaxY = rawY;
			if (rawY < this->_gapMinY) this->_gapMinY = rawY;
		}
	}
	else {
		this->_valueX = rawX;
		this->_valueY = rawY;

		int positionX = Hardware::HJoystick::Position(
			this->_valueX,
			this->_minX,
			this->_maxX,
			this->_gapMinX,
			this->_gapMaxX
		);

		int positionY = Hardware::HJoystick::Position(
			this->_valueY,
			this->_minY,
			this->_maxY,
			this->_gapMinY,
			this->_gapMaxY
		);

		bool positionChanged = false
			|| positionX != this->_positionX
			|| positionY != this->_positionY;

		//Serial.println(String(positionX) + ":" + String(positionY));

		if (this->_allowSignal) {
			if (this->_allowSignalValue) {
				String valX = String(this->_valueX);
				String valY = String(this->_valueY);

				device->Signal(this->_id, "value")
					->ValueKV(new KeyValuePair(valX, valY))
					->ValueString(valX + ":" + valY);
			}

			if (positionChanged) {
				this->_positionChanged = false;
				this->_positionX = positionX;
				this->_positionY = positionY;

				String posX = String(this->_positionX);
				String posY = String(this->_positionY);

				device->Signal(this->_id, "position")
					->ValueKV(new KeyValuePair(posX, posY))
					->ValueString(posX + ":" + posY);
			}
		}
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

int Hardware::HJoystick::Position (int value, int valMin, int valMax, int gapMin, int gapMax) {
	if (value >= gapMin && value <= gapMax) return 0;

	int min = gapMin - valMin;
	if (min == 0) return 0;

	int max = valMax - gapMax;
	if (max == 0) return 0;

	float norm = value < gapMin
		? ((float)(value - gapMin) / min)
		: ((float)(value - gapMax) / max);

	return (int)(norm * 100);
}