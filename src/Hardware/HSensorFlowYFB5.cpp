#include <Arduino.h>

#include "../Common/index.h"

#include "../Protonix.h"
#include "../ProtonixTimer.h"

#include "HSensorFlowYFB5.h"

using namespace Qybercom::Protonix;

Hardware::HSensorFlowYFB5::HSensorFlowYFB5 (unsigned short pin) {
	this->_init = false;
	this->_pulse = 0;
	this->_flow = 0.0;
	this->_amount = 0.0;

	this->_config["pin"] = pin;
	this->_config["sampling"] = 1000;
}

String Hardware::HSensorFlowYFB5::HardwareSummary () {
	return "YF-B5 flow sensor";
}

void Hardware::HSensorFlowYFB5::HardwareInitPre (Protonix* device) {
	(void)device;

	unsigned short pin = this->_config["pin"];
	this->_bridge->BridgePinInitInputUp(pin, false);
	device->InterruptAttach(pin, CHANGE);

	this->_capability("value", "flow:float", "Raw value of X-axis");
	this->_capability("value", "amount:float", "Raw value of Y-axis");

	this->_capability("command", "zero", "Reset counts to 0");
	this->_capability("command", "zeroFlow", "Set 0 for flow");
	this->_capability("command", "zeroAmount", "Set 0 for amount");

	this->_init = true;
}

void Hardware::HSensorFlowYFB5::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;
	if (!this->_init) return;

	this->_capability("flow:float", String(this->_flow));
	this->_capability("amount:float", String(this->_amount));
}

void Hardware::HSensorFlowYFB5::HardwarePipeInterrupt (Protonix* device) {
	(void)device;
	if (!this->_init) return;

	unsigned short pin = this->_config["pin"];
	bool value = this->_bridge->BridgeDigitalRead(pin);

	unsigned int sampling = this->_config["sampling"];
	unsigned long now = micros();

	if (value) {
		unsigned long dt = now - this->_pulse;

		if (dt > sampling) {
			this->_pulse = now;

			this->_flow = 0.00225 / (dt / 1000000.0) * 60.0; // (L/min)
			this->_amount = this->_amount + 0.00225;
		}
	}
}

void Hardware::HSensorFlowYFB5::HardwareOnReset (Protonix* device) {
	(void)device;

	// TODO: reset counts, maybe
}

void Hardware::HSensorFlowYFB5::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "zero") {
		this->_flow = 0.0;
		this->_amount = 0.0;
	}

	if (cmd == "zeroFlow") {
		this->_flow = 0.0;
	}

	if (cmd == "zeroAmount") {
		this->_amount = 0.0;
	}
}