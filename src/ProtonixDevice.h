#pragma once

#include <Arduino.h>

#include "IProtonixNetwork.h"
#include "IProtonixProtocol.h"
#include "IProtonixDevice.h"
#include "IProtonixDTO.h"
#include "ProtonixURI.h"
#include "ProtonixDTO.h"
#include "ProtonixTimer.h"
#include "ProtonixDeviceStatus.h"
#include "ProtonixDevicePort.h"

namespace Qybercom {
	namespace Protonix {
		class IProtonixDevice;

		// http://tedfelix.com/software/c++-callbacks.html
		class ProtonixDevice {
			private:
				IProtonixDevice* _device;
				ProtonixTimer* _timer;
				ProtonixDeviceStatus* _status;
				bool _ready;
				bool _debug;

				unsigned int _portCount;
				ProtonixDevicePort* _ports[4];

				#if defined(ESP32) || defined(ESP8266)
				IProtonixNetwork* _network;
				bool _networkConnected1;
				bool _networkConnected2;
				IProtonixProtocol* _protocol;
				bool _protocolConnected1;
				bool _protocolConnected2;
				ProtonixURI* _uri;
				ProtonixDTO* _dtoInput;
				ProtonixDTO* _dtoOutput;
				
				void _onStreamURL();
				void _onStreamResponse();
				void _onStreamEvent();
				#endif

			public:
				ProtonixDevice(IProtonixDevice* device);
				
				void Device(IProtonixDevice* device);
				IProtonixDevice* Device();
				ProtonixTimer* Timer();
				ProtonixDeviceStatus* Status();
				void Summary(String additional);
				bool Ready();
				unsigned int Tick();
				void Debug(bool debug);
				bool Debug();
				int FreeRAM();

				ProtonixDevice* Port(ProtonixDevicePort* port);
				ProtonixDevice* Port(String name, unsigned short pinTX, unsigned short pinRX);
				ProtonixDevice* Port(String name, unsigned short pinTX, unsigned short pinRX, unsigned int speed);

				void Pipe();

				void OnSerial(ProtonixDevicePort* port, IProtonixCommand* command);
				bool SerialCommand(String port, IProtonixCommand* command);
				bool SerialStatus(String port);

				#if defined(ESP32) || defined(ESP8266)
				void Network(IProtonixNetwork* network);
				IProtonixNetwork* Network();
				void Protocol(IProtonixProtocol* protocol);
				IProtonixProtocol* Protocol();
				void Server(ProtonixURI* uri);
				ProtonixURI* Server();
				void ServerEndpoint(String host, unsigned int port);
				void ServerEndpoint(String host, unsigned int port, String path);
				bool Connected();
				void OnStream(unsigned char* data);
				void RequestStream(String url, IProtonixDTORequest* request);
				void RequestStreamAuthorize();
				ProtonixDTO* DTOInput();
				ProtonixDTO* DTOOutput();
				#endif
		};
	}
}