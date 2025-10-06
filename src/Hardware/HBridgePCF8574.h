#pragma once

#include <Arduino.h>

#include "PCF8574.h"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBridgePCF8574 : public IProtonixBridge {
				private:
					int _address;
					PCF8574* _driver;

				public:
					HBridgePCF8574 (int address);
					static HBridgePCF8574* Init (int address);

					int Address ();
					HBridgePCF8574* Address (int address);

					PCF8574* Driver ();

					bool HardwareI2C ();
					void HardwareI2CPre (Protonix* device);
					void HardwareI2CPost (Protonix* device);

					bool BridgePinMode (unsigned int pin, int mode);
					bool BridgeDigitalRead (unsigned int pin);
					bool BridgeDigitalWrite (unsigned int pin, bool value);
					int BridgeAnalogRead (unsigned int pin);
					bool BridgeAnalogWrite (unsigned int pin, int value);
			};
		}
	}
}