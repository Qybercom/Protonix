#include <Arduino.h>

#include <SoftwareSerial.h>

#include "../Protonix.h"

#include "HBusSerial.h"

using namespace Qybercom::Protonix;

Hardware::HBusSerial::HBusSerial (unsigned short pinRX, unsigned short pinTX) {
	this->_started = false;
	this->_lenActive = false;
	this->_lenBuffer = "";
	this->_cmdBuffer = "";

	this->_config["pinRX"] = pinRX;
	this->_config["pinTX"] = pinTX;
	this->_config["speed"] = 9600;
	this->_config["timeout"] = 0;
	this->_config["blocking"] = false;
	this->_config["observable"] = true;

	this->_port = new SoftwareSerial(
		(int)this->_config["pinRX"],
		(int)this->_config["pinTX"]
	);
}

bool Hardware::HBusSerial::Started () {
	return this->_started;
}

SoftwareSerial* Hardware::HBusSerial::Port () {
	return this->_port;
}

bool Hardware::HBusSerial::SendCommand (String command) {
	if (command == "") return false;

	command += '\r';
	command += String(command.length());

	this->_log("[debug] SendCommand " + command);

	return this->Send(command);
}

bool Hardware::HBusSerial::Send (String s) {
	if (!this->_started) return false;

	this->_log("[debug] Send " + s);

	this->_port->println(s);

	return true;
}

bool Hardware::HBusSerial::Write (char b) {
	if (!this->_started) return false;

	this->_port->write(b);

	return true;
}

char Hardware::HBusSerial::Read () {
	return this->_started ? this->_port->read() : (char)'\0';
}

String Hardware::HBusSerial::HardwareSummary () {
	return "Serial bus";
}

void Hardware::HBusSerial::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_started = true;

	this->_port->begin(this->_config["speed"]);
	this->_port->setTimeout(this->_config["timeout"]);
}

void Hardware::HBusSerial::HardwarePipe (Protonix* device, short core) {
	(void)core;
	if (!this->_config["observable"]) return;

	if (this->_cmdBuffer.length() >= 128)
		this->_cmdBuffer = "";

	int b = 0;

	if (this->_config["blocking"]) this->_cmdBuffer = this->_port->readStringUntil('\n');
	else b = this->_port->read();

	//this->_log("Pipe " + String(this->_cmdBuffer));

	if (this->_config["blocking"]) this->_cmdBuffer.trim();
	else {
		if (b == -1) return;
		char bc = (char)b;

		if (bc == '\n') this->_lenActive = false;
		else {
			if (bc == '\r') this->_lenActive = true;
			else {
				if (this->_lenActive) this->_lenBuffer += bc;
				else this->_cmdBuffer += bc;
			}

			return;
		}
	}

	String lenBuffer = String(this->_cmdBuffer.length() + 1);

	if (lenBuffer != this->_lenBuffer) {
		this->_log("Message corrupted: e:" + this->_lenBuffer + " a:" + lenBuffer);
	}
	else {
		//this->_log("Pipe:cmd " + String(this->_cmdBuffer));

		device->CommandRecognizeAndProcess(this->_cmdBuffer, this);
	}

	this->_cmdBuffer = "";
	this->_lenBuffer = "";

	(void)device;
}

void Hardware::HBusSerial::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HBusSerial::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}

bool Hardware::HBusSerial::BusSend (Protonix* device, String data) {
	(void)device;

	return this->Send(data);
}

bool Hardware::HBusSerial::BusCommand (Protonix* device, String command) {
	(void)device;

	return this->SendCommand(command);
}