#include <Arduino.h>
#include <SoftwareSerial.h>

#include <IProtonixCommand.h>
#include <ProtonixDevicePort.h>

#include "Command/CStdOn.h"
#include "Command/CStdOff.h"
#include "Command/CStdReboot.h"
#include "Command/CStdSensor.h"
#include "Command/CCustom.h"

using namespace Qybercom::Protonix;

#if !defined(ESP32) && !defined(ESP8266)
SoftwareSerial __port__ = SoftwareSerial(0, 0);
#endif

void ProtonixDevicePort::_init(bool serial, String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout, bool blocking, bool observable) {
	this->_started = false;
	this->_serial = serial;

	this->_pinRX = pinRX;
	this->_pinTX = pinTX;

	this->_speed = speed;

	this->Name(name);
	this->Timeout(timeout);
	this->Blocking(blocking);
	this->Observable(observable);

	#if !defined(ESP32) && !defined(ESP8266)
	__port__ = SoftwareSerial(this->_pinRX, this->_pinTX);
	this->_port = &__port__;
	#endif

	this->_cmdBuffer = "";
	this->_lenBuffer = "";
	
	this->_lenActive = false;
	
	this->_cmds[0] = new Command::CStdOn();
	this->_cmds[1] = new Command::CStdOff();
	this->_cmds[2] = new Command::CStdReboot();
	this->_cmds[3] = new Command::CStdSensor();
	this->_cmds[4] = new Command::CCustom();
}

byte ProtonixDevicePort::_crc8(String data) {
	/*byte crc = 0;
	byte buffer;

	int size = data.length();
	int i = 0;
	int j = 8;

	while (i < size) {
		buffer = (byte)data[i];
		j = 0;

		while (j > 0) {
			crc = ((crc ^ buffer) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
			buffer >>= 1;

			j--;
		}

		i++;
	}

	return crc;
	*/
	int start = 0;
	int cnt = data.length();

	unsigned char i, j;
	unsigned temp, temp2, flag;

	temp = 0xFFFF;

	for (i = start; i < cnt; i++) {
		temp = temp ^ data[i];

		for (j = 1; j <= 8; j++) {
			flag = temp & 0x0001;
			temp = temp >> 1;
			if (flag)
				temp = temp ^ 0xA001;
		}
	}

	/* Reverse byte order. */
	temp2 = temp >> 8;
	temp = (temp << 8) | temp2;
	temp &= 0xFFFF;

	return (byte)temp;
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX) {
	this->_init(false, name, pinRX, pinTX, 9600, 1000, false, true);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, bool blocking) {
	this->_init(false, name, pinRX, pinTX, 9600, 1000, blocking, true);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, bool blocking, bool observable) {
	this->_init(false, name, pinRX, pinTX, 9600, 1000, blocking, observable);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed) {
	this->_init(false, name, pinRX, pinTX, speed, 1000, false, true);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, bool blocking) {
	this->_init(false, name, pinRX, pinTX, speed, 1000, blocking, true);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, bool blocking, bool observable) {
	this->_init(false, name, pinRX, pinTX, speed, 1000, blocking, observable);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout) {
	this->_init(false, name, pinRX, pinTX, speed, timeout, false, true);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout, bool blocking) {
	this->_init(false, name, pinRX, pinTX, speed, timeout, blocking, true);
}

ProtonixDevicePort::ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout, bool blocking, bool observable) {
	this->_init(false, name, pinRX, pinTX, speed, timeout, blocking, observable);
}

ProtonixDevicePort::ProtonixDevicePort() {
	this->_init(true, "", 0, 0, 9600, 1000, false, true);
}

ProtonixDevicePort::ProtonixDevicePort(bool blocking) {
	this->_init(true, "", 0, 0, 9600, 1000, blocking, true);
}

ProtonixDevicePort::ProtonixDevicePort(bool blocking, bool observable) {
	this->_init(true, "", 0, 0, 9600, 1000, blocking, observable);
}

ProtonixDevicePort::ProtonixDevicePort(unsigned int speed) {
	this->_init(true, "", 0, 0, speed, 1000, false, true);
}

ProtonixDevicePort::ProtonixDevicePort(unsigned int speed, bool blocking) {
	this->_init(true, "", 0, 0, speed, 1000, blocking, true);
}

ProtonixDevicePort::ProtonixDevicePort(unsigned int speed, bool blocking, bool observable) {
	this->_init(true, "", 0, 0, speed, 1000, blocking, observable);
}

ProtonixDevicePort::ProtonixDevicePort(unsigned int speed, unsigned int timeout) {
	this->_init(true, "", 0, 0, speed, timeout, false, true);
}

ProtonixDevicePort::ProtonixDevicePort(unsigned int speed, unsigned int timeout, bool blocking) {
	this->_init(true, "", 0, 0, speed, timeout, blocking, true);
}

ProtonixDevicePort::ProtonixDevicePort(unsigned int speed, unsigned int timeout, bool blocking, bool observable) {
	this->_init(true, "", 0, 0, speed, timeout, blocking, observable);
}

bool ProtonixDevicePort::Started() {
	return this->_started;
}

bool ProtonixDevicePort::Serial() {
	return this->_serial;
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

void ProtonixDevicePort::Name(String name) {
	this->_name = name;
}

String ProtonixDevicePort::Name() {
	return this->_name;
}

void ProtonixDevicePort::Timeout(unsigned short timeout) {
	this->_timeout = timeout;
	if (!this->_started) return;
	
	if (this->_serial) ::Serial.setTimeout(this->_timeout);
	else {
		#if defined(ESP32) || defined(ESP8266)
		this->_port.setTimeout(this->_timeout);
		#else
		this->_port->setTimeout(this->_timeout);
		#endif
	}
}

unsigned short ProtonixDevicePort::Timeout() {
	return this->_timeout;
}

void ProtonixDevicePort::Blocking(bool blocking) {
	this->_blocking = blocking;
}

bool ProtonixDevicePort::Blocking() {
	return this->_blocking;
}

void ProtonixDevicePort::Observable(bool observable) {
	this->_observable = observable;
}

bool ProtonixDevicePort::Observable() {
	return this->_observable;
}

void ProtonixDevicePort::Init(ProtonixDevice* device) {
	(void)device;

	#if defined(ESP32) || defined(ESP8266)
	this->_port.begin(this->_speed, SWSERIAL_8N1, this->_pinRX, this->_pinTX, false);
	#else
	this->_port->begin(this->_speed);
	#endif
	
	this->_started = true;
	
	this->Timeout(this->_timeout);
}

void ProtonixDevicePort::Pipe(ProtonixDevice* device) {
	if (!this->_observable) return;
	
	if (this->_cmdBuffer.length() >= 128)
		this->_cmdBuffer = "";
	
	String s = "";
	int b = 0;
	
	if (this->_serial) {
		if (this->_blocking) s = ::Serial.readStringUntil('\n');
		else b = ::Serial.read();
	}
	else {
		#if defined(ESP32) || defined(ESP8266)
			if (this->_blocking) s = this->_port.readStringUntil('\n');
			else b = this->_port.read();
		#else
			if (this->_blocking) s = this->_port->readStringUntil('\n');
			else b = this->_port->read();
		#endif
	}

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

	int i = 0;
	String lenBuffer = String(this->_blocking ? s.length() + 1 : this->_cmdBuffer.length() + 1);
	//::Serial.println("[debug] " + this->_cmdBuffer);

	if (lenBuffer != this->_lenBuffer) {
		//::Serial.println("[WARNING] Message corrupted: e:" + this->_lenBuffer + " a:" + lenBuffer);
	}
	else {
		while (i < 4) {
			if (this->_cmds[i]->CommandRecognize(device, this, this->_blocking ? s : this->_cmdBuffer)) {
				device->OnSerial(this, this->_cmds[i]);
			}
	
			i++;
		}
	}
	
	this->_cmdBuffer = "";
	this->_lenBuffer = "";
}

bool ProtonixDevicePort::Send(IProtonixCommand* command) {
	if (!command->CommandSerialize()) {
		::Serial.println("[WARNING] Can not serialize port command");

		return false;
	}
	
	String output = command->CommandOutput();//String(.c_str());
	if (output == "") return false;
	
	output += '\r';
	output += String(output.length());

	if (this->_serial) ::Serial.println(output);
	else {
		#if defined(ESP32) || defined(ESP8266)
		this->_port.println(output);
		#else
		this->_port->println(output);
		#endif
	}

	return true;
}

bool ProtonixDevicePort::Write(byte b) {
	if (!this->_started) return false;
	
	if (this->_serial) ::Serial.write(b);
	else {
		#if defined(ESP32) || defined(ESP8266)
		this->_port.write(b);
		#else
		this->_port->write(b);
		#endif
	}
	
	return true;
}

byte ProtonixDevicePort::Read() {
	if (this->_serial) return ::Serial.read();
	else {
		#if defined(ESP32) || defined(ESP8266)
		return this->_port.read();
		#else
		return this->_port->read();
		#endif
	}
}