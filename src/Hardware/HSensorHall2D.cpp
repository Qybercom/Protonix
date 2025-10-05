#include <Arduino.h>

#include "Common/Utils.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HSensorHall2D.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

void Hardware::HSensorHall2D::_signal (Protonix* device) {
	if (this->_allowSignal)
		device->Signal(this->_id, "angle")->Value(this->_value);
}

Hardware::HSensorHall2D::HSensorHall2D (unsigned short pinX, unsigned short pinY, int maxX, int maxY, int minX, int minY) {
	this->_value = -1.0;
	this->_pinX = pinX;
	this->_pinY = pinY;
	this->_maxX = maxX;
	this->_maxY = maxY;
	this->_minX = minX;
	this->_minY = minY;
}

unsigned short Hardware::HSensorHall2D::PinX () {
	return this->_pinX;
}

Hardware::HSensorHall2D* Hardware::HSensorHall2D::PinX (unsigned short pinX) {
	this->_pinX = pinX;

	return this;
}

unsigned short Hardware::HSensorHall2D::PinY () {
	return this->_pinY;
}

Hardware::HSensorHall2D* Hardware::HSensorHall2D::PinY (unsigned short pinY) {
	this->_pinY = pinY;

	return this;
}

int Hardware::HSensorHall2D::MaxX () {
	return this->_maxX;
}

Hardware::HSensorHall2D* Hardware::HSensorHall2D::MaxX (int maxX) {
	this->_maxX = maxX;

	return this;
}

int Hardware::HSensorHall2D::MaxY () {
	return this->_maxY;
}

Hardware::HSensorHall2D* Hardware::HSensorHall2D::MaxY (int maxY) {
	this->_maxY = maxY;

	return this;
}

int Hardware::HSensorHall2D::MinX () {
	return this->_minX;
}

Hardware::HSensorHall2D* Hardware::HSensorHall2D::MinX (int minX) {
	this->_minX = minX;

	return this;
}

int Hardware::HSensorHall2D::MinY () {
	return this->_minY;
}

Hardware::HSensorHall2D* Hardware::HSensorHall2D::MinY (int minY) {
	this->_minY = minY;

	return this;
}

float Hardware::HSensorHall2D::ValueFloat () {
	return this->_value;
}

float Hardware::HSensorHall2D::ValueFloat (float offset) {
	return angleOffset(this->ValueFloat(), offset);
}

bool Hardware::HSensorHall2D::ValueFloatMatch (float offset, float min, float max, bool minEQ, bool maxEQ) {
	return matchRange(angleOffset(this->ValueFloat(), offset), min, max, minEQ, maxEQ);
}

int Hardware::HSensorHall2D::ValueInt () {
	return round(this->_value);
}

int Hardware::HSensorHall2D::ValueInt (int offset) {
	return angleOffset(this->ValueInt(), offset);
}

bool Hardware::HSensorHall2D::ValueIntMatch (int offset, int min, int max, bool minEQ, bool maxEQ) {
	return matchRange(angleOffset(this->ValueInt(), offset), min, max, minEQ, maxEQ);
}

String Hardware::HSensorHall2D::HardwareSummary () {
	return "Hall-effect 2-axis sensor";
}

void Hardware::HSensorHall2D::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_capability("value", "valueX:int", "Raw X-axis value");
	this->_capability("value", "valueY:int", "Raw Y-axis value");
	this->_capability("value", "angle:int", "Interpolated angle (0..360deg)");
}

void Hardware::HSensorHall2D::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	int x = analogRead(this->_pinX);
	int y = analogRead(this->_pinY);

	this->_capability("valueX:int", String(x));
	this->_capability("valueY:int", String(y));

	float angle = angleByXY(x, y, this->_minX, this->_maxX, this->_minY, this->_maxY);

	if (angle != this->_value) {
		this->_value = angle;

		this->_signal(device);
	}

	this->_capability("angle:int", String(angle));
}

void Hardware::HSensorHall2D::HardwareOnReset (Protonix* device) {
	this->_signal(device);
}

void Hardware::HSensorHall2D::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}