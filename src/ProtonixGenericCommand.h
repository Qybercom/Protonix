#pragma once

#include <Arduino.h>

#include "IProtonixDevice.h"
#include "IProtonixCommand.h"
#include "ProtonixDevice.h"

#if defined(ESP32) || defined(ESP8266)
#include "DTO/DTOEventCommand.h"
#endif

namespace Qybercom {
	namespace Protonix {
		class ProtonixGenericCommand : public IProtonixCommand {
			protected:
				String _name;
				String _output;
				void _init(String name);

			public:
				String CommandName();
				String CommandOutput();
				virtual bool CommandRecognize(ProtonixDevice* device, ProtonixDevicePort* port, String name) = 0;
				virtual bool CommandSerialize() = 0;
				virtual void CommandReset() = 0;
				#if defined(ESP32) || defined(ESP8266)
				virtual void CommandFromDTO(DTO::DTOEventCommand* dto) = 0;
				#endif
		};
	}
}