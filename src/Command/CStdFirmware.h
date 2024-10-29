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
			class CStdFirmware : public ProtonixGenericCommand {
				private:
                    String _action;
                    String _version;

                public:
                	CStdFirmware();

                    CStdFirmware* Action(String action);
                    String Action();

                    bool ActionUpdate();

                    CStdFirmware* Version(String version);
                    String Version();

                    bool VersionLatest();

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