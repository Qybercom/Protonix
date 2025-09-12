#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSensorMicrophone : public IProtonixHardware {
				private:
					unsigned short _pin;
					bool _active;

				public:
					HSensorMicrophone (unsigned short pin);

					unsigned short Pin ();
					HSensorMicrophone* Pin (unsigned short pin);

					bool Active ();

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwareCommand (ProtonixDevice* device, String command);
			};
		}
	}
}