#include <Arduino.h>

#include <SoftwareSerial.h>

#include "../Protonix.h"

#include "HBusSerial.h"

using namespace Qybercom::Protonix;

Hardware::HBusSerial::HBusSerial (unsigned short pinRX, unsigned short pinTX, bool observable, unsigned int speed, unsigned int timeout, bool blocking) {
	this->_started = false;

	this->_pinRX = pinRX;
	this->_pinTX = pinTX;

	this->_speed = speed;

	this->Timeout(timeout);
	this->Blocking(blocking);
	this->Observable(observable);

	this->_cmdBuffer = "";
	this->_lenBuffer = "";

	this->_lenActive = false;

	this->_port = new SoftwareSerial(this->_pinRX, this->_pinTX);
}

bool Hardware::HBusSerial::Started () {
	return this->_started;
}

unsigned int Hardware::HBusSerial::PinRX () {
	return this->_pinRX;
}

unsigned int Hardware::HBusSerial::PinTX () {
	return this->_pinTX;
}

unsigned int Hardware::HBusSerial::Speed () {
	return this->_speed;
}

unsigned short Hardware::HBusSerial::Timeout () {
	return this->_timeout;
}

Hardware::HBusSerial* Hardware::HBusSerial::Timeout (unsigned short timeout) {
	this->_timeout = timeout;

	if (this->_started)
		this->_port->setTimeout(this->_timeout);

	return this;
}

bool Hardware::HBusSerial::Blocking () {
	return this->_blocking;
}

Hardware::HBusSerial* Hardware::HBusSerial::Blocking (bool blocking) {
	this->_blocking = blocking;

	return this;
}

bool Hardware::HBusSerial::Observable () {
	return this->_observable;
}

Hardware::HBusSerial* Hardware::HBusSerial::Observable (bool observable) {
	this->_observable = observable;

	return this;
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

bool Hardware::HBusSerial::Write (byte b) {
	if (!this->_started) return false;

	this->_port->write(b);

	return true;
}

byte Hardware::HBusSerial::Read () {
	return this->_started ? this->_port->read() : (byte)'\0';
}

String Hardware::HBusSerial::HardwareSummary () {
	return "Serial bus";
}

void Hardware::HBusSerial::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_port->begin(this->_speed);

	this->_started = true;

	this->Timeout(this->_timeout);
}

void Hardware::HBusSerial::HardwarePipe (Protonix* device, short core) {
	(void)core;
	if (!this->_observable) return;

	if (this->_cmdBuffer.length() >= 128)
		this->_cmdBuffer = "";

	int b = 0;

	if (this->_blocking) this->_cmdBuffer = this->_port->readStringUntil('\n');
	else b = this->_port->read();

	//this->_log("Pipe " + String(this->_cmdBuffer));

	if (this->_blocking) this->_cmdBuffer.trim();
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

bool Hardware::HBusSerial::HardwareBusSend (Protonix* device, String data) {
	(void)device;

	return this->Send(data);
}

bool Hardware::HBusSerial::HardwareBusCommand (Protonix* device, String command) {
	(void)device;

	return this->SendCommand(command);
}