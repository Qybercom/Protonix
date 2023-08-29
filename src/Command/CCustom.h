#pragma once

#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"
#include "../ProtonixDevicePort.h"
#include "../ProtonixGenericCommand.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CCustom : public ProtonixGenericCommand {
				private:
					String _cmd;

				public:
					CCustom();
					String CMD();
					bool CommandRecognize(ProtonixDevice* device, ProtonixDevicePort* port, String name);
					bool CommandSerialize();
					void CommandReset();
					#if defined(ESP32) || defined(ESP8266)
					void CommandFromDTO(DTO::DTOEventCommand* dto);
					#endif
			};
		}
	}
}