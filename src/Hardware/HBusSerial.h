#pragma once

#include <Arduino.h>

#include <SoftwareSerial.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBusSerial : public IProtonixBus {
				private:
					bool _started;
					unsigned short _pinRX;
					unsigned short _pinTX;
					unsigned int _speed;
					unsigned int _timeout;
					bool _blocking;
					bool _observable;
					String _cmdBuffer;
					String _lenBuffer;
					bool _lenActive;
					SoftwareSerial* _port;

				public:
					HBusSerial (unsigned short pinRX, unsigned short pinTX, bool observable = true, unsigned int speed = 9600, unsigned int timeout = 1000, bool blocking = false);

					bool Started ();
					unsigned int PinRX ();
					unsigned int PinTX ();
					unsigned int Speed ();

					unsigned short Timeout ();
					HBusSerial* Timeout (unsigned short timeout);

					bool Blocking ();
					HBusSerial* Blocking (bool blocking);

					bool Observable ();
					HBusSerial* Observable (bool observable);

					SoftwareSerial* Port ();

					bool SendCommand (String command);
					bool Send (String s);
					bool Write (byte b);
					byte Read ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, String command);
					bool HardwareBusSend (Protonix* device, String data);
					bool HardwareBusCommand (Protonix* device, String command);
			};
		}
	}
}