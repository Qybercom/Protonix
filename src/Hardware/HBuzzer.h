#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

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

					void Play (short value, unsigned long duration = 0);
					void Stop ();

					bool HardwareSPI ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareCommand (Protonix* device, String command);
			};
		}
	}
}