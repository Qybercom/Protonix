#pragma once

#include <Arduino.h>

#include <mcp2515.h>

#include "../Common/index.h"

#include "../IProtonixHardware.h"
#include "../Protonix.h"
#include "../ProtonixTimer.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			// https://github.com/autowp/arduino-mcp2515

			struct HBusCANMessage {
				byte From;
				byte To;
				byte Priority;
				String Data;

				HBusCANMessage (
					byte from = 0xf,
					byte to = 0xf,
					byte priority = 0x03,
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
					byte _address;
					CAN_SPEED _bitrate;
					CAN_CLOCK _clock;
					MCP2515* _driver;
					unsigned int _bufferMax;
					List<String>* _inBuffer[15];
					List<struct can_frame> _outBuffer;
					ProtonixTimer* _outTimer;

					void _settings ();
					struct can_frame _frame (unsigned short id, String data);
					void _process (Protonix* device, byte src, byte dst, byte priority, String data);
					static String _statusRecognize (MCP2515::ERROR code);

				public:
					HBusCAN (unsigned short pinCS, byte address, CAN_SPEED bitrate = CAN_125KBPS, CAN_CLOCK clock = MCP_16MHZ, bool parse = true, unsigned int bufferMax = 70);
					static HBusCAN* Init (unsigned short pinCS, byte address, CAN_SPEED bitrate = CAN_125KBPS, CAN_CLOCK clock = MCP_16MHZ, bool parse = true, unsigned int bufferMax = 70);

					ProtonixTimer* OutTimer ();

					byte Address ();
					HBusCAN* Address (byte address);

					CAN_SPEED Bitrate ();
					HBusCAN* Bitrate (CAN_SPEED bitrate);

					CAN_CLOCK Clock ();
					HBusCAN* Clock (CAN_CLOCK clock);

					bool Parse ();
					HBusCAN* Parse (bool parse);

					unsigned int BufferMax ();
					HBusCAN* BufferMax (unsigned int size);

					// Format: [priority:2][truncated:1][destination:4][source:4]
					unsigned short ID (byte priority, byte address = 0x03, bool truncated = false);

					bool Validate (byte src, byte dst, byte priority, byte truncated);

					bool Send (struct can_frame* frame);
					bool Send (unsigned short id, String data);
					bool Send (String data, byte priority = 0x03, byte address = 0x0F, bool truncate = false);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					bool HardwareSPI ();
					void HardwareSPIPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, String command);
					bool HardwareBusSend (Protonix* device, String data);
					bool HardwareBusCommand (Protonix* device, String command);
			};
		}
	}
}