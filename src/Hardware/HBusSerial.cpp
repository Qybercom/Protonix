#include <Arduino.h>

#include <SoftwareSerial.h>

#include "../IProtonixCommand.h"
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

bool Hardware::HBusSerial::Send (IProtonixCommand* command) {
	if (!command->CommandSerialize()) {
		Serial.println("[hardware:busSerial] Can not serialize command");

		return false;
	}

	String out = command->CommandBuffer();
	if (out == "") return false;

	out += '\r';
	out += String(out.length());

	return this->Send(out);
}

bool Hardware::HBusSerial::Send (String s) {
	if (!this->_started) return false;

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

bool Hardware::HBusSerial::HardwareSPI () {
	return false;
}

void Hardware::HBusSerial::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_port->begin(this->_speed);

	this->_started = true;

	this->Timeout(this->_timeout);
}

void Hardware::HBusSerial::HardwareInitPost (Protonix* device) {
	(void)device;
}

void Hardware::HBusSerial::HardwarePipe (Protonix* device, short core) {
	(void)core;
	if (!this->_observable) return;

	if (this->_cmdBuffer.length() >= 128)
		this->_cmdBuffer = "";

	String s = "";
	int b = 0;

	if (this->_blocking) s = this->_port->readStringUntil('\n');
	else b = this->_port->read();

	//Serial.println("[debug] " + String(s));
	if (this->_blocking) s.trim();
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

	String lenBuffer = String(this->_blocking ? s.length() + 1 : this->_cmdBuffer.length() + 1);

	if (lenBuffer != this->_lenBuffer) {
		Serial.println("[hardware:busSerial] Message corrupted: e:" + this->_lenBuffer + " a:" + lenBuffer);
	}
	else {
		//Serial.println("[debug] cmd " + String(s));

		device->CommandRecognizeAndProcess(s, this);
	}

	this->_cmdBuffer = "";
	this->_lenBuffer = "";

	(void)device;
}

void Hardware::HBusSerial::HardwareCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}