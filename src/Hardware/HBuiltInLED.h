#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBuiltInLED : public IProtonixHardware {
				public:
					HBuiltInLED (unsigned short pin = QYBERCOM_PROTONIX_LED_PIN);

					void Active (bool active);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}