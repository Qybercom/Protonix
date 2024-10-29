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
				unsigned int _pinRX;
				unsigned int _pinTX;
				unsigned int _speed;
				unsigned int _timeout;
				bool _started;
				bool _serial;
				bool _blocking;
				bool _observable;
				String _cmdBuffer;
				String _lenBuffer;
				bool _lenActive;
				IProtonixCommand* _cmds[7];
				//byte 
				void _init(bool serial, String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout, bool blocking, bool observable);
				byte _crc8(String data);

			public:
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX);
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, bool blocking);
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, bool blocking, bool observable);
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed);
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, bool blocking);
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, bool blocking, bool observable);
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout);
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout, bool blocking);
				ProtonixDevicePort(String name, unsigned int pinRX, unsigned int pinTX, unsigned int speed, unsigned int timeout, bool blocking, bool observable);
				ProtonixDevicePort();
				ProtonixDevicePort(bool blocking);
				ProtonixDevicePort(bool blocking, bool observable);
				ProtonixDevicePort(unsigned int speed);
				ProtonixDevicePort(unsigned int speed, bool blocking);
				ProtonixDevicePort(unsigned int speed, bool blocking, bool observable);
				ProtonixDevicePort(unsigned int speed, unsigned int timeout);
				ProtonixDevicePort(unsigned int speed, unsigned int timeout, bool blocking);
				ProtonixDevicePort(unsigned int speed, unsigned int timeout, bool blocking, bool observable);
				
				bool Started();
				bool Serial();
				
				unsigned int PinRX();
				unsigned int PinTX();
				unsigned int Speed();
				
				void Name(String name);
				String Name();
				
				void Timeout(unsigned short timeout);
				unsigned short Timeout();
				
				void Blocking(bool blocking);
				bool Blocking();
				
				void Observable(bool observable);
				bool Observable();

				void Init(ProtonixDevice* device);
				void Pipe(ProtonixDevice* device);
				bool Send(IProtonixCommand* command);
				
				byte Read();
				bool Write(byte b);
		};
	}
}