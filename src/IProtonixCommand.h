#pragma once

#include <Arduino.h>

#include "IProtonixDevice.h"
#include "ProtonixDevice.h"
#include "ProtonixDevicePort.h"

#if defined(ESP32) || defined(ESP8266)
#include "DTO/DTOEventCommand.h"
#endif

namespace Qybercom {
	namespace Protonix {
		class IProtonixCommand {
			public:
				virtual String CommandName ();
				virtual String CommandOutput ();
				virtual bool CommandRecognize (ProtonixDevice* device, ProtonixDevicePort* port, String command);
				virtual bool CommandSerialize ();
				virtual void CommandReset ();
				#if defined(ESP32) || defined(ESP8266)
				virtual void CommandFromDTO (DTO::DTOEventCommand* dto);
				#endif
				virtual ~IProtonixCommand () {};
		};
	}
}