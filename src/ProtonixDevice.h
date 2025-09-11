#pragma once

#include <Arduino.h>

#if defined(ESP32) || defined(ESP8266)
#include <StreamString.h>
#endif

#if defined(ESP32) || defined(ESP8266)
#define ISR_ATTR IRAM_ATTR
#else
#define ISR_ATTR
#endif

#include "_directives.h"

#include "Common/List.hpp"

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

namespace Qybercom {
	namespace Protonix {
		class IProtonixDevice;
		class IProtonixHardware;

		// http://tedfelix.com/software/c++-callbacks.html
		class ProtonixDevice {
			private:
				IProtonixDevice* _device;
				ProtonixTimer* _timerUptime;
				ProtonixTimer* _timerTick;
				#if defined(ESP32) || defined(ESP8266)
				ProtonixTimer* _timerNetwork;
				ProtonixTimer* _timerNetworkAuthorize;
				ProtonixTimer* _timerNetworkStatus;
				#endif
				ProtonixDeviceStatus* _status;
				bool _ready;
				bool _debug;
				ProtonixMemory* _memory;
				ProtonixRegistry* _registry;
				String _serverBaseURI;

				List<IProtonixHardware*> _hardware;

				List<ProtonixDevicePort*> _ports;

				List<ProtonixAction*> _actions;
				List<String> _actionQueue;
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
				bool _authorized;

				void _pipeNetwork();
				void _onStreamURL();
				void _onStreamResponse();
				void _onStreamEvent();
				#endif

				#if defined(ESP32)
				TaskHandle_t _dedicatedHandleCore0 = nullptr;
				TaskHandle_t _dedicatedHandleCore1 = nullptr;

				void _dedicateTask (TaskHandle_t* handle, unsigned short core, unsigned short priority = 1);
				static void _dedicatedTask (void* param);
				#endif

				void _interruptProcess ();
				static void ISR_ATTR _interrupt ();

				static List<ProtonixDevice*> _instances;

			public:
				ProtonixDevice (IProtonixDevice* device);

				void Device (IProtonixDevice* device);
				IProtonixDevice* Device ();

				void InterruptAttach (unsigned short pin, int mode);
				void InterruptDetach (unsigned short pin);

				ProtonixTimer* TimerUptime ();
				ProtonixTimer* TimerTick ();
				#if defined(ESP32) || defined(ESP8266)
				ProtonixTimer* TimerNetwork ();
				ProtonixTimer* TimerNetworkAuthorize ();
				ProtonixTimer* TimerNetworkStatus ();
				#endif
				ProtonixDeviceStatus* Status ();
				void Summary (String additional);
				bool Ready ();
				unsigned int Tick ();
				void Debug (bool debug);
				bool Debug ();
				ProtonixMemory* Memory ();
				ProtonixRegistry* Registry ();
				static int FreeRAM ();
				static int FreeFlash ();
				static void Reboot ();

				List<IProtonixHardware*> &Hardware ();
				IProtonixHardware* Hardware (String id);
				ProtonixDevice* Hardware (String id, IProtonixHardware* hardware);
				template<typename T>
				T* Hardware (String id) {
					IProtonixHardware* base = this->Hardware(id);

					return static_cast<T*>(base);
				}

				List<ProtonixDevicePort*> &Ports ();
				ProtonixDevicePort* Port (String name);
				ProtonixDevicePort* Port (ProtonixDevicePort* port);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX, bool blocking);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX, bool blocking, bool observable);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, bool blocking);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, bool blocking, bool observable);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout, bool blocking);
				ProtonixDevicePort* Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout, bool blocking, bool observable);
				ProtonixDevicePort* PortDefault ();
				ProtonixDevicePort* PortDefault (bool blocking);
				ProtonixDevicePort* PortDefault (bool blocking, bool observable);
				ProtonixDevicePort* PortDefault (unsigned int speed);
				ProtonixDevicePort* PortDefault (unsigned int speed, bool blocking);
				ProtonixDevicePort* PortDefault (unsigned int speed, bool blocking, bool observable);
				ProtonixDevicePort* PortDefault (unsigned int speed, unsigned int timeout);
				ProtonixDevicePort* PortDefault (unsigned int speed, unsigned int timeout, bool blocking);
				ProtonixDevicePort* PortDefault (unsigned int speed, unsigned int timeout, bool blocking, bool observable);

				// https://stackoverflow.com/a/120916/2097055
				List<ProtonixAction*> &Actions ();
				ProtonixAction* Action (String name);
				ProtonixAction* ActionRegister (ProtonixAction* action);
				ProtonixAction* ActionRegister (String name);
				ProtonixAction* ActionRegister (String name, unsigned int interval);
				ProtonixAction* ActionRegister (String name, unsigned int interval, int stepEnd);
				ProtonixAction* ActionRegister (String name, unsigned int interval, int stepBegin, int stepEnd);
				ProtonixAction* ActionRegister (String name, unsigned int interval, int stepBegin, int stepEnd, int step);
				bool ActionTrigger (String name); // deprecated
				bool ActionStart (String name);
				bool ActionPlay (String name);
				bool ActionPause (String name);
				bool ActionStop (String name);
				bool ActionPipe (ProtonixAction* action);
				void ActionReset ();

				void Pipe ();

				void OnSerial (ProtonixDevicePort* port, IProtonixCommand* command);
				bool SerialCommand (String port, IProtonixCommand* command);
				bool SerialCommandCustom (String port, String cmd);
				bool SerialCommandSensor (String port, ProtonixDeviceSensor* sensor);
				bool SerialCommandSensor (String port, String id, String value);
				bool SerialCommandSensor (String port, String id, String value, bool active);
				bool SerialCommandSensor (String port, String id, String value, bool active, bool failure);
				bool SerialCommandSensor (String port, String id, String value, bool active, bool failure, String state);
				bool SerialCommandSensor (String port, String id, bool active);
				bool SerialCommandSensor (String port, String id, bool active, bool failure);
				bool SerialCommandSensor (String port, String id, bool active, bool failure, String state);
				bool SerialCommandSensor (String port, String id, String value, String state);
				bool SerialStatus (String port);

				#if defined(ESP32) || defined(ESP8266)
				void Network (IProtonixNetwork* network);
				IProtonixNetwork* Network ();
				bool NetworkConnected ();
				void Protocol (IProtonixProtocol* protocol);
				IProtonixProtocol* Protocol ();
				bool ProtocolConnected ();
				void Server (ProtonixURI* uri);
				ProtonixURI* Server ();
				void ServerEndpoint (String host, unsigned int port);
				void ServerEndpoint (String host, unsigned int port, String path);
				void ServerBaseURI (String uri);
				bool Connected ();
				void OnStream (unsigned char* data);
				void RequestStream (String url, IProtonixDTORequest* request);
				void RequestStreamAuthorize ();
				ProtonixDTO* DTOInput ();
				ProtonixDTO* DTOOutput ();
				bool FirmwareUpdateOTA (String version = "");
				#endif

				#if defined(ESP32)
				void DedicateTaskCore0 ();
				void DedicateTaskCore1 ();
				#endif
		};
	}
}