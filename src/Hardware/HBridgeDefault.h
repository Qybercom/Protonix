#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBridgeDefault : public IProtonixBridge {
				public:
					HBridgeDefault ();

					bool BridgePinDualPolarity (unsigned int pin);

					bool BridgePinInitInput (unsigned int pin, int initial = 0);
					bool BridgePinInitInputUp (unsigned int pin, int initial = 0);
					bool BridgePinInitInputDown (unsigned int pin, int initial = 0);
					bool BridgePinInitOutput (unsigned int pin);

					bool BridgePinMode (unsigned int pin, int mode);

					bool BridgePinAvailable (unsigned int pin);

					bool BridgeDigitalRead (unsigned int pin);
					bool BridgeDigitalWrite (unsigned int pin, bool value);

					int BridgeAnalogRead (unsigned int pin);
					bool BridgeAnalogWrite (unsigned int pin, int value);
			};
		}
	}
}