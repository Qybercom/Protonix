#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBuzzer : public IProtonixHardware {
				private:
					unsigned short _pin;
					unsigned long _started;
					unsigned long _duration;

				public:
					HBuzzer (unsigned short pin);

					unsigned short Pin ();
					HBuzzer* Pin (unsigned short pin);

					void On (short value);
					void Off ();
					void Play (short value, unsigned long duration);
					void Stop ();

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwareCommand (ProtonixDevice* device, String command);
			};
		}
	}
}