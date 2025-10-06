#include <Arduino.h>

#include <mcp2515.h>

#include "../Common/index.h"

#include "../Protonix.h"
#include "../ProtonixTimer.h"

#include "HBusCAN.h"

using namespace Qybercom::Protonix;

void Hardware::HBusCAN::_settings () {
	if (!this->_ready) return;

	this->_driver->setBitrate(this->_bitrate, this->_clock);
	this->_driver->setNormalMode();
}

struct can_frame Hardware::HBusCAN::_frame (unsigned short id, String data) {
	int length = data.length();
	if (length > 8) length = 8;

	struct can_frame frame;

	frame.can_id = id;
	frame.can_dlc = length;

	int i = 0;
	while (i < length) {
		frame.data[i] = (byte)data[i];

		i++;
	}

	return frame;
}

void Hardware::HBusCAN::_process (Protonix* device, byte src, byte dst, byte priority, String data) {
	device->Signal(this->_id, "parsed")->Value(
		Hardware::HBusCANMessage(src, dst, priority, data)
	);

	device->CommandRecognizeAndProcess(data, this);
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
	while (i < 15) {
		this->_inBuffer[i] = nullptr;

		i++;
	}

	this->_bufferMax = bufferMax;

	this->_outTimer = new ProtonixTimer(1);
}

Hardware::HBusCAN* Hardware::HBusCAN::Init (unsigned short pinCS, byte address, CAN_SPEED bitrate, CAN_CLOCK clock, bool parse, unsigned int bufferMax) {
	return new Hardware::HBusCAN(pinCS, address, bitrate, clock, parse, bufferMax);
}

ProtonixTimer* Hardware::HBusCAN::OutTimer () {
	return this->_outTimer;
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
	if (!this->_ready) return false;

	MCP2515::ERROR result = this->_driver->sendMessage(frame);
	bool out = true;

	if (result != MCP2515::ERROR_OK) {
		out = false;

		this->_log("Send error: " + Hardware::HBusCAN::_statusRecognize(result));
	}

	return out;
}

bool Hardware::HBusCAN::Send (unsigned short id, String data) {
	struct can_frame frame = this->_frame(id, data);

	return this->Send(&frame);
}

bool Hardware::HBusCAN::Send (String data, byte priority, byte address, bool truncate) {
	if (priority > 3) priority = 3;
	int length = data.length();

	if (length <= 8)
		return this->Send(this->ID(priority, address, false), data);

	List<String>* chunks = strChunks(data, 7);
	int i = 0;
	int c = chunks->Count();

	if (!truncate && c > 10) {
		this->_log("Can not send data with length greater than 70, current: " + String(length) + " (" + String(c) + " chunks)");

		return false;
	}

	for (String& chunk : *chunks) {
		this->_outBuffer.Add(this->_frame(
			this->ID(priority, address, i != (c - 1)),
			String(i) + chunk
		));

		i++;
	}

	delete chunks;
	chunks = nullptr;

	return true;
}

String Hardware::HBusCAN::HardwareSummary () {
	return "CAN bus";
}

void Hardware::HBusCAN::HardwareInitPre (Protonix* device) {
	(void)device;
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

	if (result != MCP2515::ERROR_OK) {
		if (result == MCP2515::ERROR_NOMSG) { }
		else this->_log("Receive error: " + Hardware::HBusCAN::_statusRecognize(result));
	}
	else {
		//this->_log("Raw frame: " + Hardware::HBusCAN::_statusRecognize(result) + " : " + String(frame.can_id, HEX));

		device->Signal(this->_id, "raw")->Value(frame);

		if (this->_parse) {
			unsigned short id = frame.can_id & 0x7FF;
			byte priority = (id >> 9) & 0x03;
			byte truncated = ((id >> 8) & 0x01);
			byte dst = (id >> 4) & 0x0F;
			byte src = id & 0x0F;

			if (this->Validate(src, dst, priority, truncated)) {
				if (dst != this->_address && dst != 0x0F) return;

				String raw = "";
				int i = 0;
				while (i < frame.can_dlc) {
					raw += (char)frame.data[i];

					i++;
				}

				bool empty = this->_inBuffer[src] == nullptr;

				if (truncated == 0 && empty) this->_process(device, src, dst, priority, raw);
				else {
					if (empty)
						this->_inBuffer[src] = new List<String>();

					if (truncated != 0) this->_inBuffer[src]->Add(raw);
					else {
						unsigned int size = this->_inBuffer[src]->Count();
						String* out = new String[size];

						for (String &chunk : *this->_inBuffer[src])
							out[(int)(chunk[0] - '0')] = chunk.substring(1);

						this->_inBuffer[src]->Clear();
						delete this->_inBuffer[src];
						this->_inBuffer[src] = nullptr;

						String data = "";
						unsigned int i = 0;
						while (i < size) {
							data += out[i];

							i++;
						}

						delete[] out;

						data += raw.substring(1);
						this->_process(device, src, dst, priority, data);
					}
				}
			}
		}
	}

	unsigned int queued = this->_outBuffer.Count();

	if (this->_outTimer->Pipe() && queued != 0) {
		//this->_log("Queued: " + String(queued));

		struct can_frame frame = this->_outBuffer.PopFirst();

		this->Send(&frame);
	}
}

void Hardware::HBusCAN::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HBusCAN::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}

bool Hardware::HBusCAN::BusSend (Protonix* device, String data) {
	(void)device;

	return this->Send(data);
}

bool Hardware::HBusCAN::BusCommand (Protonix* device, String command) {
	(void)device;

	return this->Send(command);
}

bool Hardware::HBusCAN::Validate (byte src, byte dst, byte priority, byte truncated) {
	if (src > 0x0F) return false;
	if (dst != this->_address && dst != 0x0F) return false;
	if (priority > 0x03) return false;
	if (truncated != 0 && truncated != 1) return false;

	return true;
}