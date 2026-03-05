#include <Arduino.h>
#include <OneWire.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HBusOneWire.h"

using namespace Qybercom::Protonix;

Hardware::HBusOneWire::HBusOneWire (unsigned short pin) {
	this->_port = nullptr;

	this->_config["pin"] = pin;
}

bool Hardware::HBusOneWire::Search () {
	if (!this->_init) return false;

	this->_port->reset_search();

	unsigned char addr[8];
	unsigned short i = 0;

	while (this->_port->search(addr)) {
		if (OneWire::crc8(addr, 7) != addr[7]) {
			this->_log("ROM CRC error");

			continue;
		}

		String address;

		i = 0;
		while (i < 8) {
			address += String(addr[i], HEX);

			i++;
		}
	}

	return true;
}

bool Hardware::HBusOneWire::Reset () {
	if (!this->_init) return false;

	this->_port->reset();

	return true;
}

bool Hardware::HBusOneWire::Select (String rom) {
	unsigned char r[8];
	this->ROMParse(rom, r);

	return this->Select(r);
}

bool Hardware::HBusOneWire::Select (unsigned char* rom) {
	if (!this->_init) return false;

	this->_port->select(rom);

	return true;
}

unsigned char Hardware::HBusOneWire::Read () {
	return this->_init ? this->_port->read() : 0;
}

bool Hardware::HBusOneWire::Write (unsigned char value) {
	if (!this->_init) return false;

	this->_port->write(value);

	return true;
}

bool Hardware::HBusOneWire::CRC8 (unsigned char* data, unsigned short length) {
	return OneWire::crc8(data, length - 1) != data[length - 1];
}

bool Hardware::HBusOneWire::ROMParse (String input, unsigned char* output) {
	int length = input.length();
	if (length != 16) return false;

	unsigned int i = 0;

	while (i < 8) {
		char bytes[3] = { input[i * 2], input[i * 2 + 1], 0 };
		char* end;

		output[i] = (unsigned char)strtoul(bytes, &end, 16);

		if (*end != 0) return false;

		i++;
	}

	return true;
}

String Hardware::HBusOneWire::HardwareSummary () {
	return "OneWire protocol bus";
}

void Hardware::HBusOneWire::HardwareInitPre (Protonix* device) {
	(void)device;
}

void Hardware::HBusOneWire::HardwareInitPost (Protonix* device) {
	(void)device;

	unsigned short pin = this->_config["pin"];
	this->_port = new OneWire(pin);

	this->_init = true;
}

void Hardware::HBusOneWire::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;
}

void Hardware::HBusOneWire::HardwarePipeInterrupt (Protonix* device) {
	(void)device;
}

void Hardware::HBusOneWire::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HBusOneWire::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	(void)command;
}

bool Hardware::HBusOneWire::BusSend (Protonix* device, String data) {
	(void)device;
	(void)data;

	return false;
}

bool Hardware::HBusOneWire::BusCommand (Protonix* device, String command) {
	(void)device;
	(void)command;

	return false;
}