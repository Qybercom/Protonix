#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBridgePCF8574 : public IProtonixBridge {
				private:
					char _address;
					char _dataOut;
					char _modeMask;
					bool _init;

					bool _probe ();
					char _read ();
					bool _write (char value);

				public:
					HBridgePCF8574 (char address);
					static HBridgePCF8574* Init (char address);

					char Address ();
					HBridgePCF8574* Address (char address);

					bool HardwareI2C ();
					void HardwareI2CPre (Protonix* device);
					void HardwareI2CPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);

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