#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBridgeDefault : public IProtonixBridge {
				public:
					HBridgeDefault ();

					bool BridgePinMode (unsigned int pin, int mode);

					bool BridgeDigitalRead (unsigned int pin);
					bool BridgeDigitalWrite (unsigned int pin, bool value);

					int BridgeAnalogRead (unsigned int pin);
					bool BridgeAnalogWrite (unsigned int pin, int value);
			};
		}
	}
}