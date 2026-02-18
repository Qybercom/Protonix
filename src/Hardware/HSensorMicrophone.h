#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSensorMicrophone : public IProtonixHardware {
				private:
					bool _active;

					void _signal (Protonix* device);

				public:
					HSensorMicrophone (unsigned short pin);

					bool Active ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, const ProtonixCommand &command);
			};
		}
	}
}