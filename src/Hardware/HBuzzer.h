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

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}