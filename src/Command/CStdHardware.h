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
			class CStdHardware : public ProtonixGenericCommand {
				private:
					String _id;
					String _cmd;

				public:
					CStdHardware ();

					CStdHardware* ID (String id);
					String ID ();

					CStdHardware* CMD (String cmd);
					String CMD ();

					bool CommandRecognize (ProtonixDevice* device, ProtonixDevicePort* port, String name);
					bool CommandSerialize ();
					void CommandReset ();
					#if defined(ESP32) || defined(ESP8266)
					void CommandFromDTO (DTO::DTOEventCommand* dto);
					#endif
			};
		}
	}
}