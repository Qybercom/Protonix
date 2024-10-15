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

			class CStdRegistry : public ProtonixGenericCommand {
				private:
                    String _key;
                    String _value;

                public:
                	CStdRegistry();

                    CStdRegistry* Key(String key);
                    String Key();

                    CStdRegistry* Value(String value);
                    String Value();

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
