#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HGenericAnalog : public IProtonixHardware {
				private:
					int _value;

				public:
					HGenericAnalog (unsigned short pin, bool init = false);

					int Value ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);
			};
		}
	}
}