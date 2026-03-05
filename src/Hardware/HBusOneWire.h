#pragma once

#include <Arduino.h>
#include <OneWire.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBusOneWire : public IProtonixBus {
				private:
					bool _init;
					OneWire* _port;

				public:
					HBusOneWire (unsigned short pin);

					bool Search ();
					bool Reset ();
					bool Select (String rom);
					bool Select (unsigned char* rom);
					unsigned char Read ();
					bool Write (unsigned char value);
					bool CRC8 (unsigned char* data, unsigned short length);
					bool ROMParse (String input, unsigned char* output);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);

					bool BusSend (Protonix* device, String data);
					bool BusCommand (Protonix* device, String command);
			};
		}
	}
}