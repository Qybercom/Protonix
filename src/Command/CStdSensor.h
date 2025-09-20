#pragma once

#include <Arduino.h>

#include "../IProtonixCommand.h"
#include "../IProtonixHardware.h"
#include "../Protonix.h"
#include "../ProtonixSensor.h"

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CStdSensor : public IProtonixCommand {
				private:
					ProtonixSensor* _sensor;

				public:
					CStdSensor ();
					CStdSensor (ProtonixSensor* sensor);

					void Sensor (ProtonixSensor* sensor);
					ProtonixSensor* Sensor ();

					bool CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware);
					bool CommandSerialize ();
			};
		}
	}
}