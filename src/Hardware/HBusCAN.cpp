#include <Arduino.h>

#include <mcp2515.h>

#include "../Common/index.h"

#include "HBusCAN.h"

using namespace Qybercom::Protonix;

String Hardware::HBusCANMessage::toHexString (bool delim) {
	String s = "";
	int i = 0;

	while (i < raw.can_dlc) {
		uint8_t b = raw.data[i];
		if (b < 16) s += '0';

		s += String(b, HEX);

		if (delim && i + 1 < raw.can_dlc) s += ' ';

		i++;
	}

	return s;
}

void Hardware::HBusCAN::_settings () {
	if (!this->_ready) return;

	this->_driver->setBitrate(this->_bitrate, this->_clock);
	this->_driver->setNormalMode();
}

uint8_t Hardware::HBusCAN::_commandPriority (uint8_t cmd) {
	for (Hardware::HBusCANCommand* command : this->_commands)
		if (command->cmd == cmd)
			return command->priority;

	return 7;
}

Hardware::HBusCANBuffer* Hardware::HBusCAN::_buffer (uint8_t cmd, uint8_t src) {
	for (Hardware::HBusCANBuffer* buffer : this->_buffers)
		if (buffer->cmd == cmd) return buffer;

	return nullptr;
}

void Hardware::HBusCAN::_bufferRemove (HBusCANBuffer* buffer) {
	if (buffer == nullptr) return;

	this->_buffers.Remove(buffer);
	delete buffer; // maybe obsolete
}

void Hardware::HBusCAN::_inputRaw (const struct can_frame &frame) {
	Hardware::HBusCANMessage* message = new Hardware::HBusCANMessage();

	message->custom = false;
	message->raw = frame;

	this->_input.Add(message);
}

void Hardware::HBusCAN::_inputFull (HBusCANBuffer* buffer, const struct can_frame &lastFrame) {
	if (buffer == nullptr) return;

	Hardware::HBusCANMessage* message = new Hardware::HBusCANMessage();

	message->custom = true;
	message->cmd = buffer->cmd;
	message->srcID = buffer->srcID;
	message->priority = buffer->priority;
	message->raw = lastFrame;

	message->payload.reserve(buffer->rxLen);
	message->payload = "";

	int i = 0;
	while (i < buffer->rxLen) {
		message->payload += (char)buffer->buffer[i];

		i++;
	}

	this->_bufferRemove(buffer);
	this->_input.Add(message);
}

void Hardware::HBusCAN::_inputFrame (const struct can_frame &frame, uint8_t cmd, uint8_t src, uint8_t priority) {
	//if (frame == nullptr) return;

	Hardware::HBusCANMessage* message = new Hardware::HBusCANMessage();

	message->custom = true;
	message->cmd = cmd;
	message->srcID = src;
	message->priority = priority;
	message->raw = frame;
	message->payload = "";

	int i = 0;
	while (i < frame.can_dlc) {
		message->payload += (char)frame.data[i];

		i++;
	}

	this->_input.Add(message);
}

Hardware::HBusCAN::HBusCAN (unsigned short pinCS, uint8_t sourceID, unsigned int timeout, CAN_SPEED bitrate, CAN_CLOCK clock, unsigned short payloadMax) {
	this->_ready = false;
	this->_pinCS = pinCS;
	this->_sourceID = sourceID;
	this->_timeout = timeout;
	this->_bitrate = bitrate;
	this->_clock = clock;
	this->_payloadMax = payloadMax;

	this->_driver = new MCP2515(this->_pinCS);
}

Hardware::HBusCAN* Hardware::HBusCAN::Init (unsigned short pinCS, uint8_t sourceID, unsigned int timeout, CAN_SPEED bitrate, CAN_CLOCK clock, unsigned short payloadMax) {
	return new Hardware::HBusCAN(pinCS, sourceID, timeout, bitrate, clock, payloadMax);
}

unsigned short Hardware::HBusCAN::PinCS () {
	return this->_pinCS;
}

uint8_t Hardware::HBusCAN::SourceID () {
	return this->_sourceID;
}

Hardware::HBusCAN* Hardware::HBusCAN::SourceID (uint8_t sourceID) {
	this->_sourceID = sourceID;

	return this;
}

unsigned long Hardware::HBusCAN::Timeout () {
	return this->_timeout;
}

Hardware::HBusCAN* Hardware::HBusCAN::Timeout (unsigned long timeout) {
	this->_timeout = timeout;

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


unsigned short Hardware::HBusCAN::PayloadMax () {
	return this->_payloadMax;
}

Hardware::HBusCAN* Hardware::HBusCAN::PayloadMax (unsigned short size) {
	this->_payloadMax = size;

	return this;
}

Hardware::HBusCAN* Hardware::HBusCAN::FilterID (const unsigned int value) {
	this->_driver->setFilterMask(MCP2515::MASK::MASK0, false, 0x7FF);
	this->_driver->setFilter(MCP2515::RXF::RXF0, false, value);

	this->_driver->setNormalMode();

	return this;
}

Qybercom::List<Hardware::HBusCANCommand*> &Hardware::HBusCAN::Commands () {
	return this->_commands;
}

Hardware::HBusCAN* Hardware::HBusCAN::CommandAdd (uint8_t cmd, uint8_t priority) {
	this->_commands.Add(new Hardware::HBusCANCommand(cmd, priority));

	return this;
}

bool Hardware::HBusCAN::Send (struct can_frame* frame) {
	return this->_ready && this->_driver->sendMessage(frame) == MCP2515::ERROR::ERROR_OK;
}

bool Hardware::HBusCAN::Send (unsigned int id, const String &hexData) {
	if (!this->_ready) return false;

	Qybercom::List<uint8_t> bytes = hexToBytes(hexData);
	struct can_frame frame;

	frame.can_id = id;
	frame.can_dlc = bytes.Count();

	if (frame.can_dlc >= 9) return false; // TODO: seek for support more than 8 bytes in CAN data

	unsigned short i = 0;

	for (uint8_t b : bytes) {
		frame.data[i] = b;

		i++;
	}

	return this->Send(&frame);
}

bool Hardware::HBusCAN::Enqueue (struct can_frame* frame) {
	if (!this->_ready) return false;

	this->_output.Add(frame);

	return true;
}

bool Hardware::HBusCAN::Enqueue (unsigned int cmd, const String &payload) {
	long length = payload.length();
	if (length > this->_payloadMax) return false;

	uint8_t priority = this->_commandPriority(cmd);
	uint8_t* buf = strBuf(payload);
	canid_t id = ID(this->_sourceID, cmd, priority);

	if (length <= this->_payloadMax && (length == 0 || (buf[0] & 0x80) == 0)) {
        this->_output.Add(Frame(id, buf, 0, length, length));
    }
    else {
        unsigned short remaining = length;
        unsigned short offset = 0;
        uint8_t seq = 0;

        while (remaining > 0) {
            uint8_t limit = (offset == 0) ? 5 : 7;
            uint8_t chunk = (remaining > limit) ? limit : remaining;
            bool first = (offset == 0);
            bool last = (remaining <= limit);

            this->_output.Add(Frame(id, buf, offset, length, remaining, seq, first, last));

            remaining -= chunk;
            offset += chunk;
            seq++;
        }
    }

	if (buf) {
		delete[] buf;
		buf = nullptr;
	}

	return true;
}

bool Hardware::HBusCAN::HardwareSPI () {
	return true;
}

void Hardware::HBusCAN::HardwareInitPre (Protonix* device) {
	(void)device;
}

void Hardware::HBusCAN::HardwareInitPost (Protonix* device) {
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
	if (this->_driver->readMessage(&frame) == MCP2515::ERROR::ERROR_OK) {
		// unpack ID: (prio<<8) | (src<<4) | cmd  (11-bit ID)
		uint8_t prio = (frame.can_id >> 8) & 0x07;
		uint8_t src = (frame.can_id >> 4) & 0x0F;
		uint8_t cmd = frame.can_id & 0x0F;
		Hardware::HBusCANBuffer* buf = this->_buffer(cmd, src);

		bool handledAsMine = false;

		// CASE short frame candidate (no start flag in first byte)
		if (frame.can_dlc <= this->_payloadMax) {
			bool firstByteHasStart = (frame.can_dlc > 0) && (frame.data[0] & 0x80);

			// consider "short" only if first byte MSB not set
			if (!firstByteHasStart) {
				// if command registered or there's an active buffer for this cmd+src => treat as our short
				bool cmdRegistered = false;
				for (Hardware::HBusCANCommand* command : this->_commands)
					if (command->cmd == cmd)
						cmdRegistered = true;

				if (cmdRegistered || buf != nullptr) {
					this->_inputFrame(frame, cmd, src, prio);

					handledAsMine = true;
				}
			}
		}

		// CASE fragment / start
		if (!handledAsMine) {
			uint8_t flags = frame.data[0];
			bool start = flags & 0x80;
			bool end = flags & 0x40;
			uint8_t seq = flags & 0x3F;

			Hardware::HBusCANBuffer* buf = this->_buffer(cmd, src);

			if (start) {
				// read length from bytes 1..2 if present
				unsigned short msgLen = 0;

				if (frame.can_dlc < 3) msgLen = 255; // fallback
				else {
					msgLen = ((unsigned short)frame.data[1] << 8) | (unsigned short)frame.data[2];

					if (msgLen == 0) msgLen = 1;
				}

				if (msgLen > this->_payloadMax)
					msgLen = this->_payloadMax; // limit

				if (buf != nullptr)
					this->_bufferRemove(buf); // drop existing partial

				Hardware::HBusCANBuffer* nb = new Hardware::HBusCANBuffer(cmd, src, prio, msgLen);

				// copy bytes starting from index 3
				short i = 3;
				while (i < frame.can_dlc) {
					if (nb->rxLen < nb->expectedLen)
						nb->buffer[nb->rxLen++] = frame.data[i];

					i++;
				}

				nb->expectedSeq = 1; // next expected seq
				nb->lastRxTime = millis();

				this->_buffers.Add(nb);
				handledAsMine = true;

				if (end && nb->rxLen >= nb->expectedLen)
					this->_inputFull(nb, frame);
			}
			else if (buf != nullptr) {
				// continuation fragment for existing buffer
				unsigned int now = millis();

				if ((now - buf->lastRxTime) > this->_timeout) {
					// timed out — drop
					this->_bufferRemove(buf);

					buf = nullptr;
				}
			}

			// if buf exists now (maybe created above), try to append continuation
			buf = this->_buffer(cmd, src);
			if (!handledAsMine && buf != nullptr) {
				uint8_t flags2 = frame.data[0];
				uint8_t seq2 = flags2 & 0x3F;

				// seq mismatch -> drop buffer
				if (seq2 != buf->expectedSeq) this->_bufferRemove(buf);
				else {
					// append bytes from index 1 .. dlc-1
					short i = 1;
					while (i < frame.can_dlc) {
						if (buf->rxLen < buf->expectedLen)
							buf->buffer[buf->rxLen++] = frame.data[i];

						i++;
					}

					buf->expectedSeq++;
					buf->lastRxTime = millis();
					handledAsMine = true;

					// end
					if (flags2 & 0x40)
						this->_inputFull(buf, frame);
				}
			}
		}

		// If still not handled as ours — push as RAW
		if (!handledAsMine)
			this->_inputRaw(frame);
	}

	if (this->_output.Count() > 0) {
		struct can_frame* f = this->_output.PopFirst();
		MCP2515::ERROR status = this->_driver->sendMessage(f);

		this->_log("Send status: '" + Status(status) + "'");

		if (status == MCP2515::ERROR::ERROR_OK) {
		}

		delete f;
		f = nullptr;
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

canid_t Hardware::HBusCAN::ID (uint8_t id, uint8_t cmd, uint8_t priority) {
	 return ((unsigned short) priority << 8) | ((unsigned short) (id & 0x0F) << 4) | (cmd & 0x0F);
}

can_frame* Hardware::HBusCAN::Frame (canid_t id, uint8_t* buf, unsigned short offset, unsigned short length, unsigned short remaining, uint8_t seq, bool first, bool last) {
	struct can_frame* frame = new struct can_frame();

	uint8_t i = 0;
	while (i < 8) {
		frame->data[i] = 0;

		i++;
	}

	frame->can_id = id;

	if (first && last) {
		frame->can_dlc = length;

		i = 0;
		while (i < length) {
			frame->data[i] = buf[i];

			i++;
		}

		return frame;
	}

	uint8_t limit = first ? 5 : 7;
	uint8_t offs  = first ? 3 : 1;
	uint8_t flags = first ? ((seq & 0x3F) | 0x80) : (seq & 0x3F);

	if (first) {
		frame->data[1] = (length >> 8) & 0xFF;
		frame->data[2] = length & 0xFF;
	}

	uint8_t chunk = (remaining > limit) ? limit : remaining;
	frame->can_dlc = chunk + offs;

	if (last) flags |= 0x40; // end
	frame->data[0] = flags;

	i = 0;
	while (i < chunk) {
		frame->data[offs + i] = buf[offset + i];

		i++;
	}

	return frame;
}

String Hardware::HBusCAN::Status (MCP2515::ERROR status) {
	switch (status) {
		case MCP2515::ERROR::ERROR_OK: return "ERROR_OK"; break;
		case MCP2515::ERROR::ERROR_FAIL: return "ERROR_FAIL"; break;
		case MCP2515::ERROR::ERROR_ALLTXBUSY: return "ERROR_ALLTXBUSY"; break;
		case MCP2515::ERROR::ERROR_FAILINIT: return "ERROR_FAILINIT"; break;
		case MCP2515::ERROR::ERROR_FAILTX: return "ERROR_FAILTX"; break;
		case MCP2515::ERROR::ERROR_NOMSG: return "ERROR_NOMSG"; break;
		default: return "__unknown__"; break;
	}
}