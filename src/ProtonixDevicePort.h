#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>

#include "IProtonixCommand.h"

namespace Qybercom {
	namespace Protonix {
		class IProtonixCommand;

		class ProtonixDevicePort {
			private:
				#if defined(ESP32) || defined(ESP8266)
				EspSoftwareSerial::UART _port;
				#else
				SoftwareSerial* _port;
				#endif
				String _name;
				unsigned short _pinTX;
				unsigned short _pinRX;
				unsigned int _speed;
				unsigned short _timeout;
				bool _serial;
				String _cmdBuffer;
				IProtonixCommand* _cmds[4];
				void _init(String name, unsigned short pinTX, unsigned short pinRX, unsigned int speed, unsigned short timeout);

			public:
				ProtonixDevicePort(String name, unsigned short pinTX, unsigned short pinRX);
				ProtonixDevicePort(String name, unsigned short pinTX, unsigned short pinRX, unsigned int speed);
				ProtonixDevicePort(String name, unsigned short pinTX, unsigned short pinRX, unsigned int speed, unsigned short timeout);
				ProtonixDevicePort(String name);

				void Name(String name);
				String Name();
				unsigned int PinTX();
				unsigned int PinRX();
				unsigned int Speed();
				void Timeout(unsigned short timeout);
				unsigned short Timeout();

				void Init(ProtonixDevice* device);
				void Pipe(ProtonixDevice* device);
				bool Send(IProtonixCommand* command);
		};
	}
}