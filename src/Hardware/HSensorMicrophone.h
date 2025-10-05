#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSensorMicrophone : public IProtonixHardware {
				private:
					unsigned short _pin;
					bool _active;

					void _signal (Protonix* device);

				public:
					HSensorMicrophone (unsigned short pin);

					unsigned short Pin ();
					HSensorMicrophone* Pin (unsigned short pin);

					bool Active ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}