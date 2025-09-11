#pragma once

#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"
#include "../ProtonixDevicePort.h"
#include "../ProtonixDeviceSensor.h"
#include "../ProtonixGenericCommand.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CStdSensor : public ProtonixGenericCommand {
				private:
					ProtonixDeviceSensor* _sensor;

				public:
					CStdSensor ();
					CStdSensor (ProtonixDeviceSensor* sensor);

					void Sensor (ProtonixDeviceSensor* sensor);
					ProtonixDeviceSensor* Sensor ();

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