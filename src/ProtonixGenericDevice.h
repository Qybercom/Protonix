#pragma once

#include <Arduino.h>

#include "IProtonixDevice.h"
#include "IProtonixCommand.h"
#include "ProtonixDevice.h"
#include "ProtonixDevicePort.h"

#if defined(ESP32) || defined(ESP8266)
#include "ProtonixDTO.h"
#include "DTO/DTOResponseAuthorization.h"
#include "DTO/DTOEventCommand.h"
#endif

namespace Qybercom {
	namespace Protonix {
		class ProtonixGenericDevice : public IProtonixDevice {
			protected:
				bool _debug;
				IProtonixCommand* _cmds[4];
				void _init();
				void _init(bool debug);

			public:
				void LED(bool on);

				// https://stackoverflow.com/a/16248582/2097055
				virtual String DeviceID() = 0;
				virtual unsigned int DeviceTick() = 0;
				virtual void DeviceOnReady(ProtonixDevice* device) = 0;
				virtual void DeviceOnTick(ProtonixDevice* device) = 0;
				virtual void DeviceOnCommand(ProtonixDevice* device, ProtonixDevicePort* port, IProtonixCommand* command) = 0;
				void DeviceOnSerialCommand(ProtonixDevice* device, ProtonixDevicePort* port, IProtonixCommand* command);
				bool DeviceAutoStatus();

				#if defined(ESP32) || defined(ESP8266)
				virtual String DevicePassphrase() = 0;
				void DeviceOnNetworkConnect(ProtonixDevice* device);
				void DeviceOnProtocolConnect(ProtonixDevice* device);
				void DeviceOnStreamResponse(ProtonixDevice* device, ProtonixDTO* dto);
				void DeviceOnStreamResponseAuthorization(ProtonixDevice* device, DTO::DTOResponseAuthorization* authorization);
				void DeviceOnStreamEvent(ProtonixDevice* device, ProtonixDTO* dto);
				void DeviceOnStreamEventCommand(ProtonixDevice* device, DTO::DTOEventCommand* command);
				#endif
		};
	}
}