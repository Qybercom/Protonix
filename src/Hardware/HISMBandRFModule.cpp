#include <Arduino.h>
#include <RCSwitch.h>

#include "../Protonix.h"

#include "HISMBandRFModule.h"

using namespace Qybercom::Protonix;

void Hardware::HISMBandRFModule::_signal (Protonix* device, String value) {
	if (!this->_config["allowSignal"]) return;

	device->Signal(this->_id, "valueChanged")->Data(value);
}

Hardware::HISMBandRFModule::HISMBandRFModule (short pinRX, short pinTX) {
	this->_init = false;
	this->_remote = new RCSwitch();
	this->_value = "";
	this->_valueActual = "";
	this->_valueChanged = false;

	this->_config["pinRX"] = pinRX;
	this->_config["pinTX"] = pinTX;
	this->_config["allowSignal"] = true;
}

Hardware::HISMBandRFModule* Hardware::HISMBandRFModule::RX (unsigned short pin) {
	return new Hardware::HISMBandRFModule(pin, -1);
}

Hardware::HISMBandRFModule* Hardware::HISMBandRFModule::TX (unsigned short pin) {
	return new Hardware::HISMBandRFModule(-1, pin);
}

String Hardware::HISMBandRFModule::Value () {
	return this->_value;
}

String Hardware::HISMBandRFModule::ValueActual () {
	return this->_valueActual;
}

bool Hardware::HISMBandRFModule::ValueChanged () {
	bool out = this->_valueChanged;
	this->_valueChanged = false;

	return out;
}

bool Hardware::HISMBandRFModule::Send (String value) {
	if (!this->_init) return false;

	this->_remote->send(value.toInt(), 4);

	return true;
}

String Hardware::HISMBandRFModule::HardwareSummary () {
	return "ISM-band RF module";
}

void Hardware::HISMBandRFModule::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_capability("value", "value:string", "Current value");

	short pinRX = this->_config["pinRX"];
	if (pinRX > -1)
		this->_remote->enableReceive(digitalPinToInterrupt(pinRX));

	short pinTX = this->_config["pinTX"];
	if (pinTX > -1)
		this->_remote->enableTransmit(digitalPinToInterrupt(pinTX));

	this->_init = true;
}

void Hardware::HISMBandRFModule::HardwareInitPost (Protonix* device) {
	(void)device;
}

void Hardware::HISMBandRFModule::HardwarePipe (Protonix* device, short core) {
	int raw = this->_remote->available() ? this->_remote->getReceivedValue() : -1;
	String value = String(raw);
	this->_valueActual = value;

	this->_capability("value:string", value);

	if (raw == -1) return;

	if (this->_value != value) {
		this->_value = value;
		this->_valueChanged = true;

		this->_signal(device, value);
	}
}

void Hardware::HISMBandRFModule::HardwareOnReset (Protonix* device) {
	this->_signal(device, this->_value);
}

void Hardware::HISMBandRFModule::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "send") {
		String value = command.Argument(2);

		this->Send(value);
	}
}