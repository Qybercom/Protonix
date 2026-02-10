#pragma once

#include <Arduino.h>

#include <SoftwareSerial.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBusSerial : public IProtonixBus {
				private:
					bool _started;
					bool _lenActive;
					String _lenBuffer;
					String _cmdBuffer;
					SoftwareSerial* _port;

				public:
					HBusSerial (unsigned short pinRX, unsigned short pinTX);

					bool Started ();
					SoftwareSerial* Port ();

					bool SendCommand (String command);
					bool Send (String s);
					bool Write (char b);
					char Read ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);

					bool BusSend (Protonix* device, String data);
					bool BusCommand (Protonix* device, String command);
			};
		}
	}
}