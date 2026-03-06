#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HBusOneWire.h"
#include "HTemperatureDS18.h"

using namespace Qybercom::Protonix;

Hardware::HTemperatureDS18::HTemperatureDS18 (String rom) {
	this->_value = 0.0;

	this->_config["ROM"] = rom;
}

float Hardware::HTemperatureDS18::Value () {
	return this->_value;
}

String Hardware::HTemperatureDS18::HardwareSummary () {
	return "DS18-compatible temperature sensor";
}

void Hardware::HTemperatureDS18::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_capability("value", "value:float", "Temperature value");
}

void Hardware::HTemperatureDS18::HardwareInitPost (Protonix* device) {
	(void)device;

	this->_init = true;
}

void Hardware::HTemperatureDS18::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	Hardware::HBusOneWire* bus = (Hardware::HBusOneWire*)this->_bus;
	if (bus == nullptr) return;

	String rom = this->_config["ROM"];

	bus->Reset();
	bus->Select(rom);
	bus->Write(0x44); // Start temperature conversion

	//delay(50);

	bus->Reset();
	bus->Select(rom);
	bus->Write(0xBE); // Read scratchpad

	unsigned char data[9];
	unsigned short i = 0;
	//String raw;

	while (i < 9) {
		data[i] = bus->Read();
		//raw += String((char)data[i], HEX);

		i++;
	}

	//if (!bus->CRC8(data, 9)) this->_log("Scratchpad CRC error " + raw);
	//else {
		long raw = (data[1] << 8) | data[0];

		this->_value = raw / 16.0;
	//}

	this->_capability("value:float", String(this->_value));
}

void Hardware::HTemperatureDS18::HardwarePipeInterrupt (Protonix* device) {
	(void)device;
}

void Hardware::HTemperatureDS18::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HTemperatureDS18::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	(void)command;
}