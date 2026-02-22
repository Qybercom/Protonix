#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HTM1637.h"

using namespace Qybercom::Protonix;

Hardware::TM1637Char::TM1637Char (char c) {
	Value(c);
}

Hardware::TM1637Char::TM1637Char (char c, bool dot) {
	Value(c, dot);
}

char Hardware::TM1637Char::Value () {
	return _value;
}

Hardware::TM1637Char &Hardware::TM1637Char::Value (char c) {
	_value = c;

	return *this;
}

Hardware::TM1637Char &Hardware::TM1637Char::Value (char c, bool dot) {
	_value = Encode(c);

	return Dot(dot);
}

Hardware::TM1637Char &Hardware::TM1637Char::Dot (bool on) {
	if (on) _value |= (1 << 7);
	else _value &= ~(1 << 7);

	return *this;
}

char Hardware::TM1637Char::Encode (char c) {
	switch (c) {
		case '0': return 0b00111111;
		case '1': return 0b00000110;
		case '2': return 0b01011011;
		case '3': return 0b01001111;
		case '4': return 0b01100110;
		case '5': return 0b01101101;
		case '6': return 0b01111101;
		case '7': return 0b00000111;
		case '8': return 0b01111111;
		case '9': return 0b01101111;
		case '-': return 0b01000000;
		case 'A': case 'a': return 0b01110111; // A B C E F G
		case 'B': case 'b': return 0b01111100; // C D E F G
		case 'C': return 0b00111001; // A D E F
		case 'c': return 0b01011000; // D E G
		case 'D': case 'd': return 0b01011110; // B C D E G
		case 'E': case 'e': return 0b01111001; // A D E F G
		case 'F': case 'f': return 0b01110001; // A E F G
		case 'G': case 'g': return 0b00111101; // A C D E F
		case 'H': return 0b01110110; // B C E F G
		case 'h': return 0b01110100; // C E F G
		case 'I': case 'i': return 0b00110000; // E F
		case 'J': case 'j': return 0b00011110; // B C D E
		case 'L': case 'l': return 0b00111000; // D E F
		case 'N': return 0b00110111; // A B C E F
		case 'n': return 0b01010100; // C E G
		case 'O': return 0b00111111; // C D E G
		case 'o': return 0b01011100; // C D E G
		case 'P': case 'p': return 0b01110011; // A B E F G
		case 'R': case 'r': return 0b01010000; // E G
		case 'S': case 's': return 0b01101101; // как 5
		case 'T': case 't': return 0b01111000; // D E F G
		case 'U': return 0b00111110; // B C D E F
		case 'u': return 0b00011100; // C D E
		case 'Y': case 'y': return 0b01101110; // B C D F G
		case ' ': return 0b00000000;
		default: return 0;
	}
}


bool Hardware::HTM1637::_start () {
	if (!this->_init) return false;

	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinDT"], true);
	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], true);

	delayMicroseconds(2);

	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinDT"], false);

	return true;
}

bool Hardware::HTM1637::_stop () {
	if (!this->_init) return false;

	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], false);
	delayMicroseconds(2);
	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinDT"], false);

	delayMicroseconds(2);

	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], true);
	delayMicroseconds(2);
	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinDT"], true);

	return true;
}

Hardware::HTM1637::HTM1637 (unsigned short pinCLK, unsigned short pinDT) {
	this->_init = false;

	this->_config.Listener(this);
	this->_config["pinCLK"] = pinCLK;
	this->_config["pinDT"] = pinDT;
	this->_config["brightness"] = 7; // 0..7
}

bool Hardware::HTM1637::Brightness (unsigned short brightness) {
	if (brightness > 7) brightness = 7;

	this->_capability("brightness:int", String(brightness));

	if (!this->_init) return false;

	this->_start();
	this->Write(0x88 | brightness);
	this->_stop();

	return true;
}

bool Hardware::HTM1637::Write (char b) {
	if (!this->_init) return false;

	unsigned short i = 0;

	while (i < 8) {
		this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], false);
		this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinDT"], b & 0x01 ? true : false);
		delayMicroseconds(3);
		this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], true);
		delayMicroseconds(3);

		b >>= 1;

		i++;
	}

	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], false);
	this->_bridge->BridgePinMode((unsigned short)this->_config["pinDT"], INPUT);
	delayMicroseconds(5);

	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], true);
	bool ack = (this->_bridge->BridgeDigitalRead((unsigned short)this->_config["pinDT"]) == 0);

	this->_bridge->BridgePinMode((unsigned short)this->_config["pinDT"], OUTPUT);
	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], false);

	return ack;
}

bool Hardware::HTM1637::Render (String value) {
	Hardware::TM1637Char c[4] = { ' ', ' ', ' ', ' ' };
	int length = value.length();
	int i = 0;
	int pos = 0;

	while (i < length && pos < 4) {
		char ci = value.charAt(i);

		if (ci == '.' || ci == ':') {
			if (pos > 0)
				c[pos - 1].Dot(true);
		}
		else {
			c[pos].Value(ci, false);

			pos++;
		}

		i++;
	}

	return this->Render(c[0], c[1], c[2], c[3]);
}

bool Hardware::HTM1637::Render (char c1, char c2, char c3, char c4) {
	return this->Render(
		Hardware::TM1637Char(c1),
		Hardware::TM1637Char(c2),
		Hardware::TM1637Char(c3),
		Hardware::TM1637Char(c4)
	);
}

bool Hardware::HTM1637::Render (Hardware::TM1637Char c1, Hardware::TM1637Char c2, Hardware::TM1637Char c3, Hardware::TM1637Char c4) {
	if (!this->_init) return false;

	this->_start();
	this->Write(0x40); // auto-increment
	this->_stop();

	this->_start();
	this->Write(0xC0); // address of 0
	this->Write(c1.Value());
	this->Write(c2.Value());
	this->Write(c3.Value());
	this->Write(c4.Value());
	this->_stop();

	return true;
}

bool Hardware::HTM1637::Clear () {
	return this->Render(
		0b00000000,
		0b00000000,
		0b00000000,
		0b00000000
	);
}

String Hardware::HTM1637::HardwareSummary () {
	return "TM1637 display";
}

void Hardware::HTM1637::HardwareInitPre (Protonix* device) {
	this->_bridge->BridgePinMode((unsigned short)this->_config["pinCLK"], OUTPUT);
	this->_bridge->BridgePinMode((unsigned short)this->_config["pinDT"], OUTPUT);

	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinCLK"], true);
	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinDT"], true);

	this->_capability("value", "brightness:int", "Brightness 0..7");
	//this->_capability("value", "content:string", "Content");

	this->_init = true;
}

void Hardware::HTM1637::HardwareInitPost (Protonix* device) {
	this->Brightness(this->_config["brightness"]);
	this->Clear();
}

void Hardware::HTM1637::HardwarePipe (Protonix* device, short core) {
}

void Hardware::HTM1637::ValueListenerSet (Value &value) {
	if (value.Key() == "brightness")
		this->Brightness((unsigned short)value);
}