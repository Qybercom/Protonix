#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"
#include "../ProtonixTimer.h"

#include "HButton.h"

#include "HJoystick.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

void Hardware::HJoystick::_signalValue (Protonix* device) {
	if (this->_config["allowSignal"] && this->_config["allowSignalValue"]) {
		device
			->Signal(this->_id, "value")
			->DataRaw(Hardware::HJoystickState(
				this->_valueX, this->_valueY,
				this->_button != nullptr && this->_button->Active(),
				this->_calibrated
			));
	}
}

void Hardware::HJoystick::_signalPosition (Protonix* device) {
	if (this->_config["allowSignal"])
		device
			->Signal(this->_id, "position")
			->DataRaw(Hardware::HJoystickState(
				this->_positionX, this->_positionY,
				this->_button != nullptr && this->_button->Active(),
				this->_calibrated
			));
}

Hardware::HJoystick::HJoystick (short pinX, short pinY, short pinButton, bool init) {
	this->_config.Listener(this);
	this->_config["pinX"] = pinX;
	this->_config["pinY"] = pinY;
	this->_config["init"] = init;
	this->_config["maxX"] = QYBERCOM_PROTONIX_ANALOG_MAX;
	this->_config["minX"] = 0;
	this->_config["maxY"] = QYBERCOM_PROTONIX_ANALOG_MAX;
	this->_config["minY"] = 0;
	this->_config["calibrateAuto"] = true;
	this->_config["allowSignal"] = true;
	this->_config["allowSignalValue"] = false;

	if (pinButton < 0) this->_button = nullptr;
	else {
		this->_button = new Hardware::HButton(pinButton);
		this->_button->HardwareConfig()
			.Set("signal:Changed", "buttonChanged")
			.Set("signal:Pressed", "buttonPressed")
			.Set("signal:Released", "buttonReleased");

		this->_config["button"] = this->_button->HardwareConfig();
	}

	this->_valueX = -1;
	this->_valueY = -1;
	this->_positionChanged = false;
	this->_positionX = 0;
	this->_positionY = 0;
	this->_calibrateTimeout = ProtonixTimer::Timeout(5000);
	this->_calibrated = false;
	this->_gapMaxX = this->_config["maxX"];
	this->_gapMinX = this->_config["minX"];
	this->_gapMaxY = this->_config["maxY"];
	this->_gapMinY = this->_config["minY"];
	this->_first = false;
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

String Hardware::HJoystick::HardwareSummary () {
	return "Joystick";
}

void Hardware::HJoystick::HardwareInitPre (Protonix* device) {
	(void)device;

	if (this->_config["init"]) {
		if ((int)this->_config["pinX"] > -1)
			this->_bridge->BridgePinMode(this->_config["pinX"], INPUT);

		if ((int)this->_config["pinY"] > -1)
			this->_bridge->BridgePinMode(this->_config["pinY"], INPUT);
	}

	this->_capability("value", "valueX:int", "Raw value of X-axis");
	this->_capability("value", "valueY:int", "Raw value of Y-axis");

	this->_capability("value", "positionX:int", "Interpolated X-axis position (0..100%)");
	this->_capability("value", "positionY:int", "Interpolated Y-axis position (0..100%)");

	if (this->_button != nullptr) {
		this->_button->HardwareID(this->_id);
		this->_button->HardwareBridge(this->_bridge);
		this->_button->HardwareInitPre(device);

		this->_capability("value", "active:bool", "State of the joystick button");
	}
}

void Hardware::HJoystick::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	int rawX = 0;
	int rawY = 0;

	if ((int)this->_config["pinX"] > -1)
		rawX = this->_bridge->BridgeAnalogRead(this->_config["pinX"]);

	if ((int)this->_config["pinY"] > -1)
		rawY = this->_bridge->BridgeAnalogRead(this->_config["pinY"]);

	if (this->_button != nullptr) {
		this->_button->HardwarePipe(device, core);

		this->_capability("active:bool", String(this->_button->Active() ? "1" : "0"));
	}

	if (this->_config["calibrateAuto"] && !this->_calibrated) {
		this->_calibrateTimeout->Enabled(true);

		if (this->_calibrateTimeout->Pipe()) {
			this->_calibrated = true;

			if (this->_config["allowSignal"]) {
				Value data = Value::Object();
				data["gapMaxX"] = this->_gapMaxX;
				data["gapMinX"] = this->_gapMinX;
				data["gapMaxY"] = this->_gapMaxY;
				data["gapMinY"] = this->_gapMinY;

				device->Signal(this->_id, "calibrated")->Data(data);
			}
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

		this->_capability("valueX:int", String(rawX));
		this->_capability("valueY:int", String(rawY));

		int positionX = Hardware::HJoystick::Position(
			this->_valueX,
			this->_config["minX"],
			this->_config["maxX"],
			this->_gapMinX,
			this->_gapMaxX
		);

		int positionY = Hardware::HJoystick::Position(
			this->_valueY,
			this->_config["minY"],
			this->_config["maxY"],
			this->_gapMinY,
			this->_gapMaxY
		);

		this->_capability("positionX:int", String(positionX));
		this->_capability("positionY:int", String(positionY));

		bool positionChanged = !this->_first
			|| positionX != this->_positionX
			|| positionY != this->_positionY;

		if (!this->_first)
			this->_first = true;

		this->_signalValue(device);

		if (positionChanged) {
			this->_positionChanged = false;
			this->_positionX = positionX;
			this->_positionY = positionY;

			this->_signalPosition(device);
		}
	}
}

void Hardware::HJoystick::HardwarePipeInterrupt (Protonix* device) {
	if (this->_button != nullptr)
		this->_button->HardwarePipeInterrupt(device);
}

void Hardware::HJoystick::HardwareOnReset (Protonix* device) {
	this->_signalValue(device);
	this->_signalPosition(device);
}

void Hardware::HJoystick::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
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

void Hardware::HJoystick::ValueListenerSet (Value &value) {
	if (this->_button != nullptr) {
		if (value.Contains("button"))
			this->_button->HardwareConfig().Replace(value);

		// TODO: handle direct `button` config
	}
}