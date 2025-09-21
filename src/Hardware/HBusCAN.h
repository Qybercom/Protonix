#pragma once

#include <Arduino.h>

#include <mcp2515.h>

#include "../Common/index.h"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			// https://github.com/autowp/arduino-mcp2515

			using Qybercom::List;

			struct HBusCANCommand {
				uint8_t cmd;
				uint8_t priority;

				HBusCANCommand (uint8_t c = 0, uint8_t p = 7) : cmd(c), priority(p & 0x07) { }
			};

			struct HBusCANBuffer {
				uint8_t cmd;
				uint8_t srcID;
				uint8_t priority;
				unsigned short expectedLen;
				unsigned short rxLen;
				uint8_t expectedSeq;
				unsigned int lastRxTime;
				uint8_t* buffer;

				HBusCANBuffer (uint8_t c, uint8_t s, uint8_t p, unsigned short len)
					: cmd(c), srcID(s), priority(p), expectedLen(len), rxLen(0),
					  expectedSeq(0), lastRxTime(0), buffer(nullptr)
				{
					if (expectedLen > 0)
						buffer = new uint8_t[expectedLen];
				}

				~HBusCANBuffer () {
					if (buffer)
						delete[] buffer;
				}
			};

			class HBusCANMessage {
				public:
					bool custom = false;
					uint8_t cmd = 0;
					uint8_t srcID = 0;
					uint8_t priority = 0;
					String payload;
					struct can_frame raw;

					HBusCANMessage () { }

					String toHexString (bool delim = false);
			};

			class HBusCAN : public IProtonixBus {
				private:
					bool _ready;
					unsigned short _pinCS;
					uint8_t _sourceID;
					unsigned int _timeout;
					CAN_SPEED _bitrate;
					CAN_CLOCK _clock;
					unsigned short _payloadMax;
					MCP2515* _driver;
					List<HBusCANCommand*> _commands;
					List<HBusCANBuffer*> _buffers;
					List<HBusCANMessage*> _input;
					List<struct can_frame*> _output;

					void _settings ();
					uint8_t _commandPriority (uint8_t cmd);
					HBusCANBuffer* _buffer (uint8_t cmd, uint8_t src);
					void _bufferRemove (HBusCANBuffer* buffer);
					void _inputRaw (const struct can_frame &frame);
					void _inputFull (HBusCANBuffer* buffer, const struct can_frame &lastFrame);
					void _inputFrame (const struct can_frame &frame, uint8_t cmd, uint8_t src, uint8_t priority);

				public:
					HBusCAN (unsigned short pinCS, uint8_t sourceID, unsigned int timeout = 1000, CAN_SPEED bitrate = CAN_500KBPS, CAN_CLOCK clock = MCP_8MHZ, unsigned short payloadMax = 1024);
					static HBusCAN* Init (unsigned short pinCS, uint8_t sourceID, unsigned int timeout = 1000, CAN_SPEED bitrate = CAN_500KBPS, CAN_CLOCK clock = MCP_8MHZ, unsigned short payloadMax = 1024);

					unsigned short PinCS ();

					uint8_t SourceID ();
					HBusCAN* SourceID (uint8_t sourceID);

					unsigned long Timeout ();
					HBusCAN* Timeout (unsigned long timeout);

					CAN_SPEED Bitrate ();
					HBusCAN* Bitrate (CAN_SPEED bitrate);

					CAN_CLOCK Clock ();
					HBusCAN* Clock (CAN_CLOCK clock);

					unsigned short PayloadMax ();
					HBusCAN* PayloadMax (unsigned short size);

					HBusCAN* FilterID (const unsigned int value);

					List<HBusCANCommand*> &Commands ();
					HBusCAN* CommandAdd (uint8_t cmd, uint8_t priority = 7);

					bool Send (struct can_frame* frame);
					bool Send (unsigned int id, const String &hexData);

					bool Enqueue (struct can_frame* frame);
					bool Enqueue (unsigned int cmd, const String &payload);

					bool HardwareSPI ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareCommand (Protonix* device, String command);

					static canid_t ID (uint8_t id, uint8_t cmd, uint8_t priority = 7);
					static can_frame* Frame (canid_t id, uint8_t* buf, unsigned short offset, unsigned short length, unsigned short remaining, uint8_t seq = 0, bool first = true, bool last = true);
					static String Status (MCP2515::ERROR status);
			};
		}
	}
}