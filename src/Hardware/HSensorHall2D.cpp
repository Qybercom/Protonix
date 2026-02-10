#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HSensorHall2D.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

void Hardware::HSensorHall2D::_signal (Protonix* device) {
	if (this->_config["allowSignal"])
		device->Signal(this->_id, "angle")->Data(this->_value);
}

Hardware::HSensorHall2D::HSensorHall2D (unsigned short pinX, unsigned short pinY, int maxX, int maxY, int minX, int minY) {
	this->_value = -1.0;

	this->_config["pinX"] = pinX;
	this->_config["pinY"] = pinY;
	this->_config["maxX"] = maxX;
	this->_config["maxY"] = maxY;
	this->_config["minX"] = minX;
	this->_config["minY"] = minY;
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

	int x = this->_bridge->BridgeAnalogRead(this->_config["pinX"]);
	int y = this->_bridge->BridgeAnalogRead(this->_config["pinY"]);

	this->_capability("valueX:int", String(x));
	this->_capability("valueY:int", String(y));

	float angle = angleByXY(
		x, y,
		this->_config["minX"], this->_config["maxX"],
		this->_config["minY"], this->_config["maxY"]
	);

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