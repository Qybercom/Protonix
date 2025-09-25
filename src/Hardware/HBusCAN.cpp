#include <Arduino.h>

#include <mcp2515.h>

#include "../Common/index.h"

#include "HBusCAN.h"

using namespace Qybercom::Protonix;

void Hardware::HBusCAN::_settings () {
	if (!this->_ready) return;

	this->_driver->setBitrate(this->_bitrate, this->_clock);
	this->_driver->setNormalMode();
}

String Hardware::HBusCAN::_statusRecognize (MCP2515::ERROR code) {
	switch (code) {
		case MCP2515::ERROR_OK: return "OK"; break;
		case MCP2515::ERROR_FAIL: return "FAIL"; break;
		case MCP2515::ERROR_ALLTXBUSY: return "ALLTXBUSY"; break;
		case MCP2515::ERROR_FAILINIT: return "FAILINIT"; break;
		case MCP2515::ERROR_FAILTX: return "FAILTX"; break;
		case MCP2515::ERROR_NOMSG: return "NOMSG"; break;
		default: return "__unknown__"; break;
	}
}

Hardware::HBusCAN::HBusCAN (unsigned short pinCS, byte address, CAN_SPEED bitrate, CAN_CLOCK clock, bool parse, unsigned int bufferMax) {
	this->_ready = false;
	this->_parse = parse;
	this->_address = address;
	this->_bitrate = bitrate;
	this->_clock = clock;

	this->_driver = new MCP2515(pinCS);

	int i = 0;
	while (i < 16) {
		this->_buffers[i] = "";

		i++;
	}

	this->_bufferMax = bufferMax;
}

Hardware::HBusCAN* Hardware::HBusCAN::Init (unsigned short pinCS, byte address, CAN_SPEED bitrate, CAN_CLOCK clock, bool parse, unsigned int bufferMax) {
	return new Hardware::HBusCAN(pinCS, address, bitrate, clock, bufferMax);
}

byte Hardware::HBusCAN::Address () {
	return this->_address;
}

Hardware::HBusCAN* Hardware::HBusCAN::Address (byte address) {
	this->_address = address;

	return this;
}

CAN_SPEED Hardware::HBusCAN::Bitrate () {
	return this->_bitrate;
}

Hardware::HBusCAN* Hardware::HBusCAN::Bitrate (CAN_SPEED bitrate) {
	this->_bitrate = bitrate;
	this->_settings();

	return this;
}

CAN_CLOCK Hardware::HBusCAN::Clock () {
	return this->_clock;
}

Hardware::HBusCAN* Hardware::HBusCAN::Clock (CAN_CLOCK clock) {
	this->_clock = clock;
	this->_settings();

	return this;
}

bool Hardware::HBusCAN::Parse () {
	return this->_parse;
}

Hardware::HBusCAN* Hardware::HBusCAN::Parse (bool parse) {
	this->_parse = parse;

	return this;
}

unsigned int Hardware::HBusCAN::BufferMax () {
	return this->_bufferMax;
}

Hardware::HBusCAN* Hardware::HBusCAN::BufferMax (unsigned int size) {
	this->_bufferMax = size;

	return this;
}

unsigned short Hardware::HBusCAN::ID (byte priority, byte address, bool truncated) {
	unsigned short id = 0;

	id |= ((byte)(priority & 0x03)) << 9;
	id |= ((byte)(truncated ? 1 : 0)) << 8;
	id |= ((byte)(address & 0x0F)) << 4;
	id |= ((byte)(this->_address & 0x0F));

	return id;
}

bool Hardware::HBusCAN::Send (struct can_frame* frame) {
	return this->_ready && this->_driver->sendMessage(frame) == MCP2515::ERROR_OK;
}

bool Hardware::HBusCAN::Send (unsigned short id, String data) {
	struct can_frame frame;
	int length = data.length();
	if (length > 8) length = 8;

	frame.can_id = id;
	frame.can_dlc = length;

	int i = 0;
	while (i < length) {
		frame.data[i] = (byte)data[i];

		i++;
	}

	return this->Send(&frame);
}

bool Hardware::HBusCAN::Send (String data, byte priority, byte address) {
	if (priority > 7) priority = 7;

	int length = data.length();
	int offset = 0;
	int i = 0;

	while (offset < length) {
		struct can_frame frame;
		int chunk = (length - offset > 8) ? 8 : (length - offset);
		bool truncated = (offset + chunk < length);

		frame.can_id = this->ID(priority, address, truncated);
		frame.can_dlc = chunk;

		i = 0;
		while (i < chunk) {
			frame.data[i] = (byte)data[offset + i];

			i++;
		}

		if (!this->Send(&frame)) return false;

		offset += chunk;
	}

	return true;
}

bool Hardware::HBusCAN::HardwareSPI () {
	return true;
}

void Hardware::HBusCAN::HardwareSPIPost (Protonix* device) {
	(void)device;

	this->_ready = true;

	this->_driver->reset();
	this->_driver->setBitrate(this->_bitrate, this->_clock);
	this->_driver->setNormalMode();
}

void Hardware::HBusCAN::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;
	if (!this->_ready) return;

	struct can_frame frame;
	MCP2515::ERROR result = this->_driver->readMessage(&frame);

	if (result == MCP2515::ERROR_NOMSG) return;
	if (result != MCP2515::ERROR_OK) {
		this->_log("Receive error: " + Hardware::HBusCAN::_statusRecognize(result));

		return;
	}

	//this->_log("frame: " + Hardware::HBusCAN::_statusRecognize(result) + " : " + String(frame.can_id, HEX));

	String frame_data = "";
	int i = 0;
	while (i < frame.can_dlc && i < 8) {
		frame_data += (char)frame.data[i];

		i++;
	}

	device->Signal(this->_id, "raw")->ValueKV(new KeyValuePair(
		String(frame.can_id, HEX),
		String(frame_data)
	));

	if (this->_parse) {
		unsigned short id = frame.can_id & 0x7FF;
		byte priority = (id >> 9) & 0x03;
		byte truncated = ((id >> 8) & 0x01);
		byte dst = (id >> 4) & 0x0F;
		byte src = id & 0x0F;

		if (this->Validate(src, dst, priority, truncated)) {
			if (dst != this->_address && dst != 0x0F) return;

			int i = 0;
			while (i < frame.can_dlc) {
				this->_buffers[src] += (char)frame.data[i];

				i++;
			}

			if (truncated == 0 || this->_buffers[src].length() > this->_bufferMax) {
				device->Signal(this->_id, "parsed")->ValueAny(
					new Hardware::HBusCANMessage(src, dst, priority, this->_buffers[src])
				);

				device->CommandRecognizeAndProcess(this->_buffers[src], this);

				this->_buffers[src] = "";
			}
		}
	}
}

void Hardware::HBusCAN::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}

bool Hardware::HBusCAN::HardwareBusSend (Protonix* device, String data) {
	(void)device;
	(void)data;

	return false;
}

bool Hardware::HBusCAN::HardwareBusCommand (Protonix* device, String command) {
	(void)device;
	(void)command;

	return false;
}

bool Hardware::HBusCAN::Validate (byte src, byte dst, byte priority, byte truncated) {
	if (src > 0x0F) return false;
	if (dst != this->_address && dst != 0x0F) return false;
	if (priority > 0x03) return false;
	if (truncated != 0 && truncated != 1) return false;

	return true;
}