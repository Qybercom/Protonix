#include <Arduino.h>
#include <SoftwareSerial.h>

#include <IProtonixCommand.h>
#include <ProtonixDevicePort.h>

#include "Command/CStdOn.h"
#include "Command/CStdOff.h"
#include "Command/CStdSensor.h"
#include "Command/CCustom.h"

using namespace Qybercom::Protonix;

SoftwareSerial __port__ = SoftwareSerial(0, 0);

void ProtonixDevicePort::_init(String name, unsigned short pinTX, unsigned short pinRX, unsigned int speed, unsigned short timeout) {
	this->_pinTX = pinTX;
	this->_pinRX = pinRX;

	#if !defined(ESP32) && !defined(ESP8266)
	__port__ = SoftwareSerial(this->_pinTX, this->_pinRX);
	this->_port = &__port__;
	#endif
	
	this->_speed = speed;

	this->Name(name);
	this->Timeout(timeout);

	this->_cmds[0] = new Command::CStdOn();
	this->_cmds[1] = new Command::CStdOff();
	this->_cmds[3] = new Command::CStdSensor();
	this->_cmds[2] = new Command::CCustom();
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned short pinTX, unsigned short pinRX) {
	this->_init(name, pinTX, pinRX, 9600, 7);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned short pinTX, unsigned short pinRX, unsigned int speed) {
	this->_init(name, pinTX, pinRX, speed, 7);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned short pinTX, unsigned short pinRX, unsigned int speed, unsigned short timeout) {
	this->_init(name, pinTX, pinRX, speed, timeout);
}

void ProtonixDevicePort::Name(String name) {
	this->_name = name;
}

String ProtonixDevicePort::Name() {
	return this->_name;
}

unsigned int ProtonixDevicePort::PinTX() {
	return this->_pinTX;
}

unsigned int ProtonixDevicePort::PinRX() {
	return this->_pinRX;
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
	#if defined(ESP32) || defined(ESP8266)
	this->_port.begin(this->_speed, SWSERIAL_8N1, this->_pinRX, this->_pinTX, false);
	this->_port.setTimeout(device->Tick());
	#else
	this->_port->begin(this->_speed);
	this->_port->setTimeout(device->Tick());
	#endif
}

/*void ProtonixDevicePort::_cmd(ProtonixDevice* device) {
	int i = 0;

	while (i < 4) {
		if (this->_cmds[i]->CommandIs(device, this, this->_cmdBuffer)) {
			this->_cmdID = i;
			this->_cmds[this->_cmdID]->CommandRecognized(device, this);
		}

		i++;
	}
}*/

void ProtonixDevicePort::Pipe(ProtonixDevice* device) {
	#if defined(ESP32) || defined(ESP8266)
		String s = this->_port.readStringUntil('\n');
	#else
		String s = this->_port->readStringUntil('\n');
	#endif

	s.trim();

	int i = 0;

	while (i < 4) {
		if (this->_cmds[i]->CommandRecognize(device, this, s)) {
			device->OnSerial(this, this->_cmds[i]);
		}

		i++;
	}
	/*
	return;

	#if defined(ESP32) || defined(ESP8266)
	int available = this->_port.available();
	#else
	int available = this->_port->available();
	#endif
	int i = 0;

	while (i < available) {
		#if defined(ESP32) || defined(ESP8266)
		char b = this->_port.read();
		#else
		char b = this->_port->read();
		#endif
		Serial.println("[debug] serial: '" + String(b) + "'");

		if (b == '\r') {
			i++;
			continue;
		}

		if (b == '\n') {
			if (this->_cmdID == -1)
				this->_cmd(device);

			if (this->_cmdID != -1)
				device->OnSerial(this, this->_cmds[this->_cmdID]);

			this->_cmdBuffer = "";
			this->_cmdID = -1;
		}
		else {
			if (this->_cmdID == -1) {
				if (b == ':') {
					if (this->_cmdBuffer == "std" || this->_cmdBuffer == "custom") {
						this->_cmdBuffer += b;
					}
					else {
						this->_cmd(device);
					}
				}
				else {
					this->_cmdBuffer += b;
				}
			}
			else {
				this->_cmds[this->_cmdID]->CommandParse(device, this, b);
			}
		}

		i++;
	}

	if (this->_cmdBuffer.length() > 32) {
		Serial.print("[WARNING] Serial cmdBuffer overflow");
		Serial.println(this->_cmdBuffer);

		this->_cmdBuffer = "";
		this->_cmdID = -1;
	}*/
}

bool ProtonixDevicePort::Send(IProtonixCommand* command) {
	if (!command->CommandSerialize()) {
		Serial.println("[WARNING] Can not serialize port command");

		return false;
	}

	#if defined(ESP32) || defined(ESP8266)
	this->_port.println(command->CommandOutput());
	#else
	this->_port->println(command->CommandOutput());
	#endif

	delay(this->_timeout);

	return true;
}