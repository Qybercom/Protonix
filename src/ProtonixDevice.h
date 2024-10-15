#pragma once

#include <Arduino.h>

#if defined(ESP32) || defined(ESP8266)
#include <StreamString.h>
#endif

#include "IProtonixHardware.h"
#include "IProtonixNetwork.h"
#include "IProtonixProtocol.h"
#include "IProtonixDevice.h"
#include "IProtonixDTO.h"
#include "ProtonixURI.h"
#include "ProtonixDTO.h"
#include "ProtonixTimer.h"
#include "ProtonixAction.h"
#include "ProtonixMemory.h"
#include "ProtonixRegistry.h"
#include "ProtonixDeviceStatus.h"
#include "ProtonixDevicePort.h"
#include "ProtonixDeviceSensor.h"

#if defined(ESP32) || defined(ESP8266)
#define PROTONIX_LIMIT_HARDWARE 32
#define PROTONIX_LIMIT_ACTION_LIST 64
#define PROTONIX_LIMIT_ACTION_BACKLOG 256
#define PROTONIX_LIMIT_PORT 8
#else
#define PROTONIX_LIMIT_HARDWARE 8
#define PROTONIX_LIMIT_ACTION_LIST 8
#define PROTONIX_LIMIT_ACTION_BACKLOG 16
#define PROTONIX_LIMIT_PORT 4
#endif

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
                ProtonixMemory* _memory;
                ProtonixRegistry* _registry;
                String _serverBaseURI;

				unsigned int _hardwareCount;
				IProtonixHardware* _hardware[PROTONIX_LIMIT_HARDWARE];

				unsigned int _portCount;
				ProtonixDevicePort* _ports[PROTONIX_LIMIT_PORT];

				ProtonixAction* _actionList[PROTONIX_LIMIT_ACTION_LIST];
				String _actionBacklog[PROTONIX_LIMIT_ACTION_BACKLOG];
				int _actionCursorList;
				int _actionCursorBacklog;
				int _actionCursorCurrent;
				void _pipeActions();

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
				
				void _pipeNetwork();
				void _onStreamURL();
				void _onStreamResponse();
				void _onStreamEvent();

                //void _updateError(String step, StreamString &error);
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
                ProtonixMemory* Memory();
                ProtonixRegistry* Registry();
				static int FreeRAM();
                static int FreeFlash();
				static void Reboot();

				IProtonixHardware* Hardware(String id);
				ProtonixDevice* Hardware(String id, IProtonixHardware* hardware);

				ProtonixDevicePort* Port(String name);
				ProtonixDevicePort* Port(ProtonixDevicePort* port);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX, bool blocking);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX, bool blocking, bool observable);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, bool blocking);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, bool blocking, bool observable);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout, bool blocking);
				ProtonixDevicePort* Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout, bool blocking, bool observable);
				ProtonixDevicePort* PortDefault();
				ProtonixDevicePort* PortDefault(bool blocking);
				ProtonixDevicePort* PortDefault(bool blocking, bool observable);
				ProtonixDevicePort* PortDefault(unsigned int speed);
				ProtonixDevicePort* PortDefault(unsigned int speed, bool blocking);
				ProtonixDevicePort* PortDefault(unsigned int speed, bool blocking, bool observable);
				ProtonixDevicePort* PortDefault(unsigned int speed, unsigned int timeout);
				ProtonixDevicePort* PortDefault(unsigned int speed, unsigned int timeout, bool blocking);
				ProtonixDevicePort* PortDefault(unsigned int speed, unsigned int timeout, bool blocking, bool observable);
				
				// https://stackoverflow.com/a/120916/2097055
				ProtonixAction* Action(String name);
				bool ActionRegister(ProtonixAction* action);
				bool ActionRegister(String name);
				bool ActionRegister(String name, unsigned int interval);
				bool ActionRegister(String name, unsigned int interval, int stepEnd);
				bool ActionRegister(String name, unsigned int interval, int stepBegin, int stepEnd);
				bool ActionRegister(String name, unsigned int interval, int stepBegin, int stepEnd, int step);
				bool ActionTrigger(String name);
				void ActionReset();

				void Pipe();

				void OnSerial(ProtonixDevicePort* port, IProtonixCommand* command);
				bool SerialCommand(String port, IProtonixCommand* command);
				bool SerialCommandCustom(String port, String cmd);
				bool SerialCommandSensor(String port, ProtonixDeviceSensor* sensor);
				bool SerialCommandSensor(String port, String id, String value);
				bool SerialCommandSensor(String port, String id, String value, bool active);
				bool SerialCommandSensor(String port, String id, String value, bool active, bool failure);
				bool SerialCommandSensor(String port, String id, String value, bool active, bool failure, String state);
				bool SerialCommandSensor(String port, String id, bool active);
				bool SerialCommandSensor(String port, String id, bool active, bool failure);
				bool SerialCommandSensor(String port, String id, bool active, bool failure, String state);
				bool SerialCommandSensor(String port, String id, String value, String state);
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
                void ServerBaseURI(String uri);
				bool Connected();
				void OnStream(unsigned char* data);
				void RequestStream(String url, IProtonixDTORequest* request);
				void RequestStreamAuthorize();
				ProtonixDTO* DTOInput();
				ProtonixDTO* DTOOutput();
                bool FirmwareUpdateOTA();
                //bool FirmwareUpdateOTA(void(*onProgress)(int, int) = nullptr);
                //bool FirmwareUpdate(String firmware, void(*onProgress)(int, int) = nullptr);
				//bool FirmwareUpdate(Stream& stream, void(*onProgress)(int, int) = nullptr);
				#endif
		};
	}
}