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
					byte _address;
					byte _dataOut;
					byte _modeMask;
					bool _init;

					bool _probe ();
					byte _read ();
					bool _write (byte value);

				public:
					HBridgePCF8574 (byte address);
					static HBridgePCF8574* Init (byte address);

					byte Address ();
					HBridgePCF8574* Address (byte address);

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