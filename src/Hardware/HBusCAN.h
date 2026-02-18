#pragma once

#include <Arduino.h>

#include <mcp2515.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"
#include "../ProtonixTimer.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			// https://github.com/autowp/arduino-mcp2515

			struct HBusCANMessage {
				char From;
				char To;
				char Priority;
				String Data;

				HBusCANMessage (
					char from = 0xf,
					char to = 0xf,
					char priority = 0x03,
					String data = ""
				):
					From(from),
					To(to),
					Priority(priority),
					Data(data) { }
			};

			class HBusCAN : public IProtonixBus {
				private:
					bool _ready;
					bool _parse;
					char _address;
					CAN_SPEED _bitrate;
					CAN_CLOCK _clock;
					MCP2515* _driver;
					unsigned int _bufferMax;
					List<String>* _inBuffer[15];
					List<struct can_frame> _outBuffer;
					ProtonixTimer* _outTimer;

					void _settings ();
					struct can_frame _frame (unsigned short id, String data);
					void _process (Protonix* device, char src, char dst, char priority, String data);
					static String _statusRecognize (MCP2515::ERROR code);

				public:
					HBusCAN (unsigned short pinCS, char address, CAN_SPEED bitrate = CAN_125KBPS, CAN_CLOCK clock = MCP_16MHZ, bool parse = true, unsigned int bufferMax = 70);
					static HBusCAN* Init (unsigned short pinCS, char address, CAN_SPEED bitrate = CAN_125KBPS, CAN_CLOCK clock = MCP_16MHZ, bool parse = true, unsigned int bufferMax = 70);

					ProtonixTimer* OutTimer ();

					char Address ();
					HBusCAN* Address (char address);

					CAN_SPEED Bitrate ();
					HBusCAN* Bitrate (CAN_SPEED bitrate);

					CAN_CLOCK Clock ();
					HBusCAN* Clock (CAN_CLOCK clock);

					bool Parse ();
					HBusCAN* Parse (bool parse);

					unsigned int BufferMax ();
					HBusCAN* BufferMax (unsigned int size);

					// Format: [priority:2][truncated:1][destination:4][source:4]
					unsigned short ID (char priority, char address = 0x03, bool truncated = false);

					bool Validate (char src, char dst, char priority, char truncated);

					bool Send (struct can_frame* frame);
					bool Send (unsigned short id, String data);
					bool Send (String data, char priority = 0x03, char address = 0x0F, bool truncate = false);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					bool HardwareSPI ();
					void HardwareSPIPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, const ProtonixCommand &command);

					bool BusSend (Protonix* device, String data);
					bool BusCommand (Protonix* device, String command);
			};
		}
	}
}