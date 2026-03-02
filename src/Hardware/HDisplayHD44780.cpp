#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HDisplayHD44780.h"

using namespace Qybercom::Protonix;

bool Hardware::HDisplayHD44780::_pulseEnable (char data) {
	if (!this->_init) return false;

	unsigned short pinRS = this->_config["pinRS"];
	unsigned short pinRW = this->_config["pinRW"];
	unsigned short pinEN = this->_config["pinEN"];
	unsigned short pinD4 = this->_config["pinD4"];
	unsigned short pinD5 = this->_config["pinD5"];
	unsigned short pinD6 = this->_config["pinD6"];
	unsigned short pinD7 = this->_config["pinD7"];

	this->_bridge->BridgeDigitalWrite(pinRW, false);

	this->_bridge->BridgeDigitalWrite(pinRS, (data >> pinRS) & 1);
	this->_bridge->BridgeDigitalWrite(pinRW, (data >> pinRW) & 1);
	this->_bridge->BridgeDigitalWrite(pinD4, (data >> pinD4) & 1);
	this->_bridge->BridgeDigitalWrite(pinD5, (data >> pinD5) & 1);
	this->_bridge->BridgeDigitalWrite(pinD6, (data >> pinD6) & 1);
	this->_bridge->BridgeDigitalWrite(pinD7, (data >> pinD7) & 1);

	this->_bridge->BridgeDigitalWrite(pinEN, true);
	delayMicroseconds(1);
	this->_bridge->BridgeDigitalWrite(pinEN, false);
	delayMicroseconds(50);

	return true;
}

bool Hardware::HDisplayHD44780::_write4 (char nibble, bool rs) {
	if (!this->_init) return false;

	char data = 0;

	if (nibble & 0x01) data |= 1 << (unsigned short)this->_config["pinD4"];
	if (nibble & 0x02) data |= 1 << (unsigned short)this->_config["pinD5"];
	if (nibble & 0x04) data |= 1 << (unsigned short)this->_config["pinD6"];
	if (nibble & 0x08) data |= 1 << (unsigned short)this->_config["pinD7"];

	if (rs)
		data |= 1 << (unsigned short)this->_config["pinRS"];

	return this->_pulseEnable(data);
}

bool Hardware::HDisplayHD44780::_send (char value, bool rs) {
	if (!this->_init) return false;

	this->_write4(value >> 4, rs);
	this->_write4(value & 0x0F, rs);

	return true;
}

Hardware::HDisplayHD44780::HDisplayHD44780 (
	unsigned short pinRS,
	unsigned short pinRW,
	unsigned short pinEN,
	unsigned short pinBL,
	unsigned short pinD4,
	unsigned short pinD5,
	unsigned short pinD6,
	unsigned short pinD7
) {
	this->_init = false;

	this->_config.Listener(this);
	this->_config["pinRS"] = pinRS;
	this->_config["pinRW"] = pinRW;
	this->_config["pinEN"] = pinEN;
	this->_config["pinBL"] = pinBL;
	this->_config["pinD4"] = pinD4;
	this->_config["pinD5"] = pinD5;
	this->_config["pinD6"] = pinD6;
	this->_config["pinD7"] = pinD7;
	this->_config["backlight"] = true;
	this->_config["width"] = 20;
	this->_config["height"] = -1;
}

bool Hardware::HDisplayHD44780::Command (char cmd) {
	return this->_send(cmd, false);
}

bool Hardware::HDisplayHD44780::WriteChar (char value) {
	return this->_send(value, true);
}

bool Hardware::HDisplayHD44780::Write (String value) {
	if (!this->_init) return false;

	int i = 0;
	int len = value.length();

	int cursorX = this->_capability("cursorX:int").toInt();
	int cursorY = this->_capability("cursorY:int").toInt();

	while (i < len) {
		char c = value.charAt(i);

		this->WriteChar(c);

		if (c == '\n') cursorY++;
		else cursorX++;

		this->_capability("cursorX:int", String(cursorX));
		this->_capability("cursorY:int", String(cursorY));

		i++;
	}

	return true;
}

bool Hardware::HDisplayHD44780::Clear () {
	if (!this->_init) return false;

	this->Command(0x01);
	delay(2);

	return true;
}

bool Hardware::HDisplayHD44780::Cursor (int x, int y) {
	if (!this->_init) return false;

	static const char offsets[] = {0x00, 0x40, 0x14, 0x54};

	int cursorX = this->_capability("cursorX:int").toInt();
	int cursorY = this->_capability("cursorY:int").toInt();

	int col = x < 0 ? cursorX : x;
	int row = y < 0 ? cursorY : y;

	this->Command(0x80 | (offsets[row] + col));

	this->_capability("cursorX:int", String(col));
	this->_capability("cursorY:int", String(row));

	return true;
}

bool Hardware::HDisplayHD44780::Char (unsigned short code, char data[8]) {
	if (!this->_init) return false;

	this->Command(0x40 | (code << 3));
	delayMicroseconds(50);

	unsigned short i = 0;

	while (i < 8) {
		this->WriteChar(data[i]);

		i++;
	}

	int cursorX = this->_capability("cursorX:int").toInt();
	int cursorY = this->_capability("cursorY:int").toInt();

	return this->Cursor(cursorX, cursorY);
}

String Hardware::HDisplayHD44780::HardwareSummary () {
	return "DisplayHD44780 display";
}

void Hardware::HDisplayHD44780::HardwareInitPre (Protonix* device) {
	this->_capability("value", "backlight:bool", "Backlight");
	this->_capability("value", "cursorX:int", "Cursor column");
	this->_capability("value", "cursorY:int", "Cursor row");
	//this->_capability("value", "content:string", "Content");

	this->_capability("command", "backlight", "Control backlight");

	this->_init = true;
}

void Hardware::HDisplayHD44780::HardwareInitPost (Protonix* device) {
	unsigned short i = 0;
	while (i < 8) {
		this->_bridge->BridgePinInitOutput(i);

		i++;
	}

	this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinRW"], false);

	bool backlight = this->_config["backlight"];
	this->_config["backlight"] = backlight;
	this->_capability("backlight:bool", String(backlight));

	this->_write4(0x03, false);
	delay(5);
	this->_write4(0x03, false);
	delayMicroseconds(150);
	this->_write4(0x03, false);
	this->_write4(0x02, false);

	//this->Command(0x28); // 2 lines
	this->Command(0x0C); // display ON
	this->Command(0x01); // clear
	delay(2);
	this->Command(0x06); // entry mode
}

void Hardware::HDisplayHD44780::HardwarePipe (Protonix* device, short core) {
}

void Hardware::HDisplayHD44780::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "backlight") {
		bool value = command.Argument(2);

		this->_config["backlight"] = value;
	}

	if (cmd == "write") {
		String value = command.Argument(2);

		this->Write(value);
	}

	if (cmd == "clear") {
		this->Clear();
	}

	if (cmd == "cursor") {
		int x = command.Argument(2);
		int y = command.Argument(3);

		this->Cursor(x, y);
	}
}

void Hardware::HDisplayHD44780::ValueListenerSet (Value &value) {
	if (value.Key() == "backlight") {
		bool backlight = (bool)value;
		this->_capability("backlight:bool", String(backlight));

		if (this->_init)
			this->_bridge->BridgeDigitalWrite((unsigned short)this->_config["pinBL"], backlight);
	}
}