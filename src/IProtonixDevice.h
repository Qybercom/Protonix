#pragma once

#include <Arduino.h>

#include "IProtonixCommand.h"
#include "ProtonixDevice.h"
#include "ProtonixDevicePort.h"
#include "ProtonixAction.h"

#if defined(ESP32) || defined(ESP8266)
#include "ProtonixDTO.h"
#include "DTO/DTOResponseAuthorization.h"
#include "DTO/DTOEventCommand.h"
#endif

namespace Qybercom {
	namespace Protonix {
		class IProtonixDevice {
			public:
				virtual String DeviceID ();
				virtual unsigned int DeviceTick ();
				virtual void DeviceOnReady (ProtonixDevice* device);
				virtual void DeviceOnLoop (ProtonixDevice* device);
				virtual void DeviceOnTick (ProtonixDevice* device);
				virtual void DeviceOnSerialCommand (ProtonixDevice* device, ProtonixDevicePort* port, IProtonixCommand* command);
				virtual void DeviceOnAction (ProtonixDevice* device, ProtonixAction* action);
				virtual bool DeviceAutoStatus ();
				
				#if defined(ESP32) || defined(ESP8266)
				virtual String DevicePassphrase ();
				virtual void DeviceOnNetworkConnect (ProtonixDevice* device);
				virtual void DeviceOnProtocolConnect (ProtonixDevice* device);
				virtual void DeviceOnStreamResponse (ProtonixDevice* device, ProtonixDTO* dto);
				virtual void DeviceOnStreamResponseAuthorization (ProtonixDevice* device, DTO::DTOResponseAuthorization* authorization);
				virtual void DeviceOnStreamEvent (ProtonixDevice* device, ProtonixDTO* dto);
				virtual void DeviceOnStreamEventCommand (ProtonixDevice* device, DTO::DTOEventCommand* command);
				#endif

				#if defined(ESP32)
				virtual void DeviceOnDedicatedTask (unsigned short core);
				#endif
		};
	}
}