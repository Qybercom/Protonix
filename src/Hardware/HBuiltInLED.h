#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBuiltInLED : public IProtonixHardware {
				private:
					unsigned short _pin;

				public:
					HBuiltInLED (unsigned short pin = QYBERCOM_PROTONIX_LED_PIN);

					void Active (bool active);

					void HardwareInitPre (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}