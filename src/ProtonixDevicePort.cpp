#include <Arduino.h>
#include <SoftwareSerial.h>

#include <IProtonixCommand.h>
#include <ProtonixDevicePort.h>

#include "Command/CStdOn.h"
#include "Command/CStdOff.h"
#include "Command/CStdSensor.h"
#include "Command/CCustom.h"

using namespace Qybercom::Protonix;

#if !defined(ESP32) && !defined(ESP8266)
SoftwareSerial __port__ = SoftwareSerial(0, 0);
#endif

void ProtonixDevicePort::_init(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout) {
	this->_serial = false;

	this->_pinRX = pinRX;
	this->_pinTX = pinTX;

	#if !defined(ESP32) && !defined(ESP8266)
	__port__ = SoftwareSerial(this->_pinRX, this->_pinTX);
	this->_port = &__port__;
	#endif
	
	this->_speed = speed;

	this->Name(name);
	this->Timeout(timeout);

	this->_cmds[0] = new Command::CStdOn();
	this->_cmds[1] = new Command::CStdOff();
	this->_cmds[3] = new Command::CStdSensor();
	this->_cmds[2] = new Command::CCustom();
	// TODO: CStdReboot
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX) {
	this->_init(name, pinRX, pinTX, 9600, 7);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed) {
	this->_init(name, pinRX, pinTX, speed, 7);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout) {
	this->_init(name, pinRX, pinTX, speed, timeout);
}

ProtonixDevicePort::ProtonixDevicePort(String name) {
	this->_serial = true;

	this->Name(name);

	this->_cmds[0] = new Command::CStdOn();
	this->_cmds[1] = new Command::CStdOff();
	this->_cmds[3] = new Command::CStdSensor();
	this->_cmds[2] = new Command::CCustom();
	// TODO: CStdReboot
}

void ProtonixDevicePort::Name(String name) {
	this->_name = name;
}

String ProtonixDevicePort::Name() {
	return this->_name;
}

unsigned int ProtonixDevicePort::PinRX() {
	return this->_pinRX;
}

unsigned int ProtonixDevicePort::PinTX() {
	return this->_pinTX;
}

unsigned int ProtonixDevicePort::Speed() {
	return this->_speed;
}

void ProtonixDevicePort::Timeout(unsigned short timeout) {
	this->_timeout = timeout;
}

unsigned short ProtonixDevicePort::Timeout() {
	return this->_timeout;
}

void ProtonixDevicePort::Init(ProtonixDevice* device) {
	unsigned int tick = device->Tick();

	if (this->_serial) Serial.setTimeout(tick);
	else {
		#if defined(ESP32) || defined(ESP8266)
		this->_port.begin(this->_speed, SWSERIAL_8N1, this->_pinRX, this->_pinTX, false);
		this->_port.setTimeout(tick);
		#else
		this->_port->begin(this->_speed);
		this->_port->setTimeout(tick);
		#endif
	}
}

void ProtonixDevicePort::Pipe(ProtonixDevice* device) {
	//String s = "";
	int b = 0;
	//if (this->_serial) s = Serial.readStringUntil('\n');
	if (this->_serial) b = Serial.read();
	else {
		#if defined(ESP32) || defined(ESP8266)
			//s = this->_port.readStringUntil('\n');
			b = this->_port.read();
		#else
			//s = this->_port->readStringUntil('\n');
			b = this->_port->read();
		#endif
	}

	//s.trim();
	if (b == -1) return;
	
	char bc = (char)b;
	if (bc != '\n') {
		this->_cmdBuffer = this->_cmdBuffer + bc;
		//Serial.println("[debug] port " + this->_cmdBuffer);
		return;
	}

	int i = 0;

	while (i < 4) {
		//if (this->_cmds[i]->CommandRecognize(device, this, s)) {
		if (this->_cmds[i]->CommandRecognize(device, this, this->_cmdBuffer)) {
			device->OnSerial(this, this->_cmds[i]);
		}

		i++;
	}
	
	this->_cmdBuffer = "";
}

bool ProtonixDevicePort::Send(IProtonixCommand* command) {
	if (!command->CommandSerialize()) {
		Serial.println("[WARNING] Can not serialize port command");

		return false;
	}

	String output = command->CommandOutput();

	if (this->_serial) Serial.println(output);
	else {
		#if defined(ESP32) || defined(ESP8266)
		this->_port.println(output);
		#else
		this->_port->println(output);
		#endif
	}

	delay(this->_timeout);

	return true;
}